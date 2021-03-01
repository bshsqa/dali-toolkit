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

#include "toolkit-timer.h"

#include <dali/devel-api/adaptor-framework/web-engine.h>
#include <dali/devel-api/adaptor-framework/web-engine-back-forward-list.h>
#include <dali/devel-api/adaptor-framework/web-engine-back-forward-list-item.h>
#include <dali/devel-api/adaptor-framework/web-engine-certificate.h>
#include <dali/devel-api/adaptor-framework/web-engine-console-message.h>
#include <dali/devel-api/adaptor-framework/web-engine-context.h>
#include <dali/devel-api/adaptor-framework/web-engine-context-menu.h>
#include <dali/devel-api/adaptor-framework/web-engine-context-menu-item.h>
#include <dali/devel-api/adaptor-framework/web-engine-cookie-manager.h>
#include <dali/devel-api/adaptor-framework/web-engine-form-repost-decision.h>
#include <dali/devel-api/adaptor-framework/web-engine-frame.h>
#include <dali/devel-api/adaptor-framework/web-engine-hit-test.h>
#include <dali/devel-api/adaptor-framework/web-engine-http-auth-handler.h>
#include <dali/devel-api/adaptor-framework/web-engine-load-error.h>
#include <dali/devel-api/adaptor-framework/web-engine-policy-decision.h>
#include <dali/devel-api/adaptor-framework/web-engine-request-interceptor.h>
#include <dali/devel-api/adaptor-framework/web-engine-security-origin.h>
#include <dali/devel-api/adaptor-framework/web-engine-settings.h>
#include <dali/public-api/adaptor-framework/native-image-source.h>
#include <dali/public-api/images/pixel-data.h>
#include <dali/public-api/object/any.h>
#include <dali/public-api/object/base-object.h>
#include <dali/public-api/object/property-map.h>
#include <memory>
#include <string.h>
#include <toolkit-application.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class WebEngine;
class MockWebEngineContext;

namespace
{

// Generally only one WebEngine instance exists.
// If > 1, a new web engine has been created by CreateWindowSignal.
static WebEngine* gInstance = nullptr;
static int gInstanceCount = 0;
static MockWebEngineContext* gWebEngineContextInstance = nullptr;

bool OnGoBack();
bool OnGoForward();
bool OnLoadUrl();
bool OnEvaluteJavaScript();
bool OnJavaScriptAlert();
bool OnJavaScriptConfirm();
bool OnJavaScriptPrompt();
bool OnHitTestCreated();
bool OnScrollEdge();
bool OnScreenshotCaptured();
bool OnVideoPlaying();
bool OnGeolocationPermission();
bool OnClearHistory();
bool OnSecurityOriginAcquired();
bool OnStorageUsageAcquired();
bool OnFormPasswordAcquired();
bool OnDownloadStarted();
bool OnMimeOverridden();

static void ConnectToGlobalSignal( bool ( *func )() )
{
  Dali::Timer timer = Dali::Timer::New( 0 );
  timer.TickSignal().Connect( func );
}

static void DisconnectFromGlobalSignal( bool ( *func )() )
{
  Dali::Timer timer = Dali::Timer::New( 0 );
  timer.TickSignal().Disconnect( func );
}
} // namespace anonymous

class MockWebEngineContext : public Dali::WebEngineContext
{
public:
  MockWebEngineContext()
    : mockModel( Dali::WebEngineContext::CacheModel::DOCUMENT_VIEWER )
  {
  }

  Dali::WebEngineContext::CacheModel GetCacheModel() const override
  {
    return mockModel;
  }

  void SetCacheModel( Dali::WebEngineContext::CacheModel cacheModel ) override
  {
    mockModel = cacheModel;
  }

  void SetProxyUri( const std::string& uri ) override
  {
  }

  void SetDefaultProxyAuth( const std::string& username, const std::string& password ) override
  {
  }

  void SetCertificateFilePath( const std::string& certificatePath ) override
  {
  }

  void DeleteAllWebDatabase() override
  {
  }

  bool GetWebDatabaseOrigins(Dali::WebEngineContext::WebEngineSecurityOriginAcquiredCallback callback)
  {
    if (callback)
    {
      ConnectToGlobalSignal(&OnSecurityOriginAcquired);
      mSecurityOriginAcquiredCallback = callback;
    }
    return true;
  }

  bool DeleteWebDatabase(Dali::WebEngineSecurityOrigin& origin)
  {
    return true;
  }

  bool GetWebStorageOrigins(Dali::WebEngineContext::WebEngineSecurityOriginAcquiredCallback callback)
  {
    if (callback)
    {
      ConnectToGlobalSignal(&OnSecurityOriginAcquired);
      mSecurityOriginAcquiredCallback = callback;
    }
    return true;
  }

  bool GetWebStorageUsageForOrigin(Dali::WebEngineSecurityOrigin& origin, Dali::WebEngineContext::WebEngineStorageUsageAcquiredCallback callback) 
  {
    if (callback)
    {
      ConnectToGlobalSignal(&OnStorageUsageAcquired);
      mStorageUsageAcquiredCallback = callback;
    }
    return true;
  }

  void DeleteAllWebStorage() override
  {
  }

  bool DeleteWebStorageOrigin(Dali::WebEngineSecurityOrigin& origin)
  {
    return true;
  }

  void DeleteLocalFileSystem() override
  {
  }

  void DisableCache( bool cacheDisabled ) override
  {
  }

  void ClearCache() override
  {
  }

  bool DeleteApplicationCache(Dali::WebEngineSecurityOrigin& origin)
  {
    return true;
  }

  void GetFormPasswordList(Dali::WebEngineContext::WebEngineFormPasswordAcquiredCallback callback)
  {
    if (callback)
    {
      ConnectToGlobalSignal(&OnFormPasswordAcquired);
      mFormPasswordAcquiredCallback = callback;
    }
  }

  void RegisterDownloadStartedCallback(Dali::WebEngineContext::WebEngineDownloadStartedCallback callback)
  {
    if (callback)
    {
      ConnectToGlobalSignal(&OnDownloadStarted);
      mDownloadStartedCallback = callback;
    }
  }

  void RegisterMimeOverriddenCallback(Dali::WebEngineContext::WebEngineMimeOverriddenCallback callback)
  {
    if (callback)
    {
      ConnectToGlobalSignal(&OnMimeOverridden);
      mMimeOverriddenCallback = callback;
    }
  }

public:
  Dali::WebEngineContext::WebEngineSecurityOriginAcquiredCallback mSecurityOriginAcquiredCallback;
  Dali::WebEngineContext::WebEngineStorageUsageAcquiredCallback mStorageUsageAcquiredCallback;
  Dali::WebEngineContext::WebEngineFormPasswordAcquiredCallback mFormPasswordAcquiredCallback;
  Dali::WebEngineContext::WebEngineDownloadStartedCallback mDownloadStartedCallback;
  Dali::WebEngineContext::WebEngineMimeOverriddenCallback mMimeOverriddenCallback;

private:
  Dali::WebEngineContext::CacheModel mockModel;
};

class MockWebEngineCookieManager : public Dali::WebEngineCookieManager
{
public:
  MockWebEngineCookieManager()
    : mockCookieAcceptPolicy( Dali::WebEngineCookieManager::CookieAcceptPolicy::NO_THIRD_PARTY )
  {
  }

  void SetCookieAcceptPolicy( Dali::WebEngineCookieManager::CookieAcceptPolicy policy ) override
  {
    mockCookieAcceptPolicy = policy;
  }

  Dali::WebEngineCookieManager::CookieAcceptPolicy GetCookieAcceptPolicy() const override
  {
    return mockCookieAcceptPolicy;
  }

  void ClearCookies() override
  {
  }

  void SetPersistentStorage( const std::string& path, Dali::WebEngineCookieManager::CookiePersistentStorage storage ) override
  {
  }

private:
  Dali::WebEngineCookieManager::CookieAcceptPolicy mockCookieAcceptPolicy;
};

class MockWebEngineBackForwardListItem : public Dali::WebEngineBackForwardListItem
{
public:
  MockWebEngineBackForwardListItem()
    : mockUrl( "http://url" ),
      mockTitle( "title" ),
      mockOriginalUrl( "http://originalurl" )
  {
  }

  std::string GetUrl() const override
  {
    return mockUrl;
  }

  std::string GetTitle() const override
  {
    return mockTitle;
  }

  std::string GetOriginalUrl() const override
  {
    return mockOriginalUrl;
  }

private:
  std::string mockUrl;
  std::string mockTitle;
  std::string mockOriginalUrl;
};

class MockWebEngineBackForwardList : public Dali::WebEngineBackForwardList
{
public:
  MockWebEngineBackForwardList()
  {
  }

  std::unique_ptr<Dali::WebEngineBackForwardListItem> GetCurrentItem() const override
  {
    std::unique_ptr<Dali::WebEngineBackForwardListItem> ret(new MockWebEngineBackForwardListItem());
    return ret;
  }

  std::unique_ptr<Dali::WebEngineBackForwardListItem> GetPreviousItem() const override
  {
    std::unique_ptr<Dali::WebEngineBackForwardListItem> ret(new MockWebEngineBackForwardListItem());
    return ret;
  }

  std::unique_ptr<Dali::WebEngineBackForwardListItem> GetNextItem() const override
  {
    std::unique_ptr<Dali::WebEngineBackForwardListItem> ret(new MockWebEngineBackForwardListItem());
    return ret;
  }

  std::unique_ptr<Dali::WebEngineBackForwardListItem> GetItemAtIndex( uint32_t index ) const override
  {
    std::unique_ptr<Dali::WebEngineBackForwardListItem> ret(new MockWebEngineBackForwardListItem());
    return ret;
  }

  uint32_t GetItemCount() const override
  {
    return 1;
  }

  std::vector<std::unique_ptr<Dali::WebEngineBackForwardListItem>> GetBackwardItems(int limit) override
  {
    std::vector<std::unique_ptr<Dali::WebEngineBackForwardListItem>> ret;
    std::unique_ptr<Dali::WebEngineBackForwardListItem> item(new MockWebEngineBackForwardListItem());
    ret.push_back(std::move(item));
    return ret;
  }

  std::vector<std::unique_ptr<Dali::WebEngineBackForwardListItem>> GetForwardItems(int limit) override
  {
    std::vector<std::unique_ptr<Dali::WebEngineBackForwardListItem>> ret;
    std::unique_ptr<Dali::WebEngineBackForwardListItem> item(new MockWebEngineBackForwardListItem());
    ret.push_back(std::move(item));
    return ret;
  }
};


class MockWebEngineCertificate : public Dali::WebEngineCertificate
{
public:
  MockWebEngineCertificate()
  {
  }

  void Allow(bool allowed) override
  {
  }

  bool IsFromMainFrame() const override
  {
    return true;
  }

  std::string GetPem() const override
  {
    return "abc";
  }

  bool IsContextSecure() const override
  {
    return true;
  }
};

class MockWebEngineHttpAuthHandler : public Dali::WebEngineHttpAuthHandler
{
public:
  MockWebEngineHttpAuthHandler()
  {
  }

  std::string GetRealm() const override
  {
    return "test";
  }

  void Suspend() override
  {
  }

  void UseCredential(const std::string& user, const std::string& password) override
  {
  }

  void CancelCredential() override
  {
  }
};

class MockWebEngineFormRepostDecision : public WebEngineFormRepostDecision
{
public:
  MockWebEngineFormRepostDecision()
  {
  }

  void Reply(bool allowed) override {}
};

class MockWebEngineFrame : public Dali::WebEngineFrame
{
public:
  MockWebEngineFrame()
  {
  }

  bool IsMainFrame() const override
  {
    return true;
  }
};

class MockWebEnginePolicyDecision : public Dali::WebEnginePolicyDecision
{
public:
  MockWebEnginePolicyDecision()
  {
  }

  std::string GetUrl() const override
  {
    return "http://test.html";
  }

  std::string GetCookie() const override
  {
    return "test:abc";
  }

  Dali::WebEnginePolicyDecision::DecisionType GetDecisionType() const
  {
    return Dali::WebEnginePolicyDecision::DecisionType::USE;
  }

  std::string GetResponseMime() const
  {
    return "txt/xml";
  }

  int32_t GetResponseStatusCode() const
  {
    return 500;
  }

  Dali::WebEnginePolicyDecision::NavigationType GetNavigationType() const
  {
    return Dali::WebEnginePolicyDecision::NavigationType::LINK_CLICKED;
  }

  Dali::WebEngineFrame& GetFrame() const
  {
    return *(Dali::WebEngineFrame*)(&mockWebFrame);
  }

  std::string GetScheme() const
  {
    return "test";
  }

  bool Use()
  {
    return true;
  }

  bool Ignore()
  {
    return true;
  }

  bool Suspend()
  {
    return true;
  }

private:
  MockWebEngineFrame mockWebFrame;
};

class MockWebEngineRequestInterceptor : public WebEngineRequestInterceptor
{
public:
  MockWebEngineRequestInterceptor()
  {
  }

  std::string GetUrl() const override
  {
    return "http://test.html";
  }

  bool Ignore() override
  {
    return true;
  }

  bool SetResponseStatus(int statusCode, const std::string &customedStatusText) override
  {
    return true;
  }

  bool AddResponseHeader(const std::string &fieldName, const std::string &fieldValue) override
  {
    return true;
  }

  bool AddResponseBody(const std::string &body, uint32_t length) override
  {
    return true;
  }
};

class MockWebEngineConsoleMessage : public Dali::WebEngineConsoleMessage
{
public:
  MockWebEngineConsoleMessage()
  {
  }

  std::string GetSource() const override
  {
    return "source";
  }

  uint32_t GetLine() const override
  {
    return 10;
  }

  SeverityLevel GetSeverityLevel() const override
  {
    return SeverityLevel::EMPTY;
  }

  std::string GetText() const override
  {
    return "This is a text.";
  }
};

class MockWebEngineLoadError : public Dali::WebEngineLoadError
{
public:
  MockWebEngineLoadError(const std::string& url)
    : mockUrl(url)
  {
  }

  std::string GetUrl() const override
  {
    return mockUrl;
  }

  ErrorCode GetCode() const override
  {
    return ErrorCode::UNKNOWN;
  }

  std::string GetDescription() const override
  {
    return "This is an error.";
  }

  ErrorType GetType() const override
  {
    return ErrorType::NONE;
  }

private:
  std::string mockUrl;
};

class MockWebEngineContextMenuItem : public Dali::WebEngineContextMenuItem
{
public:
  MockWebEngineContextMenuItem()
  {
  }

  ItemTag GetTag() const override
  {
    return ItemTag::NO_ACTION;
  }

  ItemType GetType() const override
  {
    return ItemType::ACTION;
  }

  bool IsEnabled() const override
  {
    return true;
  }

  std::string GetLinkUrl() const override
  {
    return "http://test.html";
  }

  std::string GetImageUrl() const override
  {
    return "http://test.jpg";
  }

  std::string GetTitle() const override
  {
    return "title";
  }

  std::unique_ptr<Dali::WebEngineContextMenu> GetParentMenu() const override
  {
    std::unique_ptr<Dali::WebEngineContextMenu> result;
    return result;
  }
};

class MockWebEngineContextMenu : public Dali::WebEngineContextMenu
{
public:
  MockWebEngineContextMenu()
  {
  }

  uint32_t GetItemCount() const override
  {
    return 1;
  }

  std::unique_ptr<Dali::WebEngineContextMenuItem> GetItemAt(uint32_t index) const override
  {
    std::unique_ptr<Dali::WebEngineContextMenuItem> webitem(new MockWebEngineContextMenuItem());
    return webitem;
  }

  std::vector<std::unique_ptr<WebEngineContextMenuItem>> GetItemList() const override
  {
    std::vector<std::unique_ptr<WebEngineContextMenuItem>> result;
    std::unique_ptr<Dali::WebEngineContextMenuItem> webitem(new MockWebEngineContextMenuItem());
    result.push_back(std::move(webitem));
    return result;
  }

  Dali::Vector2 GetPosition() const override
  {
    return Dali::Vector2(100, 100);
  }

  bool RemoveItem(WebEngineContextMenuItem& item) override
  {
    return true;
  }

  bool AppendItemAsAction(WebEngineContextMenuItem::ItemTag tag, const std::string& title, bool enabled) override
  {
    return true;
  }

  bool AppendItem(WebEngineContextMenuItem::ItemTag tag, const std::string& title, const std::string& iconFile, bool enabled) override
  {
    return true;
  }

  bool SelectItem(WebEngineContextMenuItem& item) override
  {
    return true;
  }

  bool Hide() override
  {
    return true;
  }
};

class MockWebEngineHitTest : public Dali::WebEngineHitTest
{
public:
  MockWebEngineHitTest()
  {
  }

  ResultContext GetResultContext() const override
  {
    return ResultContext::DOCUMENT;
  }

  std::string GetLinkUri() const override
  {
    return "http://test.html";
  }

  std::string GetLinkTitle() const override
  {
    return "test";
  }

  std::string GetLinkLabel() const override
  {
    return "label";
  }

  std::string GetImageUri() const override
  {
    return "http://test.jpg";
  }

  std::string GetMediaUri() const override
  {
    return "http://test.mp4";
  }

  std::string GetTagName() const override
  {
    return "img";
  }

  std::string GetNodeValue() const override
  {
    return "test";
  }

  Dali::Property::Map& GetAttributes() const override
  {
    return mockAttributesMap;
  }

  std::string GetImageFileNameExtension() const override
  {
    return "jpg";
  }

  Dali::PixelData GetImageBuffer() override
  {
    uint8_t* faviconData = new uint8_t[ 16 ];
    memset(faviconData, 0xff, 16);
    return Dali::PixelData::New( faviconData, 16, 2, 2,
                                 Dali::Pixel::Format::RGBA8888,
                                 Dali::PixelData::ReleaseFunction::DELETE_ARRAY );
  }

private:
  mutable Dali::Property::Map mockAttributesMap;
};

class MockWebEngineSecurityOrigin : public Dali::WebEngineSecurityOrigin
{
public:
  MockWebEngineSecurityOrigin()
    : mockUrl("https://test.html")
    , mockPotocol("https")
  {
  }

  std::string GetHost() const
  {
    return mockUrl;
  }

  std::string GetProtocol() const
  {
    return mockPotocol;
  }

private:
  std::string mockUrl;
  std::string mockPotocol;
};

class MockWebEngineSettings : public WebEngineSettings
{
public:
  MockWebEngineSettings()
    : mockDefaultFontSize( 16 ),
      mockJavaScriptEnabled( true ),
      mockAutoFittingEnabled ( true ),
      mockPluginsEnabled ( true ),
      mockPrivateBrowsingEnabled( true ),
      mockLinkMagnifierEnabled( true ),
      mockKeypadWithoutUserActionUsed( true ),
      mockAutofillPasswordFormEnabled( true ),
      mockFormCandidateDataEnabled( true ),
      mockTextSelectionEnabled( true ),
      mockTextAutosizingEnable( true ),
      mockArrowScrollEnable( true ),
      mockClipboardEnabled( true ),
      mockImePanelEnabled( true ),
      mockImageLoadedAutomatically( true ),
      mockDefaultTextEncodingName()
  {
  }

  uint32_t GetDefaultFontSize() const override
  {
    return mockDefaultFontSize;
  }

  void SetDefaultFontSize( uint32_t size ) override
  {
    mockDefaultFontSize = size;
  }

  bool IsJavaScriptEnabled() const override
  {
    return mockJavaScriptEnabled;
  }

  void EnableJavaScript( bool enabled ) override
  {
    mockJavaScriptEnabled = enabled;
  }

  bool IsAutoFittingEnabled() const override
  {
    return mockAutoFittingEnabled;
  }

  void EnableAutoFitting( bool enabled ) override
  {
    mockAutoFittingEnabled = enabled;
  }

  bool ArePluginsEnabled() const override
  {
    return mockPluginsEnabled;
  }

  void EnablePlugins( bool enabled ) override
  {
    mockPluginsEnabled = enabled;
  }

  bool IsPrivateBrowsingEnabled() const override
  {
    return mockPrivateBrowsingEnabled;
  }

  void EnablePrivateBrowsing( bool enabled ) override
  {
    mockPrivateBrowsingEnabled = enabled;
  }

  bool IsLinkMagnifierEnabled() const override
  {
    return mockLinkMagnifierEnabled;
  }

  void EnableLinkMagnifier( bool enabled ) override
  {
    mockLinkMagnifierEnabled = enabled;
  }

  bool IsKeypadWithoutUserActionUsed() const override
  {
    return mockKeypadWithoutUserActionUsed;
  }

  void UseKeypadWithoutUserAction( bool used ) override
  {
    mockKeypadWithoutUserActionUsed = used;
  }

  bool IsAutofillPasswordFormEnabled() const override
  {
    return mockAutofillPasswordFormEnabled;
  }

  void EnableAutofillPasswordForm( bool enabled ) override
  {
    mockAutofillPasswordFormEnabled = enabled;
  }

  bool IsFormCandidateDataEnabled() const override
  {
    return mockFormCandidateDataEnabled;
  }

  void EnableFormCandidateData( bool enabled ) override
  {
    mockFormCandidateDataEnabled = enabled;
  }

  bool IsTextSelectionEnabled() const override
  {
    return mockTextSelectionEnabled;
  }

  void EnableTextSelection( bool enabled ) override
  {
    mockTextSelectionEnabled = enabled;
  }

  bool IsTextAutosizingEnabled() const override
  {
    return mockTextAutosizingEnable;
  }

  void EnableTextAutosizing( bool enabled ) override
  {
    mockTextAutosizingEnable = enabled;
  }

  bool IsArrowScrollEnabled() const override
  {
    return mockArrowScrollEnable;
  }

  void EnableArrowScroll( bool enabled ) override
  {
    mockArrowScrollEnable = enabled;
  }

  bool IsClipboardEnabled() const override
  {
    return mockClipboardEnabled;
  }

  void EnableClipboard( bool enabled ) override
  {
    mockClipboardEnabled = enabled;
  }

  bool IsImePanelEnabled() const override
  {
    return mockImePanelEnabled;
  }

  void EnableImePanel( bool enabled ) override
  {
    mockImePanelEnabled = enabled;
  }

  bool AreImagesLoadedAutomatically() const override
  {
    return mockImageLoadedAutomatically;
  }

  void AllowImagesLoadAutomatically( bool automatic ) override
  {
    mockImageLoadedAutomatically = automatic;
  }

  std::string GetDefaultTextEncodingName() const override
  {
    return mockDefaultTextEncodingName;
  }

  void SetDefaultTextEncodingName( const std::string& defaultTextEncodingName ) override
  {
    mockDefaultTextEncodingName = defaultTextEncodingName;
  }

  void AllowMixedContents( bool allowed ) override
  {
  }

  void EnableSpatialNavigation( bool enabled ) override
  {
  }

  void EnableWebSecurity( bool enabled ) override
  {
  }

  void EnableCacheBuilder( bool enabled ) override
  {
  }

  void UseScrollbarThumbFocusNotifications( bool used ) override
  {
  }

  void EnableDoNotTrack( bool enabled ) override
  {
  }

  void AllowFileAccessFromExternalUrl( bool allowed ) override
  {
  }

  void AllowScriptsOpenWindows( bool allowed ) override
  {
  }

private:
  int mockDefaultFontSize;
  bool mockJavaScriptEnabled;
  bool mockAutoFittingEnabled;
  bool mockPluginsEnabled;
  bool mockPrivateBrowsingEnabled;
  bool mockLinkMagnifierEnabled;
  bool mockKeypadWithoutUserActionUsed;
  bool mockAutofillPasswordFormEnabled;
  bool mockFormCandidateDataEnabled;
  bool mockTextSelectionEnabled;
  bool mockTextAutosizingEnable;
  bool mockArrowScrollEnable;
  bool mockClipboardEnabled;
  bool mockImePanelEnabled;
  bool mockImageLoadedAutomatically;
  std::string mockDefaultTextEncodingName;
};

class WebEngine: public Dali::BaseObject
{
public:

  using JavaScriptEvaluatedResultCallback = std::function<void(const std::string&)>;

  WebEngine()
    : mUrl()
    , mCurrentPlusOnePos( 0 )
    , mUserAgent()
    , mEvaluating( false )
    , mScrollPosition( 0, 0 )
    , mScrollSize( 500, 500 )
    , mContentSize( 500, 500 )
  {
    gInstanceCount++;
    if ( gInstanceCount == 1 ) // only first web engine need be saved.
    {
      gInstance = this;
    }

    mockWebEngineSettings = new MockWebEngineSettings();
    MockWebEngineContext* engineContext = new MockWebEngineContext();
    mockWebEngineContext = engineContext;
    if ( gInstanceCount == 1 )
    {
      gWebEngineContextInstance = engineContext;
    }
    mockWebEngineCookieManager = new MockWebEngineCookieManager();
    mockWebEngineBackForwardList = new MockWebEngineBackForwardList();
  }

  virtual ~WebEngine()
  {
    gInstanceCount--;
    if( !gInstanceCount )
    {
      gInstance = 0;
      gWebEngineContextInstance = 0;
    }

    delete mockWebEngineSettings;
    delete mockWebEngineContext;
    delete mockWebEngineCookieManager;
    delete mockWebEngineBackForwardList;
  }

  Dali::WebEngineSettings& GetSettings() const
  {
    return *mockWebEngineSettings;
  }

  Dali::WebEngineContext& GetContext() const
  {
    return *mockWebEngineContext;
  }

  Dali::WebEngineCookieManager& GetCookieManager() const
  {
    return *mockWebEngineCookieManager;
  }

  Dali::WebEngineBackForwardList& GetBackForwardList() const
  {
    return *mockWebEngineBackForwardList;
  }

  void LoadUrl( const std::string& url )
  {
    mUrl = url;
    ConnectToGlobalSignal( &OnLoadUrl );
  }

  const std::string& GetUrl() const
  {
    return mUrl;
  }

  std::string GetTitle() const
  {
    return std::string("title");
  }

  Dali::PixelData GetFavicon() const
  {
    uint8_t* faviconData = new uint8_t[ 16 ];

    faviconData[ 0 ] = 0xff;
    faviconData[ 1 ] = 0x00;
    faviconData[ 2 ] = 0x00;
    faviconData[ 3 ] = 0xff;
    faviconData[ 4 ] = 0xff;
    faviconData[ 5 ] = 0x00;
    faviconData[ 6 ] = 0x00;
    faviconData[ 7 ] = 0xff;
    faviconData[ 8 ] = 0xff;
    faviconData[ 9 ] = 0x00;
    faviconData[ 10 ] = 0x00;
    faviconData[ 11 ] = 0xff;
    faviconData[ 12 ] = 0xff;
    faviconData[ 13 ] = 0x00;
    faviconData[ 14 ] = 0x00;
    faviconData[ 15 ] = 0xff;

    return Dali::PixelData::New( faviconData, 16, 2, 2,
                                 Dali::Pixel::Format::RGBA8888,
                                 Dali::PixelData::ReleaseFunction::DELETE_ARRAY );
  }

  bool CanGoForward() const
  {
    return mHistory.size() > mCurrentPlusOnePos;
  }

  void GoForward()
  {
    ConnectToGlobalSignal( &OnGoForward );
  }

  bool CanGoBack() const
  {
    return mCurrentPlusOnePos > 1;
  }

  void GoBack()
  {
    ConnectToGlobalSignal( &OnGoBack );
  }

  void EvaluateJavaScript( const std::string& script, std::function< void( const std::string& ) > resultHandler )
  {
    if( resultHandler )
    {
      if( !mEvaluating )
      {
        ConnectToGlobalSignal( &OnEvaluteJavaScript );
      }
      mResultCallbacks.push_back( resultHandler );
    }
  }

  void RegisterJavaScriptAlertCallback( Dali::WebEnginePlugin::JavaScriptAlertCallback callback )
  {
    if ( callback )
    {
      ConnectToGlobalSignal( &OnJavaScriptAlert );
      mJavaScriptAlertCallback = callback;
    }
  }

  void RegisterJavaScriptConfirmCallback( Dali::WebEnginePlugin::JavaScriptConfirmCallback callback )
  {
    if ( callback )
    {
      ConnectToGlobalSignal( &OnJavaScriptConfirm );
      mJavaScriptConfirmCallback = callback;
    }
  }

  void RegisterJavaScriptPromptCallback( Dali::WebEnginePlugin::JavaScriptPromptCallback callback )
  {
    if ( callback )
    {
      ConnectToGlobalSignal( &OnJavaScriptPrompt );
      mJavaScriptPromptCallback = callback;
    }
  }

  std::unique_ptr<Dali::WebEngineHitTest> CreateHitTest(int32_t x, int32_t y, Dali::WebEngineHitTest::HitTestMode mode)
  {
    std::unique_ptr<Dali::WebEngineHitTest> hitTest(new MockWebEngineHitTest());
    return hitTest;
  }

  bool CreateHitTestAsynchronously(int32_t x, int32_t y, Dali::WebEngineHitTest::HitTestMode mode, Dali::WebEnginePlugin::WebEngineHitTestCreatedCallback callback)
  {
    if (callback)
    {
      ConnectToGlobalSignal( &OnHitTestCreated );
      mHitTestCreatedCallback = callback;
    }
    return true;
  }

  void ClearHistory()
  {
    ConnectToGlobalSignal( &OnClearHistory );
  }

  const std::string& GetUserAgent() const
  {
    return mUserAgent;
  }

  void SetUserAgent( const std::string& userAgent )
  {
    mUserAgent = userAgent;
  }

  void ScrollBy( int32_t dx, int32_t dy )
  {
    mScrollPosition += Dali::Vector2( dx, dy );
    if ( mScrollPosition.y + mScrollSize.height > mContentSize.height )
    {
      ConnectToGlobalSignal( &OnScrollEdge );
    }
  }

  bool ScrollEdgeBy( int32_t dx, int32_t dy )
  {
    mScrollPosition += Dali::Vector2( dx, dy );
    if ( mScrollPosition.y + mScrollSize.height > mContentSize.height )
    {
      ConnectToGlobalSignal( &OnScrollEdge );
    }
    return true;
  }

  void SetScrollPosition( int32_t x, int32_t y )
  {
    mScrollPosition.x = x;
    mScrollPosition.y = y;
  }

  Dali::Vector2 GetScrollPosition() const
  {
    return mScrollPosition;
  }

  Dali::Vector2 GetScrollSize() const
  {
    return mScrollSize;
  }

  Dali::Vector2 GetContentSize() const
  {
    return mContentSize;
  }

  void SetPageZoomFactor(float zoomFactor)
  {
    mPageZoomFactor = zoomFactor;
  }

  float GetPageZoomFactor() const
  {
    return mPageZoomFactor;
  }

  void SetTextZoomFactor(float zoomFactor)
  {
    mTextZoomFactor = zoomFactor;
  }

  float GetTextZoomFactor() const
  {
    return mTextZoomFactor;
  }

  float GetLoadProgressPercentage() const
  {
    return 0.5f;
  }

  void SetScaleFactor(float scaleFactor, Dali::Vector2 point)
  {
    mScaleFactor = scaleFactor;
  }

  float GetScaleFactor() const
  {
    return mScaleFactor;
  }

  Dali::PixelData GetScreenshot(Dali::Rect<int32_t> viewArea, float scaleFactor)
  {
    uint32_t bufferSize = viewArea.width * viewArea.height * 4 ;
    uint8_t* pixel = new uint8_t[ bufferSize ];
    memset(pixel, 0xff, bufferSize);
    return Dali::PixelData::New( pixel, bufferSize, viewArea.width, viewArea.height,
                                 Dali::Pixel::Format::RGBA8888,
                                 Dali::PixelData::ReleaseFunction::DELETE_ARRAY );
  }

  bool GetScreenshotAsynchronously(Dali::Rect<int32_t> viewArea, float scaleFactor, Dali::WebEnginePlugin::ScreenshotCapturedCallback callback)
  {
    if ( callback )
    {
      ConnectToGlobalSignal( &OnScreenshotCaptured );
      mScreenshotCapturedCallback = callback;
    }
    return true;
  }

  bool CheckVideoPlayingAsynchronously(Dali::WebEnginePlugin::VideoPlayingCallback callback)
  {
    if ( callback )
    {
      ConnectToGlobalSignal( &OnVideoPlaying );
      mVideoPlayingCallback = callback;
    }
    return true;
  }

  void RegisterGeolocationPermissionCallback(Dali::WebEnginePlugin::GeolocationPermissionCallback callback)
  {
    if ( callback )
    {
      ConnectToGlobalSignal( &OnGeolocationPermission );
      mGeolocationPermissionCallback = callback;
    }
  }

  Dali::WebEnginePlugin::WebEnginePageLoadSignalType& PageLoadStartedSignal()
  {
    return mPageLoadStartedSignal;
  }

  Dali::WebEnginePlugin::WebEnginePageLoadSignalType& PageLoadInProgressSignal()
  {
    return mPageLoadInProgressSignal;
  }

  Dali::WebEnginePlugin::WebEnginePageLoadSignalType& PageLoadFinishedSignal()
  {
    return mPageLoadFinishedSignal;
  }

  Dali::WebEnginePlugin::WebEnginePageLoadErrorSignalType& PageLoadErrorSignal()
  {
    return mPageLoadErrorSignal;
  }

  Dali::WebEnginePlugin::WebEngineScrollEdgeReachedSignalType& ScrollEdgeReachedSignal()
  {
    return mScrollEdgeReachedSignal;
  }

  Dali::WebEnginePlugin::WebEngineUrlChangedSignalType& UrlChangedSignal()
  {
    return mUrlChangedSignal;
  }

  Dali::WebEnginePlugin::WebEngineFormRepostDecisionSignalType& FormRepostDecisionSignal()
  {
    return mFormRepostDecisionSignal;
  }

  Dali::WebEnginePlugin::WebEngineFrameRenderedSignalType& FrameRenderedSignal()
  {
    return mFrameRenderedSignal;
  }

  Dali::WebEnginePlugin::WebEngineRequestInterceptorSignalType& RequestInterceptorSignal()
  {
    return mRequestInterceptorSignal;
  }

  Dali::WebEnginePlugin::WebEngineConsoleMessageSignalType& ConsoleMessageSignal()
  {
    return mConsoleMessageSignal;
  }

  Dali::WebEnginePlugin::WebEnginePolicyDecisionSignalType& PolicyDecisionSignal()
  {
    return mPolicyDecisionSignal;
  }

  Dali::WebEnginePlugin::WebEngineCertificateSignalType& CertificateConfirmSignal()
  {
    return mCertificateConfirmSignal;
  }

  Dali::WebEnginePlugin::WebEngineCertificateSignalType& SslCertificateChangedSignal()
  {
    return mSslCertificateChangedSignal;
  }

  Dali::WebEnginePlugin::WebEngineHttpAuthHandlerSignalType& HttpAuthHandlerSignal()
  {
    return mHttpAuthHandlerSignal;
  }

  Dali::WebEnginePlugin::WebEngineContextMenuCustomizedSignalType& ContextMenuCustomizedSignal()
  {
    return mContextMenuCustomizedSignal;
  }

  Dali::WebEnginePlugin::WebEngineContextMenuItemSelectedSignalType& ContextMenuItemSelectedSignal()
  {
    return mContextMenuItemSelectedSignal;
  }

  std::string              mUrl;
  std::vector<std::string> mHistory;
  size_t                   mCurrentPlusOnePos;
  std::string              mUserAgent;

  Dali::WebEnginePlugin::WebEnginePageLoadSignalType                mPageLoadStartedSignal;
  Dali::WebEnginePlugin::WebEnginePageLoadSignalType                mPageLoadInProgressSignal;
  Dali::WebEnginePlugin::WebEnginePageLoadSignalType                mPageLoadFinishedSignal;
  Dali::WebEnginePlugin::WebEnginePageLoadErrorSignalType           mPageLoadErrorSignal;
  Dali::WebEnginePlugin::WebEngineScrollEdgeReachedSignalType       mScrollEdgeReachedSignal;
  Dali::WebEnginePlugin::WebEngineUrlChangedSignalType              mUrlChangedSignal;
  Dali::WebEnginePlugin::WebEngineFormRepostDecisionSignalType      mFormRepostDecisionSignal;
  Dali::WebEnginePlugin::WebEngineFrameRenderedSignalType           mFrameRenderedSignal;
  Dali::WebEnginePlugin::WebEngineRequestInterceptorSignalType      mRequestInterceptorSignal;
  Dali::WebEnginePlugin::WebEngineConsoleMessageSignalType          mConsoleMessageSignal;
  Dali::WebEnginePlugin::WebEnginePolicyDecisionSignalType          mPolicyDecisionSignal;
  Dali::WebEnginePlugin::WebEngineCertificateSignalType             mCertificateConfirmSignal;
  Dali::WebEnginePlugin::WebEngineCertificateSignalType             mSslCertificateChangedSignal;
  Dali::WebEnginePlugin::WebEngineHttpAuthHandlerSignalType         mHttpAuthHandlerSignal;
  Dali::WebEnginePlugin::WebEngineContextMenuCustomizedSignalType   mContextMenuCustomizedSignal;
  Dali::WebEnginePlugin::WebEngineContextMenuItemSelectedSignalType mContextMenuItemSelectedSignal;

  bool  mEvaluating;
  float mPageZoomFactor;
  float mTextZoomFactor;
  float mScaleFactor;

  Dali::Vector2             mScrollPosition;
  Dali::Vector2             mScrollSize;
  Dali::Vector2             mContentSize;
  WebEngineBackForwardList* mockWebEngineBackForwardList;
  WebEngineContext*         mockWebEngineContext;
  WebEngineCookieManager*   mockWebEngineCookieManager;
  WebEngineSettings*        mockWebEngineSettings;

  std::vector<JavaScriptEvaluatedResultCallback>         mResultCallbacks;
  Dali::WebEnginePlugin::JavaScriptAlertCallback         mJavaScriptAlertCallback;
  Dali::WebEnginePlugin::JavaScriptConfirmCallback       mJavaScriptConfirmCallback;
  Dali::WebEnginePlugin::JavaScriptPromptCallback        mJavaScriptPromptCallback;
  Dali::WebEnginePlugin::ScreenshotCapturedCallback      mScreenshotCapturedCallback;
  Dali::WebEnginePlugin::VideoPlayingCallback            mVideoPlayingCallback;
  Dali::WebEnginePlugin::GeolocationPermissionCallback   mGeolocationPermissionCallback;
  Dali::WebEnginePlugin::WebEngineHitTestCreatedCallback mHitTestCreatedCallback;
};


namespace
{

bool OnGoBack()
{
  DisconnectFromGlobalSignal( &OnGoBack );

  if( gInstance && gInstance->CanGoBack() )
  {
    gInstance->mCurrentPlusOnePos--;
  }
  return false;
}

bool OnGoForward()
{
  DisconnectFromGlobalSignal( &OnGoForward );

  if( gInstance && gInstance->CanGoForward() )
  {
    gInstance->mCurrentPlusOnePos++;
  }
  return false;
}

bool OnLoadUrl()
{
  DisconnectFromGlobalSignal( &OnLoadUrl );

  if( gInstance )
  {
    if( gInstance->mHistory.size() > gInstance->mCurrentPlusOnePos )
    {
      gInstance->mHistory.erase( gInstance->mHistory.begin() + gInstance->mCurrentPlusOnePos, gInstance->mHistory.end() );
    }
    gInstance->mHistory.push_back( gInstance->mUrl );
    gInstance->mCurrentPlusOnePos++;
    gInstance->mPageLoadStartedSignal.Emit( gInstance->mUrl );
    gInstance->mPageLoadInProgressSignal.Emit( gInstance->mUrl );
    gInstance->mPageLoadFinishedSignal.Emit( gInstance->mUrl );
    gInstance->mUrlChangedSignal.Emit( "http://new-test" );

    std::shared_ptr<Dali::WebEngineFormRepostDecision> repostDecision(new MockWebEngineFormRepostDecision());
    gInstance->mFormRepostDecisionSignal.Emit(std::move(repostDecision));
    gInstance->mFrameRenderedSignal.Emit();
    std::shared_ptr<Dali::WebEngineRequestInterceptor> interceptor(new MockWebEngineRequestInterceptor());
    gInstance->mRequestInterceptorSignal.Emit(std::move(interceptor));

    std::shared_ptr<Dali::WebEngineLoadError> error(new MockWebEngineLoadError(gInstance->mUrl));
    gInstance->mPageLoadErrorSignal.Emit(std::move(error));
    std::shared_ptr<Dali::WebEngineConsoleMessage> message(new MockWebEngineConsoleMessage());
    gInstance->mConsoleMessageSignal.Emit(std::move(message));
    std::shared_ptr<Dali::WebEnginePolicyDecision> policyDecision(new MockWebEnginePolicyDecision());
    gInstance->mPolicyDecisionSignal.Emit(std::move(policyDecision));

    std::shared_ptr<Dali::WebEngineCertificate> certificate(new MockWebEngineCertificate());
    gInstance->mCertificateConfirmSignal.Emit(std::move(certificate));
    std::shared_ptr<Dali::WebEngineCertificate> sslCertificate(new MockWebEngineCertificate());
    gInstance->mSslCertificateChangedSignal.Emit(std::move(sslCertificate));
    std::shared_ptr<Dali::WebEngineHttpAuthHandler> handler(new MockWebEngineHttpAuthHandler());
    gInstance->mHttpAuthHandlerSignal.Emit(std::move(handler));

    std::shared_ptr<Dali::WebEngineContextMenu> menu(new MockWebEngineContextMenu());
    gInstance->mContextMenuCustomizedSignal.Emit(std::move(menu));
    std::shared_ptr<Dali::WebEngineContextMenuItem> item(new MockWebEngineContextMenuItem());
    gInstance->mContextMenuItemSelectedSignal.Emit(std::move(item));
  }
  return false;
}

bool OnScrollEdge()
{
  DisconnectFromGlobalSignal( &OnScrollEdge );

  if( gInstance )
  {
    gInstance->mScrollEdgeReachedSignal.Emit( Dali::WebEnginePlugin::ScrollEdge::BOTTOM );
  }

  return false;
}

bool OnEvaluteJavaScript()
{
  DisconnectFromGlobalSignal( &OnEvaluteJavaScript );

  if( gInstance )
  {
    for( auto& func : gInstance->mResultCallbacks )
    {
      func("undefined");
    }
    gInstance->mResultCallbacks.clear();
  }
  return false;
}

bool OnJavaScriptAlert()
{
  DisconnectFromGlobalSignal( &OnJavaScriptAlert );
  if ( gInstance )
  {
    gInstance->mJavaScriptAlertCallback( "this is an alert popup." );
  }
  return false;
}

bool OnJavaScriptConfirm()
{
  DisconnectFromGlobalSignal( &OnJavaScriptConfirm );
  if ( gInstance )
  {
    gInstance->mJavaScriptConfirmCallback( "this is a confirm popup." );
  }
  return false;
}

bool OnJavaScriptPrompt()
{
  DisconnectFromGlobalSignal( &OnJavaScriptPrompt );
  if ( gInstance )
  {
    gInstance->mJavaScriptPromptCallback( "this is a prompt pompt.", "" );
  }
  return false;
}

bool OnScreenshotCaptured()
{
  DisconnectFromGlobalSignal( &OnScreenshotCaptured );
  if ( gInstance )
  {
    uint8_t* pixel = new uint8_t[ 2 * 2 * 4 ];
    memset(pixel, 0xff, 2 * 2 * 4);
    Dali::PixelData data = Dali::PixelData::New( pixel, 2 * 2 * 4, 2, 2,
                                 Dali::Pixel::Format::RGBA8888,
                                 Dali::PixelData::ReleaseFunction::DELETE_ARRAY );
    gInstance->mScreenshotCapturedCallback( data );
  }
  return false;
}

bool OnVideoPlaying()
{
  DisconnectFromGlobalSignal( &OnVideoPlaying );
  if ( gInstance )
  {
    gInstance->mVideoPlayingCallback( true );
  }
  return false;
}

bool OnGeolocationPermission()
{
  DisconnectFromGlobalSignal( &OnGeolocationPermission );
  if ( gInstance )
  {
    gInstance->mGeolocationPermissionCallback( "", "" );
  }
  return false;
}

bool OnHitTestCreated()
{
  DisconnectFromGlobalSignal(&OnHitTestCreated);
  if (gInstance)
  {
    std::unique_ptr<Dali::WebEngineHitTest> test(new MockWebEngineHitTest());
    gInstance->mHitTestCreatedCallback(std::move(test));
  }
  return false;
}

bool OnClearHistory()
{
  DisconnectFromGlobalSignal( &OnClearHistory );

  if( gInstance && gInstance->mCurrentPlusOnePos )
  {
    std::string url = gInstance->mHistory[ gInstance->mCurrentPlusOnePos - 1 ];
    std::vector< std::string >().swap( gInstance->mHistory );
    gInstance->mHistory.push_back( url );
    gInstance->mCurrentPlusOnePos = 1;
  }
  return false;
}

bool OnSecurityOriginAcquired()
{
  DisconnectFromGlobalSignal(&OnSecurityOriginAcquired);
  if (gWebEngineContextInstance)
  {
    std::vector<std::unique_ptr<Dali::WebEngineSecurityOrigin>> securityOriginList;
    std::unique_ptr<Dali::WebEngineSecurityOrigin> origin(new MockWebEngineSecurityOrigin());
    securityOriginList.push_back(std::move(origin));
    gWebEngineContextInstance->mSecurityOriginAcquiredCallback(securityOriginList);
  }
  return false;
}

bool OnStorageUsageAcquired()
{
  DisconnectFromGlobalSignal(&OnStorageUsageAcquired);
  if (gWebEngineContextInstance)
  {
    gWebEngineContextInstance->mStorageUsageAcquiredCallback(0);
  }
  return false;
}

bool OnFormPasswordAcquired()
{
  DisconnectFromGlobalSignal(&OnFormPasswordAcquired);
  if (gWebEngineContextInstance)
  {
    std::vector<std::unique_ptr<Dali::WebEngineContext::PasswordData>> formPasswordList;
    std::unique_ptr<Dali::WebEngineContext::PasswordData> data(new Dali::WebEngineContext::PasswordData());
    data->url = "http://test.html";
    data->useFingerprint = false;
    formPasswordList.push_back(std::move(data));
    gWebEngineContextInstance->mFormPasswordAcquiredCallback(formPasswordList);
  }
  return false;
}

bool OnDownloadStarted()
{
  DisconnectFromGlobalSignal(&OnDownloadStarted);
  if (gWebEngineContextInstance)
  {
    gWebEngineContextInstance->mDownloadStartedCallback("http://test.html");
  }
  return false;
}

bool OnMimeOverridden()
{
  DisconnectFromGlobalSignal(&OnMimeOverridden);
  if (gWebEngineContextInstance)
  {
    std::string newMime;
    gWebEngineContextInstance->mMimeOverriddenCallback("http://test.html", "txt/xml", newMime);
  }
  return false;
}

} // namespace

inline WebEngine& GetImplementation( Dali::WebEngine& webEngine )
{
  DALI_ASSERT_ALWAYS( webEngine && "WebEngine handle is empty." );
  BaseObject& handle = webEngine.GetBaseObject();
  return static_cast< Internal::Adaptor::WebEngine& >( handle );
}

inline const WebEngine& GetImplementation( const Dali::WebEngine& webEngine )
{
  DALI_ASSERT_ALWAYS( webEngine && "WebEngine handle is empty." );
  const BaseObject& handle = webEngine.GetBaseObject();
  return static_cast< const Internal::Adaptor::WebEngine& >( handle );
}

} // namespace Adaptor

} // namespace Internal

// Dali::WebEngine Implementation
WebEngine::WebEngine()
{
}

WebEngine::WebEngine( Internal::Adaptor::WebEngine* internal )
: BaseHandle( internal )
{
}

WebEngine::~WebEngine()
{
}

WebEngine WebEngine::New()
{
  Internal::Adaptor::WebEngine* baseObject = new Internal::Adaptor::WebEngine();

  return WebEngine( baseObject );
}

WebEngine::WebEngine( const WebEngine& WebEngine )
: BaseHandle( WebEngine )
{
}

WebEngine& WebEngine::operator=( const WebEngine& webEngine )
{
  BaseHandle::operator=( webEngine );
  return *this;
}

WebEngine WebEngine::DownCast( BaseHandle handle )
{
  return WebEngine( dynamic_cast< Internal::Adaptor::WebEngine* >( handle.GetObjectPtr() ) );
}

void WebEngine::Create( uint32_t width, uint32_t height, const std::string& locale, const std::string& timezoneId )
{
}

void WebEngine::Create( uint32_t width, uint32_t height, uint32_t argc, char** argv )
{
}

void WebEngine::Destroy()
{
}

WebEngineSettings& WebEngine::GetSettings() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetSettings();
}

WebEngineContext& WebEngine::GetContext() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetContext();
}

WebEngineCookieManager& WebEngine::GetCookieManager() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetCookieManager();
}

WebEngineBackForwardList& WebEngine::GetBackForwardList() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetBackForwardList();
}

void WebEngine::LoadUrl( const std::string& url )
{
  return Internal::Adaptor::GetImplementation( *this ).LoadUrl( url );
}

std::string WebEngine::GetTitle() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetTitle();
}

Dali::PixelData WebEngine::GetFavicon() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetFavicon();
}

const std::string& WebEngine::GetUrl()
{
  return Internal::Adaptor::GetImplementation( *this ).GetUrl();
}

NativeImageInterfacePtr WebEngine::GetNativeImageSource()
{
  Any source;
  Dali::NativeImageSourcePtr sourcePtr = Dali::NativeImageSource::New( source );
  return sourcePtr;
}

void WebEngine::LoadHtmlString( const std::string& htmlString )
{
}

bool WebEngine::LoadHtmlStringOverrideCurrentEntry(const std::string& html, const std::string& basicUri, const std::string& unreachableUrl)
{
  return true;
}

bool WebEngine::LoadContents(const std::string& contents, uint32_t contentSize, const std::string& mimeType, const std::string& encoding, const std::string& baseUri)
{
  return true;
}

void WebEngine::Reload()
{
}

bool WebEngine::ReloadWithoutCache()
{
  return true;
}

void WebEngine::StopLoading()
{
}

void WebEngine::Suspend()
{
}

void WebEngine::Resume()
{
}

void WebEngine::SuspendNetworkLoading()
{
}

void WebEngine::ResumeNetworkLoading()
{
}

bool WebEngine::AddCustomHeader(const std::string& name, const std::string& value)
{
  return true;
}

bool WebEngine::RemoveCustomHeader(const std::string& name)
{
  return true;
}

uint32_t WebEngine::StartInspectorServer(uint32_t port)
{
  return port;
}

bool WebEngine::StopInspectorServer()
{
  return true;
}

bool WebEngine::CanGoForward()
{
  return Internal::Adaptor::GetImplementation( *this ).CanGoForward();
}

void WebEngine::GoForward()
{
  Internal::Adaptor::GetImplementation( *this ).GoForward();
}

bool WebEngine::CanGoBack()
{
  return Internal::Adaptor::GetImplementation( *this ).CanGoBack();
}

void WebEngine::GoBack()
{
  Internal::Adaptor::GetImplementation( *this ).GoBack();
}

void WebEngine::EvaluateJavaScript( const std::string& script, std::function< void( const std::string& ) > resultHandler )
{
  Internal::Adaptor::GetImplementation( *this ).EvaluateJavaScript( script, resultHandler );
}

void WebEngine::AddJavaScriptMessageHandler( const std::string& exposedObjectName, std::function< void(const std::string&) > handler )
{
}

void WebEngine::RegisterJavaScriptAlertCallback( Dali::WebEnginePlugin::JavaScriptAlertCallback callback )
{
  Internal::Adaptor::GetImplementation( *this ).RegisterJavaScriptAlertCallback( callback );
}

void WebEngine::JavaScriptAlertReply()
{
}

void WebEngine::RegisterJavaScriptConfirmCallback( Dali::WebEnginePlugin::JavaScriptConfirmCallback callback )
{
  Internal::Adaptor::GetImplementation( *this ).RegisterJavaScriptConfirmCallback( callback );
}

void WebEngine::JavaScriptConfirmReply( bool confirmed )
{
}

void WebEngine::RegisterJavaScriptPromptCallback( Dali::WebEnginePlugin::JavaScriptPromptCallback callback )
{
  Internal::Adaptor::GetImplementation( *this ).RegisterJavaScriptPromptCallback( callback );
}

void WebEngine::JavaScriptPromptReply( const std::string& result )
{
}

std::unique_ptr<Dali::WebEngineHitTest> WebEngine::CreateHitTest(int32_t x, int32_t y, Dali::WebEngineHitTest::HitTestMode mode)
{
  return Internal::Adaptor::GetImplementation(*this).CreateHitTest(x, y, mode);
}

bool WebEngine::CreateHitTestAsynchronously(int32_t x, int32_t y, Dali::WebEngineHitTest::HitTestMode mode, Dali::WebEnginePlugin::WebEngineHitTestCreatedCallback callback)
{
  return Internal::Adaptor::GetImplementation(*this).CreateHitTestAsynchronously(x, y, mode, callback);
}

void WebEngine::ClearAllTilesResources()
{
}

void WebEngine::ClearHistory()
{
  Internal::Adaptor::GetImplementation( *this ).ClearHistory();
}

void WebEngine::SetScaleFactor(float scaleFactor, Dali::Vector2 point)
{
  Internal::Adaptor::GetImplementation( *this ).SetScaleFactor(scaleFactor, point);
}

float WebEngine::GetScaleFactor() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetScaleFactor();
}

void WebEngine::ActivateAccessibility(bool activated)
{
}

bool WebEngine::HighlightText(const std::string& text, Dali::WebEnginePlugin::FindOption options, uint32_t maxMatchCount)
{
  return true;
}

void WebEngine::AddDynamicCertificatePath(const std::string& host, const std::string& certPath)
{
}

Dali::PixelData WebEngine::GetScreenshot(Dali::Rect<int32_t> viewArea, float scaleFactor)
{
  return Internal::Adaptor::GetImplementation( *this ).GetScreenshot(viewArea, scaleFactor);
}

bool WebEngine::GetScreenshotAsynchronously(Dali::Rect<int32_t> viewArea, float scaleFactor, Dali::WebEnginePlugin::ScreenshotCapturedCallback callback)
{
  return Internal::Adaptor::GetImplementation( *this ).GetScreenshotAsynchronously(viewArea, scaleFactor, callback);
}

bool WebEngine::CheckVideoPlayingAsynchronously(Dali::WebEnginePlugin::VideoPlayingCallback callback)
{
  return Internal::Adaptor::GetImplementation( *this ).CheckVideoPlayingAsynchronously(callback);
}

void WebEngine::RegisterGeolocationPermissionCallback(Dali::WebEnginePlugin::GeolocationPermissionCallback callback)
{
  Internal::Adaptor::GetImplementation( *this ).RegisterGeolocationPermissionCallback(callback);
}

const std::string& WebEngine::GetUserAgent() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetUserAgent();
}

void WebEngine::SetUserAgent( const std::string& userAgent )
{
  Internal::Adaptor::GetImplementation( *this ).SetUserAgent( userAgent );
}

void WebEngine::ScrollBy( int32_t dx, int32_t dy )
{
  Internal::Adaptor::GetImplementation( *this ).ScrollBy( dx, dy );
}

bool WebEngine::ScrollEdgeBy( int32_t dx, int32_t dy )
{
  return Internal::Adaptor::GetImplementation( *this ).ScrollEdgeBy( dx, dy );
}

void WebEngine::SetScrollPosition( int32_t x, int32_t y )
{
  Internal::Adaptor::GetImplementation( *this ).SetScrollPosition( x, y );
}

Dali::Vector2 WebEngine::GetScrollPosition() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetScrollPosition();
}

Dali::Vector2 WebEngine::GetScrollSize() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetScrollSize();
}

Dali::Vector2 WebEngine::GetContentSize() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetContentSize();
}

void WebEngine::SetSize( uint32_t width, uint32_t height )
{
}

void WebEngine::SetDocumentBackgroundColor(Dali::Vector4 color)
{
}

void WebEngine::ClearTilesWhenHidden(bool cleared)
{
}

void WebEngine::SetTileCoverAreaMultiplier(float multiplier)
{
}

void WebEngine::EnableCursorByClient(bool enabled)
{
}

std::string WebEngine::GetSelectedText() const
{
  return "test";
}

bool WebEngine::SendTouchEvent( const TouchEvent& touch )
{
  return true;
}

bool WebEngine::SendKeyEvent( const KeyEvent& event )
{
  return true;
}

bool WebEngine::SendHoverEvent( const HoverEvent& event )
{
  return true;
}

bool WebEngine::SendWheelEvent( const WheelEvent& event )
{
  return true;
}

void WebEngine::SetFocus( bool focused )
{
}

void WebEngine::SetPageZoomFactor(float zoomFactor)
{
  Internal::Adaptor::GetImplementation( *this ).SetPageZoomFactor(zoomFactor);
}

float WebEngine::GetPageZoomFactor() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetPageZoomFactor();
}

void WebEngine::SetTextZoomFactor(float zoomFactor)
{
  Internal::Adaptor::GetImplementation( *this ).SetTextZoomFactor(zoomFactor);
}

float WebEngine::GetTextZoomFactor() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetTextZoomFactor();
}

float WebEngine::GetLoadProgressPercentage() const
{
  return Internal::Adaptor::GetImplementation( *this ).GetLoadProgressPercentage();
}

void WebEngine::UpdateDisplayArea( Dali::Rect< int32_t > displayArea )
{
}

void WebEngine::EnableVideoHole( bool enabled )
{
}

void WebEngine::EnableMouseEvents( bool enabled )
{
}

void WebEngine::EnableKeyEvents( bool enabled )
{
}

Dali::WebEnginePlugin::WebEnginePageLoadSignalType& WebEngine::PageLoadStartedSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).PageLoadStartedSignal();
}

Dali::WebEnginePlugin::WebEnginePageLoadSignalType& WebEngine::PageLoadInProgressSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).PageLoadInProgressSignal();
}

Dali::WebEnginePlugin::WebEnginePageLoadSignalType& WebEngine::PageLoadFinishedSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).PageLoadFinishedSignal();
}

Dali::WebEnginePlugin::WebEnginePageLoadErrorSignalType& WebEngine::PageLoadErrorSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).PageLoadErrorSignal();
}

Dali::WebEnginePlugin::WebEngineScrollEdgeReachedSignalType& WebEngine::ScrollEdgeReachedSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).ScrollEdgeReachedSignal();
}

Dali::WebEnginePlugin::WebEngineUrlChangedSignalType& WebEngine::UrlChangedSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).UrlChangedSignal();
}

Dali::WebEnginePlugin::WebEngineFormRepostDecisionSignalType& WebEngine::FormRepostDecisionSignal()
{
  return Internal::Adaptor::GetImplementation(*this).FormRepostDecisionSignal();
}

Dali::WebEnginePlugin::WebEngineFrameRenderedSignalType& WebEngine::FrameRenderedSignal()
{
  return Internal::Adaptor::GetImplementation(*this).FrameRenderedSignal();
}

Dali::WebEnginePlugin::WebEngineRequestInterceptorSignalType& WebEngine::RequestInterceptorSignal()
{
  return Internal::Adaptor::GetImplementation(*this).RequestInterceptorSignal();
}

Dali::WebEnginePlugin::WebEngineConsoleMessageSignalType& WebEngine::ConsoleMessageSignal()
{
  return Internal::Adaptor::GetImplementation(*this).ConsoleMessageSignal();
}

Dali::WebEnginePlugin::WebEnginePolicyDecisionSignalType& WebEngine::PolicyDecisionSignal()
{
  return Internal::Adaptor::GetImplementation(*this).PolicyDecisionSignal();
}

Dali::WebEnginePlugin::WebEngineCertificateSignalType& WebEngine::CertificateConfirmSignal()
{
  return Internal::Adaptor::GetImplementation(*this).CertificateConfirmSignal();
}

Dali::WebEnginePlugin::WebEngineCertificateSignalType& WebEngine::SslCertificateChangedSignal()
{
  return Internal::Adaptor::GetImplementation(*this).SslCertificateChangedSignal();
}

Dali::WebEnginePlugin::WebEngineHttpAuthHandlerSignalType& WebEngine::HttpAuthHandlerSignal()
{
  return Internal::Adaptor::GetImplementation(*this).HttpAuthHandlerSignal();
}

Dali::WebEnginePlugin::WebEngineContextMenuCustomizedSignalType& WebEngine::ContextMenuCustomizedSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).ContextMenuCustomizedSignal();
}

Dali::WebEnginePlugin::WebEngineContextMenuItemSelectedSignalType& WebEngine::ContextMenuItemSelectedSignal()
{
  return Internal::Adaptor::GetImplementation( *this ).ContextMenuItemSelectedSignal();
}

} // namespace Dali;

