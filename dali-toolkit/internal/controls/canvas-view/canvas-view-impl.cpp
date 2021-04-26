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

// CLASS HEADER
#include "canvas-view-impl.h"

// EXTERNAL INCLUDES
#include <dali/devel-api/scripting/scripting.h>
#include <dali/integration-api/adaptor-framework/adaptor.h>
#include <dali/public-api/object/type-registry-helper.h>
#include <dali/public-api/object/type-registry.h>

// INTERNAL INCLUDES
#include <dali-toolkit/devel-api/controls/canvas-view/canvas-view.h>
#include <dali-toolkit/devel-api/controls/control-devel.h>
#include <dali-toolkit/internal/controls/control/control-data-impl.h>
#include <dali-toolkit/internal/graphics/builtin-shader-extern-gen.h>
#include <dali-toolkit/internal/visuals/visual-factory-cache.h>

namespace Dali
{
namespace Toolkit
{
namespace Internal
{
namespace
{
BaseHandle Create()
{
  return BaseHandle();
}
// Setup properties, signals and actions using the type-registry.
DALI_TYPE_REGISTRATION_BEGIN(Toolkit::CanvasView, Toolkit::Control, Create);
DALI_TYPE_REGISTRATION_END()
} // anonymous namespace

using namespace Dali;

CanvasView::CanvasView(const Vector2& viewBox)
: Control(ControlBehaviour(CONTROL_BEHAVIOUR_DEFAULT)),
  mCanvasRenderer(CanvasRenderer::New(viewBox)),
  mTexture(),
  mChanged(false)
{
}

CanvasView::~CanvasView()
{
  if(Adaptor::IsAvailable())
  {
    Adaptor::Get().UnregisterProcessor(*this);
  }
}

Toolkit::CanvasView CanvasView::New(const Vector2& viewBox)
{
  CanvasView* impl = new CanvasView(viewBox);

  Toolkit::CanvasView handle = Toolkit::CanvasView(*impl);

  // Second-phase init of the implementation
  // This can only be done after the CustomActor connection has been made...
  impl->Initialize();

  return handle;
}

/////////////////////////////////////////////////////////////

void CanvasView::OnInitialize()
{
  // CanvasView can relayout in the OnImageReady, alternative to a signal would be to have a upcall from the Control to CanvasView
  Dali::Toolkit::Control handle(GetOwner());

  DevelControl::SetAccessibilityConstructor(Self(), [](Dali::Actor actor) {
    return std::unique_ptr<Dali::Accessibility::Accessible>(
      new DevelControl::AccessibleImpl(actor, Dali::Accessibility::Role::IMAGE));
  });

  Self().SetProperty(Toolkit::DevelControl::Property::ACCESSIBILITY_HIGHLIGHTABLE, true);

  Adaptor::Get().RegisterProcessor(*this);
}

void CanvasView::OnRelayout(const Vector2& size, RelayoutContainer& container)
{
  if(!mCanvasRenderer ||
     mCanvasRenderer.GetSize() == size ||
     !mCanvasRenderer.SetSize(size))
  {
    return;
  }
  mChanged = true;
}

void CanvasView::OnSizeSet(const Vector3& targetSize)
{
  Control::OnSizeSet(targetSize);

  if(!mCanvasRenderer ||
     mCanvasRenderer.GetSize() == Vector2(targetSize) ||
     !mCanvasRenderer.SetSize(Vector2(targetSize)))
  {
    return;
  }
  mChanged = true;
}

void CanvasView::Process()
{
  if(!mCanvasRenderer)
  {
    return;
  }
  Commit();
}

void CanvasView::Commit()
{
  if(mCanvasRenderer && mCanvasRenderer.Commit())
  {
    Devel::PixelBuffer pixbuf = mCanvasRenderer.GetPixelBuffer();
    auto               width  = pixbuf.GetWidth();
    auto               height = pixbuf.GetHeight();

    Dali::PixelData pixelData = Devel::PixelBuffer::Convert(pixbuf);
    if(!pixelData)
    {
      return;
    }

    if(!mTexture || mChanged)
    {
      mTexture = Texture::New(TextureType::TEXTURE_2D, Dali::Pixel::RGBA8888, width, height);
      mTexture.Upload(pixelData);
      TextureSet textureSet = TextureSet::New();
      textureSet.SetTexture(0, mTexture);
      Geometry geometry = VisualFactoryCache::CreateQuadGeometry();
      Shader   shader   = Shader::New(SHADER_CANVAS_VIEW_VERT, SHADER_CANVAS_VIEW_FRAG);
      Renderer renderer = Renderer::New(geometry, shader);
      renderer.SetTextures(textureSet);
      renderer.SetProperty(Renderer::Property::BLEND_PRE_MULTIPLIED_ALPHA, true);

      Self().AddRenderer(renderer);
      mChanged = false;
    }
    else
    {
      //Update texture
      mTexture.Upload(pixelData);
    }
  }
}

bool CanvasView::AddDrawable(Dali::CanvasRenderer::Drawable& drawable)
{
  if(mCanvasRenderer && mCanvasRenderer.AddDrawable(drawable))
  {
    return true;
  }
  return false;
}
} // namespace Internal
} // namespace Toolkit
} // namespace Dali
