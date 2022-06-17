#ifndef DALI_TOOLKIT_INTERNAL_MODEL_VIEW_H
#define DALI_TOOLKIT_INTERNAL_MODEL_VIEW_H

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

// EXTERNAL INCLUDES
#include <dali/public-api/actors/layer.h>
#include <dali/public-api/animation/animation.h>
#include <dali/public-api/rendering/texture.h>

// INTERNAL INCLUDES
#include <dali-toolkit/public-api/controls/control-impl.h>
#include <dali-toolkit/public-api/controls/model-view/model-view.h>

namespace Dali
{
namespace Toolkit
{
class ModelView;

namespace Internal
{
/**
 * @brief Impl class for ModelView.
 *
 * All the geometry loaded with the control is automatically centered and scaled to fit
 * the size of all the other controls. So the max is (0.5,0.5) and the min is (-0.5,-0.5)
 */
class ModelView : public Control
{
public:
  /**
   * @brief Create a new ModelView.
   *
   * @return A public handle to the newly allocated ModelView.
   */
  static Toolkit::ModelView New(const std::string& modelPath, const std::string& resourcePath = std::string());

  // Properties

  /**
   * @brief Called when a property of an object of this type is set.
   *
   * @param[in] object The object whose property is set.
   * @param[in] index The property index.
   * @param[in] value The new property value.
   */
  static void SetProperty(BaseObject* object, Property::Index index, const Property::Value& value);

  /**
   * @brief Called to retrieve a property of an object of this type.
   *
   * @param[in] object The object whose property is to be retrieved.
   * @param[in] index The property index.
   * @return The current value of the property.
   */
  static Property::Value GetProperty(BaseObject* object, Property::Index index);

  void PlayAnimation();

  void PauseAnimation();

  void StopAnimation();

  /**
   * @brief Sets whether the animation will loop.
   *
   * This function resets the loop count and should not be used with SetLoopCount(int).
   * Setting this parameter does not cause the animation to play.
   *
   * @param[in] looping True if the animation will loop
   */
  void SetLooping(bool looping);

  /**
   * @brief Enables looping for 'count' repeats.
   *
   * A zero is the same as SetLooping(true) i.e. repeat forever.
   * This function resets the looping value and should not be used with SetLooping(bool).
   * Setting this parameter does not cause the animation to play.
   *
   * @param[in] count The number of times to loop
   */
  void SetLoopCount(int32_t count);

  /**
   * @brief Gets the loop count.
   *
   * A zero is the same as SetLooping(true) ie repeat forever.
   * The loop count is initially 1 for play once.
   *
   * @return The number of times to loop
   */
  int32_t GetLoopCount();

  void SetImageBasedLightSource(const std::string& diffuse, const std::string& specular, float scaleFactor);

protected:
  /**
   * @brief Construct a new ModelView.
   */
  ModelView(const std::string& modelPath, const std::string& resourcePath);

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~ModelView();

private:
  /**
   * @copydoc Toolkit::Control::OnInitialize()
   */
  void OnInitialize() override;

  /**
   * @copydoc CustomActorImpl::OnSceneConnection()
   */
  void OnSceneConnection(int depth) override;

  void LoadModel();

  void SetImageBasedLight(Actor node);

private:
  std::string     mModelPath;
  std::string     mResourcePath;
  Dali::Layer     mModelLayer;
  Dali::Actor     mModelRoot;
  Dali::Animation mAnimation;

  Dali::Texture mSpecularTexture;
  Dali::Texture mDiffuseTexture;
  float         mIblScaleFactor;
};

} // namespace Internal

// Helpers for public-api forwarding methods
inline Toolkit::Internal::ModelView& GetImpl(Toolkit::ModelView& obj)
{
  DALI_ASSERT_ALWAYS(obj);
  Dali::RefObject& handle = obj.GetImplementation();
  return static_cast<Toolkit::Internal::ModelView&>(handle);
}

inline const Toolkit::Internal::ModelView& GetImpl(const Toolkit::ModelView& obj)
{
  DALI_ASSERT_ALWAYS(obj);
  const Dali::RefObject& handle = obj.GetImplementation();
  return static_cast<const Toolkit::Internal::ModelView&>(handle);
}

} // namespace Toolkit

} // namespace Dali

#endif // DALI_TOOLKIT_INTERNAL_MODEL_VIEW_H
