#ifndef DALI_TOOLKIT_INTERNAL_IMAGE_CACHE_H
#define DALI_TOOLKIT_INTERNAL_IMAGE_CACHE_H

/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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
 */

// EXTERNAL INCLUDES
#include <dali-toolkit/internal/visuals/texture-manager-impl.h>
#include <dali-toolkit/internal/visuals/texture-upload-observer.h>

namespace Dali
{
namespace Toolkit
{
namespace Internal
{
class ImageCache : public TextureManager::LifecycleObserver
{
public:
  /**
   * Observer class to inform when the next image is ready
   */
  class FrameReadyObserver
  {
  public:
    /**
     * @brief Informs observer when the next texture set is ready to display
     * @param[in] textureSet The ready texture set
     * @param[in] interval interval for the frame
     */
    virtual void FrameReady(TextureSet textureSet, uint32_t interval) = 0;
  };

  struct UrlStore
  {
    TextureManager::TextureId mTextureId = TextureManager::INVALID_TEXTURE_ID;
    std::string               mUrl;
  };

  /**
   * List of URLs
   */
  typedef std::vector<UrlStore> UrlList;

public:
  /**
   * @brief Constructor.
   * @param[in] textureManager The texture manager
   * @param[in] size           The width and height to fit the loaded image to.
   * @param[in] samplingMode   The SamplingMode of the resource to load
   * @param[in] observer       FrameReady observer
   * @param[in] maskingData    Masking data to be applied.
   * @param[in] batchSize      The size of a batch to load
   * @param[in] interval       Time interval between each frame
   *
   * This will start loading textures immediately, according to the
   * batch and cache sizes. The cache is as large as the number of urls.
   */
  ImageCache(TextureManager&                     textureManager,
             ImageDimensions                     size,
             Dali::SamplingMode::Type            samplingMode,
             TextureManager::MaskingDataPointer& maskingData,
             ImageCache::FrameReadyObserver&     observer,
             uint32_t                            batchSize,
             uint32_t                            interval);

  virtual ~ImageCache();

  /**
   * @brief Get the first frame. If it's not ready, this will trigger the
   * sending of FrameReady() when the image becomes ready.
   *
   * @return TextureSet of the first frame.
   */
  virtual TextureSet FirstFrame() = 0;

  /**
   * @brief Get the Nth frame. If it's not ready, this will trigger the
   * sending of FrameReady() when the image becomes ready.
   *
   * @param[in] frameIndex required frame index to be returned.
   * @return TextureSet of the frame index.
   */
  virtual TextureSet Frame(uint32_t frameIndex) = 0;

  /**
   * @brief Get the interval of Nth frame.
   *
   * @param[in] frameIndex frame index to get frame interval.
   * @return Time interval in millisecond between frames of frameIndex and frameIndex + 1.
   */
  virtual uint32_t GetFrameInterval(uint32_t frameIndex) const = 0;

  /**
   * @brief Get the current rendered frame index.
   * If there isn't any loaded frame, returns -1.
   *
   * @return Frame index of currently showing frame.
   */
  virtual int32_t GetCurrentFrameIndex() const = 0;

  /**
   * @brief Get total frame count of the animated image file.
   *
   * @return Total frame count of the animated image file.
   */
  virtual int32_t GetTotalFrameCount() const = 0;

  /**
   * @brief Get Loading state of the animated image file.
   *
   * @return LoadState of currently requested frame.
   */
  virtual TextureManager::LoadState GetLoadState() const = 0;

  /**
   * @brief Clears animated image cache and remove loaded textures.
   */
  virtual void ClearCache() = 0;

  /**
   * @brief Set default interval between each frame.
   *
   * @param[in] interval time interval in millisecond to be used as default interval.
   */
  virtual void SetInterval(uint32_t interval);

private:
  /**
   * @brief Called before the texture manager is destroyed.
   */
  void TextureManagerDestroyed() final;

  void AllocateMaskData();

protected:
  TextureManager&                     mTextureManager;
  FrameReadyObserver&                 mObserver;
  TextureManager::MaskingDataPointer& mMaskingData;
  Dali::ImageDimensions               mDesiredSize;
  Dali::SamplingMode::Type            mSamplingMode : 4;
  uint32_t                            mBatchSize;
  uint32_t                            mInterval;
  bool                                mRequestingLoad : 1;
  bool                                mTextureManagerAlive : 1;
};

} //namespace Internal
} //namespace Toolkit
} //namespace Dali

#endif // DALI_TOOLKIT_INTERNAL_IMAGE_CACHE_H
