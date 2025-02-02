#ifndef DALI_TOOLKIT_INTERNAL_DRAWABLE_VIEW_H
#define DALI_TOOLKIT_INTERNAL_DRAWABLE_VIEW_H

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
 *
 */

// EXTERNAL INCLUDES
#include <dali/devel-api/actors/actor-devel.h>
#include <dali/devel-api/adaptor-framework/native-image-source-queue.h>
#include <dali/public-api/adaptor-framework/window.h>
#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/rendering/shader.h>
#include <dali/public-api/signals/render-callback.h>
#include "gl-view-interface-impl.h"

// INTERNAL INCLUDES
#include <dali-toolkit/internal/controls/gl-view/gl-view-interface-impl.h>
#include <dali-toolkit/public-api/controls/control-impl.h>
#include <dali-toolkit/public-api/controls/gl-view/gl-view.h>

namespace Dali::Toolkit
{
class GlView;

namespace Internal
{
class DrawableView : public Dali::Toolkit::Internal::GlViewImpl
{
protected:
  virtual ~DrawableView();

public:
  /**
   * @copydoc Dali::Toolkit::GlView::New()
   */
  static Dali::Toolkit::GlView New();

  /**
   * Construct a new GlView.
   */
  DrawableView();

  /**
   * @copydoc Dali::Toolkit::GlView::RegisterGlCallbacks()
   */
  void RegisterGlCallbacks(CallbackBase* initCallback, CallbackBase* renderFrameCallback, CallbackBase* terminateCallback) override;

  /**
   * @copydoc Dali::Toolkit::GlView::SetResizeCallback()
   */
  void SetResizeCallback(CallbackBase* resizeCallback) override;

  /**
   * @copydoc Dali::Toolkit::GlView::SetGraphicsConfig()
   */
  bool SetGraphicsConfig(bool depth, bool stencil, int msaa, Dali::Toolkit::GlView::GraphicsApiVersion version) override;

  /**
   * @copydoc Dali::Toolkit::GlView::SetRenderingMode()
   */
  void SetRenderingMode(Dali::Toolkit::GlView::RenderingMode mode) override;

  /**
   * @copydoc Dali::Toolkit::GlView::GetRenderingMode()
   */
  Dali::Toolkit::GlView::RenderingMode GetRenderingMode() const override;

  /**
   * @copydoc Dali::Toolkit::GlView::RenderOnce()
   */
  void RenderOnce();

private: // From Control
  /**
   * @copydoc Toolkit::Control::OnInitialize()
   */
  virtual void OnInitialize() override;

  /**
   * @copydoc Toolkit::Control::OnSceneConnection()
   */
  void OnSceneConnection(int depth) override;

  /**
   * @copydoc Toolkit::Control::OnSceneDisconnection()
   */
  void OnSceneDisconnection() override;

  /**
   * @copydoc Toolkit::Control::OnSizeSet()
   */
  void OnSizeSet(const Vector3& targetSize) override;

private:
  // Undefined copy constructor and assignment operators
  DrawableView(const DrawableView& GlView);
  DrawableView& operator=(const DrawableView& GlView);

  /**
   * Callback when the visibility of the GlView is changed
   */
  void OnControlVisibilityChanged(Dali::Actor actor, bool visible, Dali::DevelActor::VisibilityChange::Type type);

  /**
   * Callback when the visibility of the window is changed
   */
  void OnWindowVisibilityChanged(Dali::Window window, bool visible);

  /**
   * Adds renderer to Actor.
   */
  void AddRenderer();

private:

  bool OnRenderCallback( const RenderCallbackInput& renderCallbackInput );

private:
  Dali::Toolkit::GlView::RenderingMode mRenderingMode;

  bool mDepth;
  bool mStencil;
  int  mMSAA;

  std::unique_ptr<RenderCallback> mRenderCallback;

  /*
   * Used within RenderCallback to handle the current render state
   */
  enum class ViewState
  {
    INIT,
    RENDER,
    TERMINATE
  };

  ViewState mCurrentViewState{ViewState::INIT}; ///< state within RenderCallback

  // These callbacks are stored for GLView API compatibility
  std::unique_ptr<CallbackBase> mOnInitCallback;
  std::unique_ptr<CallbackBase> mOnRenderCallback;
  std::unique_ptr<CallbackBase> mOnTerminateCallback;
  std::unique_ptr<CallbackBase> mOnResizeCallback;

  std::atomic_bool mSurfaceResized{false}; ///< Flag to invoke surface resize callback

  Size mSurfaceSize{}; ///< Surface size
};

} // namespace Internal

} // namespace Dali

#endif // DALI_TOOLKIT_INTERNAL_DRAWABLE_VIEW_H
