#ifndef DALI_TOOLKIT_MODEL_VIEW_H
#define DALI_TOOLKIT_MODEL_VIEW_H

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
#include <dali-toolkit/public-api/controls/control.h>

namespace Dali
{
namespace Toolkit
{
namespace Internal DALI_INTERNAL
{
class ModelView;
}

/**
 * @addtogroup dali_toolkit_controls_model_view
 * @{
 */

/**
 * @brief ModelView left-hand coordinate (CW) as like other DALi space.
 * If the model is defined in CCW, Call SetYInvert method to convert CCW to CW.
 */
class DALI_TOOLKIT_API ModelView : public Control
{
public:
  /**
   * @brief Create an initialized Scene3dView.
   * @param[in] modelPath model file path.(e.g., glTF, and dli).
   * @param[in] resourcePath resource file path that includes binary, image etc.
   * @note If resourcePath is empty, modelPath is used for resource path.
   * @return A handle to a newly allocated Dali resource
   */
  static ModelView New(const std::string& modelPath, const std::string& resourcePath = std::string());

  /**
   * @brief Creates an uninitialized ModelView.
   *
   * Only derived versions can be instantiated. Calling member
   * functions with an uninitialized Dali::Object is not allowed.
   */
  ModelView();

  /**
   * @brief Destructor.
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~ModelView();

  /**
   * @brief Copy constructor.
   * @param[in] modelView Handle to an object
   */
  ModelView(const ModelView& modelView);

  /**
   * @brief Move constructor
   * @SINCE_1_9.23
   *
   * @param[in] rhs A reference to the moved handle
   */
  ModelView(ModelView&& rhs);

  /**
   * @brief Assignment operator.
   * @param[in] modelView Handle to an object
   * @return reference to this
   */
  ModelView& operator=(const ModelView& modelView);

  /**
   * @brief Move assignment
   * @SINCE_1_9.23
   *
   * @param[in] rhs A reference to the moved handle
   * @return A reference to this
   */
  ModelView& operator=(ModelView&& rhs);

  /**
   * @brief Downcasts an Object handle to ModelView.
   *
   * If handle points to a ModelView, the downcast produces valid handle.
   * If not, the returned handle is left uninitialized.
   *
   * @param[in] handle Handle to an object
   * @return Handle to a ModelView or an uninitialized handle
   */
  static ModelView DownCast(BaseHandle handle);

  void PlayAnimation();

  // TODO : documentation - for custom shader : last 2 sampler should be defined for IBL diffuse, specular
  void SetImageBasedLightSource(const std::string& diffuse, const std::string& specular, float scaleFactor = 1.0f);

public: // Not intended for application developers
  /// @cond internal
  /**
   * @brief Creates a handle using the Toolkit::Internal implementation.
   *
   * @param[in] implementation The Control implementation
   */
  DALI_INTERNAL ModelView(Internal::ModelView& implementation);

  /**
   * @brief Allows the creation of this Control from an Internal::CustomActor pointer.
   *
   * @SINCE_1_1.4
   * @param[in] internal A pointer to the internal CustomActor
   */
  DALI_INTERNAL ModelView(Dali::Internal::CustomActor* internal);
  /// @endcond
};

/**
 * @}
 */
} // namespace Toolkit

} // namespace Dali

#endif // DALI_TOOLKIT_MODEL_VIEW_H
