#ifndef DALI_TOOLKIT_TEXT_UNDERLINED_CHARACTER_RUN_H
#define DALI_TOOLKIT_TEXT_UNDERLINED_CHARACTER_RUN_H

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
#include <dali/public-api/math/vector4.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/text/character-run.h>
#include <dali-toolkit/internal/text/underline-style-properties.h>

namespace Dali
{
namespace Toolkit
{
namespace Text
{
/**
 * @brief Run of underlined characters with same properties.
 */
struct UnderlinedCharacterRun
{
  /**
   * Default constructor to set the default values of bitfields
   */
  UnderlinedCharacterRun()
  : characterRun{},
    properties{}
  {
  }

  CharacterRun             characterRun; ///< The initial character index and the number of characters of the run.
  UnderlineStyleProperties properties;   /// The properties of underline style
};

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // DALI_TOOLKIT_TEXT_UNDERLINED_CHARACTER_RUN_H
