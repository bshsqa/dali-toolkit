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

// CLASS HEADER
#include "model-view-impl.h"

// EXTERNAL INCLUDES
#include <dali-scene-loader/public-api/animation-definition.h>
#include <dali-scene-loader/public-api/camera-parameters.h>
#include <dali-scene-loader/public-api/dli-loader.h>
#include <dali-scene-loader/public-api/gltf2-loader.h>
#include <dali-scene-loader/public-api/light-parameters.h>
#include <dali-scene-loader/public-api/load-result.h>
#include <dali-scene-loader/public-api/node-definition.h>
#include <dali-scene-loader/public-api/scene-definition.h>
#include <dali-scene-loader/public-api/shader-definition-factory.h>
#include <dali/devel-api/adaptor-framework/image-loading.h>
#include <dali/devel-api/adaptor-framework/pixel-buffer.h>
#include <filesystem>

// INTERNAL INCLUDES
#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/controls/control-devel.h>
#include <dali-toolkit/internal/controls/control/control-data-impl.h>
#include <dali-toolkit/internal/graphics/builtin-shader-extern-gen.h>
#include <dali-toolkit/public-api/controls/model-view/model-view.h>

using namespace Dali;

namespace Dali
{
namespace Toolkit
{
namespace Internal
{
namespace
{

/**
 * @brief cube map face index
 * Cube map layer order is as fallows:
 * POSITIVE_X, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z. @see CubeMapLayer
 * The indices are for 4 kind of environment cube map. Cross_horizontal, Array_horizontal, Cross_vertical, and Array_vertical.
 */
const uint32_t CUBEMAP_INDEX_X[4][6] = {{2, 0, 1, 1, 1, 3}, {0, 1, 3, 2, 4, 5}, {1, 1, 1, 1, 0, 2}, {0, 0, 0, 0, 0, 0}};
const uint32_t CUBEMAP_INDEX_Y[4][6] = {{1, 1, 2, 0, 1, 1}, {0, 0, 0, 0, 0, 0}, {1, 3, 2, 0, 1, 1}, {0, 1, 3, 2, 4, 5}};

enum CubeType
{
  CROSS_HORIZONTAL = 0, // Cross horizontal style cube map
  ARRAY_HORIZONTAL,     // array horizontal style cube map
  CROSS_VERTICAL,       // Cross vertical style cube map
  ARRAY_VERTICAL,       // array vertical style cube map
  NONE
};

const std::string_view KTX_EXTENSION = ".ktx";

const std::string_view OBJ_EXTENSION  = ".obj";
const std::string_view GLTF_EXTENSION = ".gltf";
const std::string_view DLI_EXTENSION  = ".dli";

uint8_t* GetCroppedBuffer(uint8_t* sourceBuffer, uint32_t bytesPerPixel, uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset, uint32_t xFaceSize, uint32_t yFaceSize)
{
  uint32_t byteSize   = bytesPerPixel * xFaceSize * yFaceSize;
  uint8_t* destBuffer = reinterpret_cast<uint8_t*>(malloc(byteSize + 4u));

  int32_t srcStride  = width * bytesPerPixel;
  int32_t destStride = xFaceSize * bytesPerPixel;
  int32_t srcOffset  = xOffset * bytesPerPixel + yOffset * srcStride;
  int32_t destOffset = 0;
  for(uint16_t row = yOffset; row < yOffset + yFaceSize; ++row)
  {
    memcpy(destBuffer + destOffset, sourceBuffer + srcOffset, destStride);
    srcOffset += srcStride;
    destOffset += destStride;
  }

  return destBuffer;
}

void UploadTextureFace(Texture& texture, Devel::PixelBuffer pixelBuffer, uint32_t faceIndex, CubeType cubeType, uint32_t faceSize)
{
  if(cubeType != NONE)
  {
    uint8_t* imageBuffer   = pixelBuffer.GetBuffer();
    uint32_t bytesPerPixel = Pixel::GetBytesPerPixel(pixelBuffer.GetPixelFormat());
    uint32_t imageWidth    = pixelBuffer.GetWidth();
    uint32_t imageHeight   = pixelBuffer.GetHeight();

    uint32_t xOffset = CUBEMAP_INDEX_X[cubeType][faceIndex] * faceSize;
    uint32_t yOffset = CUBEMAP_INDEX_Y[cubeType][faceIndex] * faceSize;

    uint8_t*  tempImageBuffer = GetCroppedBuffer(imageBuffer, bytesPerPixel, imageWidth, imageHeight, xOffset, yOffset, faceSize, faceSize);
    PixelData pixelData       = PixelData::New(tempImageBuffer, faceSize * faceSize * bytesPerPixel, faceSize, faceSize, pixelBuffer.GetPixelFormat(), PixelData::FREE);
    texture.Upload(pixelData, CubeMapLayer::POSITIVE_X + faceIndex, 0, 0, 0, faceSize, faceSize);
  }
}

Texture LoadCubeMap(const std::string& cubeMapPath)
{
  std::filesystem::path modelPath(cubeMapPath);
  std::string           extension = modelPath.extension();
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  Texture cubeTexture;
  if(extension == KTX_EXTENSION)
  {
    SceneLoader::CubeData cubeData;
    if(SceneLoader::LoadCubeMapData(cubeMapPath, cubeData))
    {
      cubeTexture = cubeData.CreateTexture();
    }
    else
    {
      DALI_LOG_ERROR("Fail to load cube map, %s\n", cubeMapPath.c_str());
    }
  }
  else
  {
    // Diffuse Cube Map
    Devel::PixelBuffer pixelBuffer = LoadImageFromFile(cubeMapPath);
    if(pixelBuffer)
    {
      uint32_t imageWidth  = pixelBuffer.GetWidth();
      uint32_t imageHeight = pixelBuffer.GetHeight();

      uint32_t faceSize = 0;
      CubeType cubeType = NONE;
      if(imageWidth / 4 == imageHeight / 3)
      {
        cubeType = CROSS_HORIZONTAL;
        faceSize = imageWidth / 4;
      }
      else if(imageWidth / 6 == imageHeight)
      {
        cubeType = ARRAY_HORIZONTAL;
        faceSize = imageHeight;
      }
      else if(imageWidth / 3 == imageHeight / 4)
      {
        cubeType = CROSS_VERTICAL;
        faceSize = imageHeight / 4;
      }
      else if(imageWidth == imageHeight / 6)
      {
        cubeType = ARRAY_VERTICAL;
        faceSize = imageWidth;
      }
      else
      {
        DALI_LOG_ERROR("The image is not a cube map\n");
      }

      cubeTexture = Texture::New(TextureType::TEXTURE_CUBE, pixelBuffer.GetPixelFormat(), faceSize, faceSize);
      for(uint32_t i = 0; i < 6; ++i)
      {
        UploadTextureFace(cubeTexture, pixelBuffer, i, cubeType, faceSize);
      }
      cubeTexture.GenerateMipmaps();
    }
  }

  return cubeTexture;
}

void ConfigureBlendShapeShaders(
  SceneLoader::ResourceBundle& resources, const SceneLoader::SceneDefinition& scene, Actor root, std::vector<SceneLoader::BlendshapeShaderConfigurationRequest>&& requests)
{
  std::vector<std::string> errors;
  auto                     onError = [&errors](const std::string& msg)
  { errors.push_back(msg); };
  if(!scene.ConfigureBlendshapeShaders(resources, root, std::move(requests), onError))
  {
    SceneLoader::ExceptionFlinger flinger(ASSERT_LOCATION);
    for(auto& msg : errors)
    {
      flinger << msg << '\n';
    }
  }
}

} // anonymous namespace

ModelView::ModelView(const std::string& modelPath, const std::string& resourcePath)
: Control(ControlBehaviour(CONTROL_BEHAVIOUR_DEFAULT)),
  mModelPath(modelPath),
  mResourcePath(resourcePath),
  mModelLayer(),
  mModelRoot()
{
}

ModelView::~ModelView()
{
}

Toolkit::ModelView ModelView::New(const std::string& modelPath, const std::string& resourcePath)
{
  ModelView* impl = new ModelView(modelPath, resourcePath);

  Dali::Toolkit::ModelView handle = Dali::Toolkit::ModelView(*impl);

  // Second-phase init of the implementation
  // This can only be done after the CustomActor connection has been made...
  impl->Initialize();

  return handle;
}

void ModelView::SetProperty(BaseObject* object, Property::Index index, const Property::Value& value)
{
}

Property::Value ModelView::GetProperty(BaseObject* object, Property::Index index)
{
  Property::Value value;

  return value;
}

void ModelView::PlayAnimation()
{
  if(mAnimation)
  {
    mAnimation.Play();
  }
}

void ModelView::SetImageBasedLightSource(const std::string& diffuse, const std::string& specular, float scaleFactor)
{
  // Diffuse Cube Map
  mDiffuseTexture  = LoadCubeMap(diffuse);
  mSpecularTexture = LoadCubeMap(specular);
  mIblScaleFactor  = scaleFactor;

  SetImageBasedLight(mModelRoot);
}

/////////////////////////////////////////////////////////////

void ModelView::OnSceneConnection(int depth)
{
  if(!mModelRoot)
  {
    LoadModel();
  }
}

void ModelView::LoadModel()
{
  std::filesystem::path modelPath(mModelPath);
  if(mResourcePath.empty())
  {
    mResourcePath = std::string(modelPath.parent_path()) + "/";
  }
  std::string extension = modelPath.extension();
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  SceneLoader::ResourceBundle::PathProvider pathProvider = [&](SceneLoader::ResourceType::Value type)
  {
    return mResourcePath;
  };

  SceneLoader::ResourceBundle                        resources;
  SceneLoader::SceneDefinition                       scene;
  std::vector<SceneLoader::AnimationGroupDefinition> animGroups;
  std::vector<SceneLoader::CameraParameters>         cameraParameters;
  std::vector<SceneLoader::LightParameters>          lights;

  std::vector<Dali::SceneLoader::AnimationDefinition> animations;
  animations.clear();

  SceneLoader::LoadResult output{resources, scene, animations, animGroups, cameraParameters, lights};

  if(extension == DLI_EXTENSION)
  {
    SceneLoader::DliLoader              loader;
    SceneLoader::DliLoader::InputParams input{
      pathProvider(SceneLoader::ResourceType::Mesh),
      nullptr,
      {},
      {},
      nullptr,
      {}};
    SceneLoader::DliLoader::LoadParams loadParams{input, output};
    if(!loader.LoadScene(mModelPath, loadParams))
    {
      SceneLoader::ExceptionFlinger(ASSERT_LOCATION) << "Failed to load scene from '" << mModelPath << "': " << loader.GetParseError();
    }
  }
  else
  {
    SceneLoader::ShaderDefinitionFactory sdf;
    sdf.SetResources(resources);
    SceneLoader::LoadGltfScene(mModelPath, sdf, output);

    resources.mEnvironmentMaps.push_back({});
  }

  SceneLoader::Transforms                   xforms{SceneLoader::MatrixStack{}, SceneLoader::ViewProjection{}};
  SceneLoader::NodeDefinition::CreateParams nodeParams{resources, xforms, {}, {}, {}};
  SceneLoader::Customization::Choices       choices;

  mModelRoot = Actor::New();
  mModelRoot.SetResizePolicy(ResizePolicy::FILL_TO_PARENT,
                             Dimension::ALL_DIMENSIONS);
  SceneLoader::SetActorCentered(mModelRoot);

  for(auto iRoot : scene.GetRoots())
  {
    auto resourceRefs = resources.CreateRefCounter();
    scene.CountResourceRefs(iRoot, choices, resourceRefs);
    resources.CountEnvironmentReferences(resourceRefs);

    resources.LoadResources(resourceRefs, pathProvider);

    if(auto actor = scene.CreateNodes(iRoot, choices, nodeParams))
    {
      scene.ConfigureSkeletonJoints(iRoot, resources.mSkeletons, actor);
      scene.ConfigureSkinningShaders(resources, actor, std::move(nodeParams.mSkinnables));
      ConfigureBlendShapeShaders(resources, scene, actor, std::move(nodeParams.mBlendshapeRequests));

      scene.ApplyConstraints(actor, std::move(nodeParams.mConstrainables));

      mModelRoot.Add(actor);
    }
  }

  if(!animations.empty())
  {
    auto getActor = [&](const std::string& name)
    {
      return mModelRoot.FindChildByName(name);
    };

    mAnimation = Dali::Animation::New(0.0f);
    /* TODO: Set Looping mode */
    for(auto&& animation : animations)
    {
      animation.Animate(mAnimation, getActor);
    }
  }

  SetImageBasedLight(mModelRoot);

  mModelLayer.Add(mModelRoot);
}

///////////////////////////////////////////////////////////
//
// Private methods
//

void ModelView::OnInitialize()
{
  Actor self  = Self();
  mModelLayer = Layer::New();
  mModelLayer.SetProperty(Layer::Property::BEHAVIOR, Layer::LAYER_3D);
  mModelLayer.SetProperty(Layer::Property::DEPTH_TEST, true);
  mModelLayer.SetResizePolicy(ResizePolicy::FILL_TO_PARENT,
                              Dimension::ALL_DIMENSIONS);
  // Winding order change.
  mModelLayer.SetProperty(Dali::Actor::Property::SCALE_Y, -1.0f);
  self.Add(mModelLayer);
}

void ModelView::SetImageBasedLight(Actor node)
{
  if(!mDiffuseTexture || !mSpecularTexture || !node)
  {
    return;
  }

  uint32_t rendererCount = node.GetRendererCount();
  if(rendererCount)
  {
    node.RegisterProperty(SceneLoader::NodeDefinition::GetIBLScaleFactorName().data(), mIblScaleFactor);
  }

  for(uint32_t i = 0; i < rendererCount; ++i)
  {
    Renderer renderer = node.GetRendererAt(i);
    if(renderer)
    {
      TextureSet textures = renderer.GetTextures();
      if(textures)
      {
        uint32_t textureCount = textures.GetTextureCount();
        // EnvMap requires at least 2 texture, diffuse and specular
        if(textureCount > 2u)
        {
          textures.SetTexture(textureCount - 2u, mDiffuseTexture);
          textures.SetTexture(textureCount - 1u, mSpecularTexture);
        }
      }
    }
  }

  uint32_t childrenCount = node.GetChildCount();
  for(uint32_t i = 0; i < childrenCount; ++i)
  {
    SetImageBasedLight(node.GetChildAt(i));
  }
}

} // namespace Internal
} // namespace Toolkit
} // namespace Dali
