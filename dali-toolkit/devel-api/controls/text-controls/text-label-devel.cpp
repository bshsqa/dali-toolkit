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

// INTERNAL INCLUDES
#include <dali-toolkit/devel-api/controls/text-controls/text-label-devel.h>
#include <dali-toolkit/internal/controls/text-controls/text-label-impl.h>

namespace Dali
{
namespace Toolkit
{
namespace DevelTextLabel
{
AnchorClickedSignalType& AnchorClickedSignal(TextLabel textLabel)
{
  return GetImpl(textLabel).AnchorClickedSignal();
}

TextFitChangedSignalType& TextFitChangedSignal(TextLabel textLabel)
{
  return GetImpl(textLabel).TextFitChangedSignal();
}

Vector<Vector2> GetTextSize(TextLabel textLabel, const uint32_t startIndex, const uint32_t endIndex)
{
  return GetImpl(textLabel).GetTextSize(startIndex, endIndex);
}

Vector<Vector2> GetTextPosition(TextLabel textLabel, const uint32_t startIndex, const uint32_t endIndex)
{
  return GetImpl(textLabel).GetTextPosition(startIndex, endIndex);
}

} // namespace DevelTextLabel

} // namespace Toolkit

} // namespace Dali
