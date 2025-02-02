/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// INTERNAL INCLUDES
#include "dali-scene-loader/public-api/material-definition.h"

// EXTERNAL INCLUDES
#include "dali-toolkit/public-api/image-loader/sync-image-loader.h"

namespace Dali
{
using namespace Toolkit;

namespace SceneLoader
{
namespace
{
constexpr SamplerFlags::Type FILTER_MODES_FROM_DALI[]{
  SamplerFlags::FILTER_LINEAR | SamplerFlags::FILTER_MIPMAP_NEAREST,
  SamplerFlags::FILTER_LINEAR,
  SamplerFlags::FILTER_NEAREST,
  SamplerFlags::FILTER_LINEAR,
  SamplerFlags::FILTER_NEAREST | SamplerFlags::FILTER_MIPMAP_NEAREST,
  SamplerFlags::FILTER_LINEAR | SamplerFlags::FILTER_MIPMAP_NEAREST,
  SamplerFlags::FILTER_NEAREST | SamplerFlags::FILTER_MIPMAP_LINEAR,
  SamplerFlags::FILTER_LINEAR | SamplerFlags::FILTER_MIPMAP_LINEAR,
};

constexpr SamplerFlags::Type WRAP_MODES_FROM_DALI[]{
  SamplerFlags::WRAP_CLAMP,
  SamplerFlags::WRAP_CLAMP,
  SamplerFlags::WRAP_REPEAT,
  SamplerFlags::WRAP_MIRROR,
};

constexpr FilterMode::Type FILTER_MODES_TO_DALI[]{
  FilterMode::NEAREST,
  FilterMode::LINEAR,
  FilterMode::NEAREST_MIPMAP_NEAREST,
  FilterMode::LINEAR_MIPMAP_NEAREST,
  FilterMode::NEAREST_MIPMAP_LINEAR,
  FilterMode::LINEAR_MIPMAP_LINEAR,
};

constexpr WrapMode::Type WRAP_MODES_TO_DALI[]{
  WrapMode::REPEAT,
  WrapMode::CLAMP_TO_EDGE,
  WrapMode::MIRRORED_REPEAT};

const SamplerFlags::Type SINGLE_VALUE_SAMPLER = SamplerFlags::Encode(FilterMode::NEAREST, FilterMode::NEAREST, WrapMode::CLAMP_TO_EDGE, WrapMode::CLAMP_TO_EDGE);
} // namespace

SamplerFlags::Type SamplerFlags::Encode(FilterMode::Type minFilter, FilterMode::Type magFilter, WrapMode::Type wrapS, WrapMode::Type wrapT)
{
  return FILTER_MODES_FROM_DALI[minFilter] | ((FILTER_MODES_FROM_DALI[magFilter] & FILTER_MAG_BITS) << FILTER_MAG_SHIFT) |
         (WRAP_MODES_FROM_DALI[wrapS] << WRAP_S_SHIFT) | (WRAP_MODES_FROM_DALI[wrapT] << WRAP_T_SHIFT);
}

FilterMode::Type SamplerFlags::GetMinFilter(Type flags)
{
  return FILTER_MODES_TO_DALI[flags & FILTER_MIN_MASK];
}

FilterMode::Type SamplerFlags::GetMagFilter(Type flags)
{
  return FILTER_MODES_TO_DALI[(flags >> FILTER_MAG_SHIFT) & FILTER_MAG_MASK];
}

WrapMode::Type SamplerFlags::GetWrapS(Type flags)
{
  return WRAP_MODES_TO_DALI[(flags >> WRAP_S_SHIFT) & WRAP_S_MASK];
}

WrapMode::Type SamplerFlags::GetWrapT(Type flags)
{
  return WRAP_MODES_TO_DALI[(flags >> WRAP_T_SHIFT) & WRAP_T_MASK];
}

Sampler SamplerFlags::MakeSampler(Type flags)
{
  auto sampler = Sampler::New();
  sampler.SetFilterMode(GetMinFilter(flags), GetMagFilter(flags));
  sampler.SetWrapMode(GetWrapS(flags), GetWrapT(flags));
  return sampler;
}

TextureDefinition::TextureDefinition(const std::string& imageUri, SamplerFlags::Type samplerFlags)
: mImageUri(imageUri),
  mSamplerFlags(samplerFlags)
{
}

MaterialDefinition::RawData
MaterialDefinition::LoadRaw(const std::string& imagesPath) const
{
  RawData raw;

  const bool hasTransparency = MaskMatch(mFlags, TRANSPARENCY);
  // Why we add additional count here?
  uint32_t numBuffers = mTextureStages.size() + (hasTransparency ? !CheckTextures(ALBEDO) + !CheckTextures(METALLIC | ROUGHNESS) + !CheckTextures(NORMAL)
                                                                 : !CheckTextures(ALBEDO | METALLIC) + !CheckTextures(NORMAL | ROUGHNESS));
  if(numBuffers == 0)
  {
    return raw;
  }
  raw.mTextures.reserve(numBuffers);

  // Load textures
  auto iTexture   = mTextureStages.cbegin();
  auto checkStage = [&](uint32_t flags) {
    return iTexture != mTextureStages.end() && MaskMatch(iTexture->mSemantic, flags);
  };

  // Check for compulsory textures: Albedo, Metallic, Roughness, Normal
  if(checkStage(ALBEDO | METALLIC))
  {
    raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
    ++iTexture;

    if(checkStage(NORMAL | ROUGHNESS))
    {
      raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
      ++iTexture;
    }
    else // single value normal-roughness
    {
      const auto bufferSize = 4;
      uint8_t*   buffer     = new uint8_t[bufferSize]{0x7f, 0x7f, 0xff, 0xff}; // normal of (0, 0, 1), roughness of 1
      raw.mTextures.push_back({PixelData::New(buffer, bufferSize, 1, 1, Pixel::RGBA8888, PixelData::DELETE_ARRAY), SINGLE_VALUE_SAMPLER});
    }
  }
  else
  {
    if(checkStage(ALBEDO))
    {
      raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
      ++iTexture;
    }
    else if(mNeedAlbedoTexture) // single value albedo, albedo-alpha or albedo-metallic
    {
      uint32_t bufferSize = 4;
      uint8_t* buffer     = nullptr;
      auto     format     = Pixel::Format::RGBA8888;
      if(hasTransparency) // albedo-alpha
      {
        buffer    = new uint8_t[bufferSize];
        buffer[3] = static_cast<uint8_t>(mColor.a * 255.f);
      }
      else if(!checkStage(METALLIC | ROUGHNESS)) // albedo-metallic
      {
        buffer    = new uint8_t[bufferSize];
        buffer[3] = 0xff; // metallic of 1.0
      }
      else // albedo
      {
        bufferSize = 3;
        buffer     = new uint8_t[bufferSize];
        format     = Pixel::Format::RGB888;
      }
      buffer[0] = static_cast<uint8_t>(mColor.r * 255.f);
      buffer[1] = static_cast<uint8_t>(mColor.g * 255.f);
      buffer[2] = static_cast<uint8_t>(mColor.b * 255.f);
      raw.mTextures.push_back({PixelData::New(buffer, bufferSize, 1, 1, format, PixelData::DELETE_ARRAY), SINGLE_VALUE_SAMPLER});
    }

    // If we have transparency, or an image based albedo map, we will have to continue with separate metallicRoughness + normal.
    const bool createMetallicRoughnessAndNormal = hasTransparency || std::distance(mTextureStages.begin(), iTexture) > 0;
    if(checkStage(METALLIC | ROUGHNESS))
    {
      raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
      ++iTexture;
    }
    else if(createMetallicRoughnessAndNormal && mNeedMetallicRoughnessTexture)
    {
      // NOTE: we want to set both metallic and roughness to 1.0; dli uses the R & A channels,
      // glTF2 uses B & G, so we might as well just set all components to 1.0.
      const auto bufferSize = 4;
      uint8_t*   buffer     = new uint8_t[bufferSize]{0xff, 0xff, 0xff, 0xff};
      raw.mTextures.push_back({PixelData::New(buffer, bufferSize, 1, 1, Pixel::RGBA8888, PixelData::DELETE_ARRAY), SINGLE_VALUE_SAMPLER});
    }

    if(checkStage(NORMAL))
    {
      raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
      ++iTexture;
    }
    else if(mNeedNormalTexture)
    {
      if(createMetallicRoughnessAndNormal)
      {
        const auto bufferSize = 3;
        uint8_t*   buffer     = new uint8_t[bufferSize]{0x7f, 0x7f, 0xff}; // normal of (0, 0, 1)
        raw.mTextures.push_back({PixelData::New(buffer, bufferSize, 1, 1, Pixel::RGB888, PixelData::DELETE_ARRAY), SINGLE_VALUE_SAMPLER});
      }
      else // single-value normal-roughness
      {
        const auto bufferSize = 4;
        uint8_t*   buffer     = new uint8_t[bufferSize]{0x7f, 0x7f, 0xff, 0xff}; // normal of (0, 0, 1), roughness of 1.0
        raw.mTextures.push_back({PixelData::New(buffer, bufferSize, 1, 1, Pixel::RGBA8888, PixelData::DELETE_ARRAY), SINGLE_VALUE_SAMPLER});
      }
    }
  }

  // Extra textures.
  if(checkStage(SUBSURFACE))
  {
    raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
    ++iTexture;
  }

  if(checkStage(OCCLUSION))
  {
    raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
    ++iTexture;
  }

  if(checkStage(EMISSIVE))
  {
    raw.mTextures.push_back({SyncImageLoader::Load(imagesPath + iTexture->mTexture.mImageUri), iTexture->mTexture.mSamplerFlags});
    ++iTexture;
  }

  return raw;
}

TextureSet MaterialDefinition::Load(const EnvironmentDefinition::Vector& environments, RawData&& raw) const
{
  auto textureSet = TextureSet::New();

  uint32_t n = 0;
  for(auto& tData : raw.mTextures)
  {
    auto& pixels  = tData.mPixels;
    auto  texture = Texture::New(TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight());
    texture.Upload(tData.mPixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight());
    if(tData.mSamplerFlags & SamplerFlags::MIPMAP_MASK)
    {
      texture.GenerateMipmaps();
    }

    textureSet.SetTexture(n, texture);
    textureSet.SetSampler(n, SamplerFlags::MakeSampler(tData.mSamplerFlags));

    ++n;
  }

  // Assign textures to slots -- starting with 2D ones, then cubemaps, if any.
  if(mEnvironmentIdx < environments.size())
  {
    auto& envTextures = environments[mEnvironmentIdx].second;
    if(envTextures.mDiffuse)
    {
      textureSet.SetTexture(n, envTextures.mDiffuse);
      ++n;
    }

    if(envTextures.mSpecular)
    {
      auto specularSampler = Sampler::New();
      specularSampler.SetWrapMode(WrapMode::CLAMP_TO_EDGE, WrapMode::CLAMP_TO_EDGE, WrapMode::CLAMP_TO_EDGE);
      specularSampler.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR);

      textureSet.SetTexture(n, envTextures.mSpecular);
      textureSet.SetSampler(n, specularSampler);
      ++n;
    }

    // If pre-computed brdf texture is defined, set the texture.
    if(envTextures.mBrdf)
    {
      textureSet.SetTexture(n, envTextures.mBrdf);
    }
  }
  else
  {
    ExceptionFlinger(ASSERT_LOCATION) << "Environment index (" << mEnvironmentIdx << ") out of bounds (" << environments.size() << ").";
  }

  return textureSet;
}

bool MaterialDefinition::CheckTextures(uint32_t flags) const
{
  return std::find_if(mTextureStages.begin(), mTextureStages.end(), [flags](const TextureStage& ts) {
           return MaskMatch(ts.mSemantic, flags);
         }) != mTextureStages.end();
}

} // namespace SceneLoader
} // namespace Dali
