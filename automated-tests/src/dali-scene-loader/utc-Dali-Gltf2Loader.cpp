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

// Enable debug log for test coverage
#define DEBUG_ENABLED 1

#include <dali-test-suite-utils.h>
#include <string_view>
#include "dali-scene-loader/public-api/gltf2-loader.h"
#include "dali-scene-loader/public-api/load-result.h"
#include "dali-scene-loader/public-api/resource-bundle.h"
#include "dali-scene-loader/public-api/scene-definition.h"
#include "dali-scene-loader/public-api/shader-definition-factory.h"

using namespace Dali;
using namespace Dali::SceneLoader;

#define DALI_TEST_THROW(expression, exception, predicate) \
  {                                                       \
    bool daliTestThrowSuccess__ = false;                  \
    try                                                   \
    {                                                     \
      do                                                  \
      {                                                   \
        expression;                                       \
      } while(0);                                         \
      printf("No exception was thrown.\n");               \
    }                                                     \
    catch(std::decay<exception>::type & ex)               \
    {                                                     \
      daliTestThrowSuccess__ = predicate(ex);             \
    }                                                     \
    catch(...)                                            \
    {                                                     \
      printf("Wrong type of exception thrown.\n");        \
    }                                                     \
    DALI_TEST_CHECK(daliTestThrowSuccess__);              \
  }

namespace
{
struct Context
{
  ResourceBundle::PathProvider pathProvider = [](ResourceType::Value type) {
    return TEST_RESOURCE_DIR "/";
  };

  ResourceBundle  resources;
  SceneDefinition scene;

  std::vector<AnimationDefinition>      animations;
  std::vector<AnimationGroupDefinition> animationGroups;
  std::vector<CameraParameters>         cameras;
  std::vector<LightParameters>          lights;

  LoadResult loadResult{
    resources,
    scene,
    animations,
    animationGroups,
    cameras,
    lights};
};

struct ExceptionMessageStartsWith
{
  const std::string_view expected;

  bool operator()(const std::runtime_error& e)
  {
    const bool success = (0 == strncmp(e.what(), expected.data(), expected.size()));
    if(!success)
    {
      printf("Expected: %s, got: %s.\n", expected.data(), e.what());
    }
    return success;
  }
};

} // namespace

int UtcDaliGltfLoaderFailedToLoad(void)
{
  Context ctx;

  ShaderDefinitionFactory sdf;
  sdf.SetResources(ctx.resources);

  DALI_TEST_THROW(LoadGltfScene("non-existent.gltf", sdf, ctx.loadResult),
                  std::runtime_error,
                  ExceptionMessageStartsWith{"Failed to load"});

  DALI_TEST_EQUAL(0, ctx.scene.GetRoots().size());
  DALI_TEST_EQUAL(0, ctx.scene.GetNodeCount());

  DALI_TEST_EQUAL(0, ctx.resources.mEnvironmentMaps.size());
  DALI_TEST_EQUAL(0, ctx.resources.mMaterials.size());
  DALI_TEST_EQUAL(0, ctx.resources.mMeshes.size());
  DALI_TEST_EQUAL(0, ctx.resources.mShaders.size());
  DALI_TEST_EQUAL(0, ctx.resources.mSkeletons.size());

  DALI_TEST_EQUAL(0, ctx.cameras.size());
  DALI_TEST_EQUAL(0, ctx.lights.size());
  DALI_TEST_EQUAL(0, ctx.animations.size());
  DALI_TEST_EQUAL(0, ctx.animationGroups.size());

  END_TEST;
}

int UtcDaliGltfLoaderFailedToParse(void)
{
  Context ctx;

  ShaderDefinitionFactory sdf;
  sdf.SetResources(ctx.resources);

  DALI_TEST_THROW(LoadGltfScene(TEST_RESOURCE_DIR "/invalid.gltf", sdf, ctx.loadResult),
                  std::runtime_error,
                  ExceptionMessageStartsWith{"Failed to parse"});

  DALI_TEST_EQUAL(0, ctx.scene.GetRoots().size());
  DALI_TEST_EQUAL(0, ctx.scene.GetNodeCount());

  DALI_TEST_EQUAL(0, ctx.resources.mEnvironmentMaps.size());
  DALI_TEST_EQUAL(0, ctx.resources.mMaterials.size());
  DALI_TEST_EQUAL(0, ctx.resources.mMeshes.size());
  DALI_TEST_EQUAL(0, ctx.resources.mShaders.size());
  DALI_TEST_EQUAL(0, ctx.resources.mSkeletons.size());

  DALI_TEST_EQUAL(0, ctx.cameras.size());
  DALI_TEST_EQUAL(0, ctx.lights.size());
  DALI_TEST_EQUAL(0, ctx.animations.size());
  DALI_TEST_EQUAL(0, ctx.animationGroups.size());

  END_TEST;
}

int UtcDaliGltfLoaderSuccess1(void)
{
  Context ctx;

  ShaderDefinitionFactory sdf;
  sdf.SetResources(ctx.resources);

  LoadGltfScene(TEST_RESOURCE_DIR "/AnimatedCube.gltf", sdf, ctx.loadResult);

  DALI_TEST_EQUAL(1u, ctx.scene.GetRoots().size());
  DALI_TEST_EQUAL(6u, ctx.scene.GetNodeCount());

  // Default envmap is used
  DALI_TEST_EQUAL(1u, ctx.resources.mEnvironmentMaps.size());

  auto& materials = ctx.resources.mMaterials;
  DALI_TEST_EQUAL(2u, materials.size());
  const MaterialDefinition materialGroundTruth[]{
    {MaterialDefinition::ALBEDO | MaterialDefinition::EMISSIVE | MaterialDefinition::OCCLUSION |
       MaterialDefinition::NORMAL | MaterialDefinition::TRANSPARENCY |
       (0x80 << MaterialDefinition::ALPHA_CUTOFF_SHIFT),
     0,
     Color::WHITE,
     1.f,
     0.f,
     Vector4(1.000, 0.766, 0.336, 1.0),
     1.f,
     1.f,
     Vector3(0.2, 0.1, 0.0),
     true,
     false,
     true,
     {
       {MaterialDefinition::ALBEDO,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
       {MaterialDefinition::NORMAL,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
       {MaterialDefinition::OCCLUSION,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
       {MaterialDefinition::EMISSIVE,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
     }},
    {MaterialDefinition::ALBEDO | MaterialDefinition::METALLIC | MaterialDefinition::ROUGHNESS |
       MaterialDefinition::EMISSIVE | MaterialDefinition::OCCLUSION |
       MaterialDefinition::NORMAL | MaterialDefinition::GLTF_CHANNELS,
     0,
     Color::WHITE,
     1.f,
     0.f,
     Vector4(1.000, 0.766, 0.336, 1.0),
     1.f,
     1.f,
     Vector3(0.2, 0.1, 0.0),
     true,
     true,
     true,
     {
       {MaterialDefinition::ALBEDO,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
       {MaterialDefinition::METALLIC | MaterialDefinition::ROUGHNESS | MaterialDefinition::GLTF_CHANNELS,
        {"AnimatedCube_MetallicRoughness.png",
         SamplerFlags::Encode(FilterMode::NEAREST_MIPMAP_LINEAR, FilterMode::NEAREST, WrapMode::CLAMP_TO_EDGE, WrapMode::MIRRORED_REPEAT)}},
       {MaterialDefinition::NORMAL,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
       {MaterialDefinition::OCCLUSION,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
       {MaterialDefinition::EMISSIVE,
        {"AnimatedCube_BaseColor.png",
         SamplerFlags::Encode(FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR, WrapMode::CLAMP_TO_EDGE, WrapMode::REPEAT)}},
     }},
  };

  auto iMaterial = materials.begin();
  for(auto& m : materialGroundTruth)
  {
    printf("material %ld\n", iMaterial - materials.begin());
    auto& md = iMaterial->first;
    DALI_TEST_EQUAL(md.mFlags, m.mFlags);
    DALI_TEST_EQUAL(md.mEnvironmentIdx, m.mEnvironmentIdx);
    DALI_TEST_EQUAL(md.mColor, m.mColor);
    DALI_TEST_EQUAL(md.mMetallic, m.mMetallic);
    DALI_TEST_EQUAL(md.mRoughness, m.mRoughness);
    DALI_TEST_EQUAL(md.mBaseColorFactor, m.mBaseColorFactor);
    DALI_TEST_EQUAL(md.mNormalScale, m.mNormalScale);
    DALI_TEST_EQUAL(md.mOcclusionStrength, m.mOcclusionStrength);
    DALI_TEST_EQUAL(md.mEmissiveFactor, m.mEmissiveFactor);
    DALI_TEST_EQUAL(md.mNeedAlbedoTexture, m.mNeedAlbedoTexture);
    DALI_TEST_EQUAL(md.mNeedMetallicRoughnessTexture, m.mNeedMetallicRoughnessTexture);
    DALI_TEST_EQUAL(md.mNeedNormalTexture, m.mNeedNormalTexture);

    DALI_TEST_EQUAL(md.mTextureStages.size(), m.mTextureStages.size());
    auto iTexture = md.mTextureStages.begin();
    for(auto& ts : m.mTextureStages)
    {
      printf("texture %ld\n", iTexture - md.mTextureStages.begin());
      DALI_TEST_EQUAL(iTexture->mSemantic, ts.mSemantic);
      DALI_TEST_EQUAL(iTexture->mTexture.mImageUri, ts.mTexture.mImageUri);
      DALI_TEST_EQUAL(uint32_t(iTexture->mTexture.mSamplerFlags), uint32_t(ts.mTexture.mSamplerFlags)); // don't interpret it as a character
      ++iTexture;
    }
    ++iMaterial;
  }

  auto& meshes = ctx.resources.mMeshes;
  DALI_TEST_EQUAL(2u, meshes.size());

  using Blob     = MeshDefinition::Blob;
  using Accessor = MeshDefinition::Accessor;
  const MeshDefinition meshGroundTruth[]{
    {
      0,
      Geometry::TRIANGLES,
      "AnimatedCube.bin",
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
    },
    {
      0,
      Geometry::TRIANGLES,
      "AnimatedCube.bin",
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
      Accessor{Blob{0, 0}, {}},
    },
  };

  auto iMesh = meshes.begin();
  for(auto& m : meshGroundTruth)
  {
    printf("mesh %ld\n", iMesh - meshes.begin());

    auto& md = iMesh->first;
    DALI_TEST_EQUAL(md.mFlags, m.mFlags);
    DALI_TEST_EQUAL(md.mPrimitiveType, m.mPrimitiveType);
    for(auto mp : {
          &MeshDefinition::mIndices,
          &MeshDefinition::mPositions,
          &MeshDefinition::mNormals,
          &MeshDefinition::mTexCoords,
          &MeshDefinition::mColors,
          &MeshDefinition::mTangents,
          &MeshDefinition::mJoints0,
          &MeshDefinition::mWeights0})
    {
      DALI_TEST_EQUAL((md.*mp).IsDefined(), (m.*mp).IsDefined());
      DALI_TEST_EQUAL((md.*mp).mBlob.IsDefined(), (m.*mp).mBlob.IsDefined());
    }

    DALI_TEST_EQUAL(md.mBlendShapeHeader.IsDefined(), m.mBlendShapeHeader.IsDefined());

    ++iMesh;
  }

  DALI_TEST_EQUAL(2u, ctx.resources.mShaders.size());
  DALI_TEST_EQUAL(0u, ctx.resources.mSkeletons.size());

  DALI_TEST_EQUAL(3u, ctx.cameras.size());
  DALI_TEST_EQUAL(0u, ctx.lights.size());
  DALI_TEST_EQUAL(1u, ctx.animations.size());
  DALI_TEST_EQUAL(0u, ctx.animationGroups.size());

  END_TEST;
}

int UtcDaliGltfLoaderSuccessShort(void)
{
  TestApplication app;

  const std::string resourcePath = TEST_RESOURCE_DIR "/";
  auto              pathProvider = [resourcePath](ResourceType::Value) {
    return resourcePath;
  };

  Customization::Choices choices;
  for(auto modelName : {
        "2CylinderEngine",
        "AnimatedMorphCube",
        "AnimatedMorphSphere",
        "AnimatedTriangle",
        "BoxAnimated",
        "CesiumMan",
        "CesiumMilkTruck",
        "EnvironmentTest",
        "MetalRoughSpheres",
        "MorphPrimitivesTest",
        "MRendererTest",
        "SimpleSparseAccessor",
        "AnimatedCube",
      })
  {
    Context ctx;

    ShaderDefinitionFactory sdf;

    auto& resources = ctx.resources;
    resources.mEnvironmentMaps.push_back({});

    sdf.SetResources(resources);

    printf("%s\n", modelName);
    LoadGltfScene(resourcePath + modelName + ".gltf", sdf, ctx.loadResult);
    DALI_TEST_CHECK(ctx.scene.GetNodeCount() > 0);

    auto& scene = ctx.scene;
    for(auto iRoot : scene.GetRoots())
    {
      struct Visitor : NodeDefinition::IVisitor
      {
        struct ResourceReceiver : IResourceReceiver
        {
          std::vector<bool> mCounts;

          void Register(ResourceType::Value type, Index id) override
          {
            if(type == ResourceType::Mesh)
            {
              mCounts[id] = true;
            }
          }
        } receiver;

        void Start(NodeDefinition& n) override
        {
          if(n.mRenderable)
          {
            n.mRenderable->RegisterResources(receiver);
          }
        }

        void Finish(NodeDefinition& n) override
        {
        }
      } visitor;
      visitor.receiver.mCounts.resize(resources.mMeshes.size(), false);

      scene.Visit(iRoot, choices, visitor);
      for(uint32_t i0 = 0, i1 = resources.mMeshes.size(); i0 < i1; ++i0)
      {
        if(visitor.receiver.mCounts[i0])
        {
          auto raw = resources.mMeshes[i0].first.LoadRaw(resourcePath);
          DALI_TEST_CHECK(!raw.mAttribs.empty());

          resources.mMeshes[i0].second = resources.mMeshes[i0].first.Load(std::move(raw));
          DALI_TEST_CHECK(resources.mMeshes[i0].second.geometry);
        }
      }
    }
  }

  END_TEST;
}

int UtcDaliGltfLoaderMRendererTest(void)
{
  Context ctx;

  ShaderDefinitionFactory sdf;
  sdf.SetResources(ctx.resources);
  auto& resources = ctx.resources;

  LoadGltfScene(TEST_RESOURCE_DIR "/MRendererTest.gltf", sdf, ctx.loadResult);

  auto& scene = ctx.scene;
  auto& roots = scene.GetRoots();
  DALI_TEST_EQUAL(roots.size(), 1u);
  DALI_TEST_EQUAL(scene.GetNode(roots[0])->mName, "RootNode");
  DALI_TEST_EQUAL(scene.GetNode(roots[0])->mScale, Vector3(1.0f, 1.0f, 1.0f));

  DALI_TEST_EQUAL(scene.GetNodeCount(), 1u);

  ViewProjection viewProjection;
  Transforms     xforms{
    MatrixStack{},
    viewProjection};
  NodeDefinition::CreateParams nodeParams{
    resources,
    xforms,
  };

  Customization::Choices choices;

  TestApplication app;

  Actor root = Actor::New();
  SetActorCentered(root);
  for(auto iRoot : roots)
  {
    auto resourceRefs = resources.CreateRefCounter();
    scene.CountResourceRefs(iRoot, choices, resourceRefs);
    resources.CountEnvironmentReferences(resourceRefs);
    resources.LoadResources(resourceRefs, ctx.pathProvider);
    if(auto actor = scene.CreateNodes(iRoot, choices, nodeParams))
    {
      scene.ConfigureSkeletonJoints(iRoot, resources.mSkeletons, actor);
      scene.ConfigureSkinningShaders(resources, actor, std::move(nodeParams.mSkinnables));
      scene.ApplyConstraints(actor, std::move(nodeParams.mConstrainables));
      root.Add(actor);
    }
  }

  DALI_TEST_EQUAL(root.GetChildCount(), 1u);
  Actor child = root.GetChildAt(0);

  DALI_TEST_EQUAL(child.GetProperty(Actor::Property::NAME).Get<std::string>(), "RootNode");
  DALI_TEST_EQUAL(child.GetProperty(Actor::Property::SCALE).Get<Vector3>(), Vector3(1.0f, 1.0f, 1.0f));
  DALI_TEST_EQUAL(child.GetRendererCount(), 1u);
  DALI_TEST_EQUAL(child.GetRendererAt(0).GetTextures().GetTextureCount(), 4u);

  END_TEST;
}
