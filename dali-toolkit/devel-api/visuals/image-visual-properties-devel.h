#ifndef DALI_TOOLKIT_DEVEL_API_VISUALS_IMAGE_VISUAL_PROPERTIES_DEVEL_H
#define DALI_TOOLKIT_DEVEL_API_VISUALS_IMAGE_VISUAL_PROPERTIES_DEVEL_H

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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
#include <dali-toolkit/public-api/visuals/image-visual-properties.h>

namespace Dali
{

namespace Toolkit
{

namespace DevelImageVisual
{

/**
 * @brief The policy determining if the image is loaded when the visual is staged or created.
 */
namespace LoadPolicy
{

/**
 * @brief The available named elements that define the LoadPolicy.
 */
enum Type
{
  IMMEDIATE = 0,  ///< The image is loaded when the ImageVisual is created.
  ATTACHED        ///< The image is loaded when the ImageVisual is attached to the stage.
};

} // namespace LoadPolicy

/**
 * @brief The policy determining when a image is deleted from the cache in relation to the ImageVisual lifetime.
 * @note If the texture is being shared by another visual it persist if still required.
 */
namespace ReleasePolicy
{

/**
 * @brief The available named elements that define the ReleasePolicy.
 */
enum Type
{
  DETACHED = 0,  ///<  Image deleted from cache when ImageVisual detached from stage.
  DESTROYED,     ///<  Image deleted from cache when ImageVisual destroyed.
  NEVER          ///<  Image is never deleted, will survive the lifetime of the application.
};

} // namespace ReleasePolicy;

namespace Property
{

enum Type
{
  URL                 = Dali::Toolkit::ImageVisual::Property::URL,
  FITTING_MODE        = Dali::Toolkit::ImageVisual::Property::FITTING_MODE,
  SAMPLING_MODE       = Dali::Toolkit::ImageVisual::Property::SAMPLING_MODE,
  DESIRED_WIDTH       = Dali::Toolkit::ImageVisual::Property::DESIRED_WIDTH,
  DESIRED_HEIGHT      = Dali::Toolkit::ImageVisual::Property::DESIRED_HEIGHT,
  SYNCHRONOUS_LOADING = Dali::Toolkit::ImageVisual::Property::SYNCHRONOUS_LOADING,
  BORDER_ONLY         = Dali::Toolkit::ImageVisual::Property::BORDER_ONLY,
  PIXEL_AREA          = Dali::Toolkit::ImageVisual::Property::PIXEL_AREA,
  WRAP_MODE_U         = Dali::Toolkit::ImageVisual::Property::WRAP_MODE_U,
  WRAP_MODE_V         = Dali::Toolkit::ImageVisual::Property::WRAP_MODE_V,
  BORDER              = Dali::Toolkit::ImageVisual::Property::BORDER,
  ATLASING            = Dali::Toolkit::ImageVisual::Property::ATLASING,
  ALPHA_MASK_URL      = Dali::Toolkit::ImageVisual::Property::ALPHA_MASK_URL,
  BATCH_SIZE          = Dali::Toolkit::ImageVisual::Property::BATCH_SIZE,
  CACHE_SIZE          = Dali::Toolkit::ImageVisual::Property::CACHE_SIZE,
  FRAME_DELAY         = Dali::Toolkit::ImageVisual::Property::FRAME_DELAY,
  MASK_CONTENT_SCALE  = Dali::Toolkit::ImageVisual::Property::MASK_CONTENT_SCALE,
  CROP_TO_MASK        = Dali::Toolkit::ImageVisual::Property::CROP_TO_MASK,

  /**
   * @brief The policy to determine when an image should be loaded.
   * @details Name "loadPolicy",  Type LoadPolicy::Type (Property::INTEGER)or Property::STRING.
   * @note Default LoadPolicy::ATTACHED
   * @see LoadPolicy::Type
   */
  LOAD_POLICY = CROP_TO_MASK + 1,

  /**
   * @brief The policy to determine when an image should no longer be cached.
   * @details Name "releasePolicy", Type ReleasePolicy::Type (Property::INTEGER) or Property::STRING
   * @note Default ReleasePolicy::DESTROYED
   * @see ReleasePolicy::Type
   */
  RELEASE_POLICY = CROP_TO_MASK + 2,

  /**
   * @brief Determines if image orientation should be corrected so the image displays as it was intended.
   * @details Name "orientationCorrection", Type Property::BOOLEAN, if true the image's orientation will be corrected.
   * @note Default true
   */
  ORIENTATION_CORRECTION = CROP_TO_MASK + 3,

  /**
   * @brief Overlays the auxiliary iamge on top of an NPatch image.
   *
   * The resulting visual image will be at least as large as the
   * smallest possible n-patch or the auxiliary image, whichever is
   * larger.
   *
   * @details Name "auxiliaryImage", Type Property::STRING, URL of the image.
   * @note Default true
   */
  AUXILIARY_IMAGE = CROP_TO_MASK + 4,

  /**
   * @brief An alpha value for mixing between the masked main NPatch image and the auxiliary image
   * @details Name "auxiliaryImageAlpha", Type Property::FLOAT, between 0 and 1
   * @note Default 0
   */
  AUXILIARY_IMAGE_ALPHA = CROP_TO_MASK + 5,

};

} //namespace Property

} // namespace DevelImageVisual

} // namespace Toolkit

} // namespace Dali

#endif // DALI_TOOLKIT_DEVEL_API_VISUALS_IMAGE_VISUAL_PROPERTIES_DEVEL_H
