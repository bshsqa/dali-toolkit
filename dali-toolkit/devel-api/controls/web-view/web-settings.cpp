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
#include <dali-toolkit/devel-api/controls/web-view/web-settings.h>

// EXTERNAL INCLUDES
#include <dali/devel-api/adaptor-framework/web-engine-settings.h>

namespace Dali
{
namespace Toolkit
{
WebSettings::WebSettings(Dali::WebEngineSettings& settings)
: mWebEngineSettings(settings)
{
}

WebSettings::~WebSettings()
{
}

void WebSettings::AllowMixedContents(bool allowed)
{
  mWebEngineSettings.AllowMixedContents(allowed);
}

void WebSettings::EnableSpatialNavigation(bool enabled)
{
  mWebEngineSettings.AllowMixedContents(enabled);
}

int WebSettings::GetDefaultFontSize() const
{
  return mWebEngineSettings.GetDefaultFontSize();
}

void WebSettings::SetDefaultFontSize(int defaultFontSize)
{
  mWebEngineSettings.SetDefaultFontSize(defaultFontSize);
}

void WebSettings::EnableWebSecurity(bool enabled)
{
  mWebEngineSettings.EnableWebSecurity(enabled);
}

void WebSettings::AllowFileAccessFromExternalUrl(bool allowed)
{
  mWebEngineSettings.AllowFileAccessFromExternalUrl(allowed);
}

bool WebSettings::IsJavaScriptEnabled() const
{
  return mWebEngineSettings.IsJavaScriptEnabled();
}

void WebSettings::EnableJavaScript(bool enabled)
{
  mWebEngineSettings.EnableJavaScript(enabled);
}

void WebSettings::AllowScriptsOpenWindows(bool allowed)
{
  mWebEngineSettings.AllowScriptsOpenWindows(allowed);
}

bool WebSettings::AreImagesLoadedAutomatically() const
{
  return mWebEngineSettings.AreImagesLoadedAutomatically();
}

void WebSettings::AllowImagesLoadAutomatically(bool automatic)
{
  mWebEngineSettings.AllowImagesLoadAutomatically(automatic);
}

std::string WebSettings::GetDefaultTextEncodingName() const
{
  return mWebEngineSettings.GetDefaultTextEncodingName();
}

void WebSettings::SetDefaultTextEncodingName(const std::string& defaultTextEncodingName)
{
  mWebEngineSettings.SetDefaultTextEncodingName(defaultTextEncodingName);
}

} // namespace Toolkit

} // namespace Dali
