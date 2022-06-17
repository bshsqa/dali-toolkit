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

#include <dali-toolkit/public-api/controls/model-view/model-view.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/controls/model-view/model-view-impl.h>

// EXTERNAL INCLUDES

namespace Dali
{
namespace Toolkit
{
ModelView::ModelView()
{
}

ModelView::ModelView(const ModelView& modelView) = default;

ModelView::ModelView(ModelView&& rhs) = default;

ModelView& ModelView::operator=(const ModelView& modelView) = default;

ModelView& ModelView::operator=(ModelView&& rhs) = default;

ModelView::~ModelView()
{
}

ModelView ModelView::New(const std::string& modelPath, const std::string& resourcePath)
{
  return Internal::ModelView::New(modelPath, resourcePath);
}

ModelView ModelView::DownCast(BaseHandle handle)
{
  return Control::DownCast<ModelView, Internal::ModelView>(handle);
}

ModelView::ModelView(Internal::ModelView& implementation)
: Control(implementation)
{
}

ModelView::ModelView(Dali::Internal::CustomActor* internal)
: Control(internal)
{
  VerifyCustomActorPointer<Internal::ModelView>(internal);
}

void ModelView::PlayAnimation()
{
  return GetImpl(*this).PlayAnimation();
}

void ModelView::SetImageBasedLightSource(const std::string& diffuse, const std::string& specular, float scaleFactor)
{
  return GetImpl(*this).SetImageBasedLightSource(diffuse, specular, scaleFactor);
}

} // namespace Toolkit

} // namespace Dali
