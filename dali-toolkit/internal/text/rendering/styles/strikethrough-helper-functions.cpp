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
#include <dali-toolkit/internal/text/rendering/styles/strikethrough-helper-functions.h>

namespace Dali
{
namespace Toolkit
{
namespace Text
{
/// Helper method to fetch the underline metrics for the specified font glyph
void CalcualteStrikethroughHeight(float& currentStrikethroughHeight, float& maxStrikethroughHeight)
{
  //Height of strikethrough represents the thickness of line.

  // Ensure strikethrough will be at least a pixel high
  if(currentStrikethroughHeight < 1.0f)
  {
    currentStrikethroughHeight = 1.0f;
  }
  else
  {
    currentStrikethroughHeight = ceil(currentStrikethroughHeight);
  }

  // The strikethrough height should be the max strikethrough height of all glyphs of the line.
  if(currentStrikethroughHeight > maxStrikethroughHeight)
  {
    maxStrikethroughHeight = currentStrikethroughHeight;
  }
}

} // namespace Text

} // namespace Toolkit

} // namespace Dali