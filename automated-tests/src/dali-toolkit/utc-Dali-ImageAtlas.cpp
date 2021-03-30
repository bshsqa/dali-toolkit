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

#include <stdlib.h>
#include <unistd.h>
#include <dali/dali.h>
#include <dali-toolkit-test-suite-utils.h>
#include <toolkit-event-thread-callback.h>
#include <dali-toolkit/devel-api/image-loader/image-atlas.h>
#include <dali-toolkit/dali-toolkit.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
// resolution: 34*34, pixel format: RGBA8888
static const char* gImage_34_RGBA = TEST_RESOURCE_DIR "/icon-edit.png";
// resolution: 50*50, pixel format: RGBA8888
static const char* gImage_50_RGBA = TEST_RESOURCE_DIR "/icon-delete.png";
// resolution: 128*128, pixel format: RGB888
static const char* gImage_128_RGB = TEST_RESOURCE_DIR "/gallery-small-1.jpg";

// Empty image, for testing broken image loading
static const char* gEmptyImage = TEST_RESOURCE_DIR "/empty.bmp";

const int RENDER_FRAME_INTERVAL = 16; ///< Duration of each frame in ms. (at approx 60FPS)

PixelData CreatePixelData( unsigned int width, unsigned int height )
{
  unsigned int bufferSize = width*height*Pixel::GetBytesPerPixel( Pixel::RGBA8888 );

  unsigned char* buffer= reinterpret_cast<unsigned char*>( malloc( bufferSize ) );
  PixelData pixelData = PixelData::New( buffer, bufferSize, width, height, Pixel::RGBA8888, PixelData::FREE );

  return pixelData;
}

Rect<int> TextureCoordinateToPixelArea( const Vector4& textureCoordinate, float size )
{
  Vector4 temp = textureCoordinate * size;
  Rect<int> pixelArea;
  pixelArea.x = static_cast<int>( temp.x );
  pixelArea.y = static_cast<int>( temp.y );
  pixelArea.width = static_cast<int>( temp.z-temp.x+1.01f );
  pixelArea.height = static_cast<int>( temp.w-temp.y+1.01f );

  return pixelArea;
}

Rect<int> TextureCoordinateToPixelArea( const Vector4& textureCoordinate, float width, float height )
{
  Rect<int> pixelArea;
  pixelArea.x = static_cast<int>( textureCoordinate.x*width );
  pixelArea.y = static_cast<int>( textureCoordinate.y*height);
  pixelArea.width = static_cast<int>( (textureCoordinate.z-textureCoordinate.x)*width+1.01f );
  pixelArea.height = static_cast<int>( (textureCoordinate.w-textureCoordinate.y)*height+1.01f );

  return pixelArea;
}

bool IsOverlap( Rect<int> rect1, Rect<int> rect2 )
{
 return rect1.x < rect2.x+rect2.width
     && rect2.x < rect1.x+rect1.width
     && rect1.y < rect2.y+rect2.height
     && rect2.y < rect1.y+rect1.height;
}

static unsigned int gCountOfTestFuncCall;
class TestUploadObserver : public AtlasUploadObserver
{
public:
  TestUploadObserver()
  {}

  virtual ~TestUploadObserver()
  {}

  void UploadCompleted()
  {
    gCountOfTestFuncCall++;
  }
};

} // anonymous namespace

void dali_image_atlas_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_image_atlas_cleanup(void)
{
  test_return_value = TET_PASS;
}

int UtcDaliImageAtlasNew(void)
{
  ToolkitTestApplication application;

  // invoke default handle constructor
  ImageAtlas atlas;

  DALI_TEST_CHECK( !atlas );

  // initialise handle
  atlas = ImageAtlas::New( 32, 32 );

  DALI_TEST_CHECK( atlas );
  END_TEST;
}

int UtcDaliImageAtlasCopyConstructor(void)
{
  ToolkitTestApplication application;

  ImageAtlas atlas = ImageAtlas::New( 32, 32);
  ImageAtlas atlasCopy(atlas);

  DALI_TEST_EQUALS( (bool)atlasCopy, true, TEST_LOCATION );
  END_TEST;
}

int UtcDaliImageAtlasAssignmentOperator(void)
{
  ToolkitTestApplication application;

  ImageAtlas atlas = ImageAtlas::New( 32, 32 );

  ImageAtlas atlas2;
  DALI_TEST_EQUALS( (bool)atlas2, false, TEST_LOCATION );

  atlas2 = atlas;
  DALI_TEST_EQUALS( (bool)atlas2, true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasGetAtlas(void)
{
  ToolkitTestApplication application;

  ImageAtlas atlas = ImageAtlas::New( 32, 32 );
  Texture image = atlas.GetAtlas();

  // test the atlas created
  DALI_TEST_EQUALS( (bool)image, true, TEST_LOCATION );
  DALI_TEST_CHECK( image.GetHeight() == 32u );
  DALI_TEST_CHECK( image.GetWidth() == 32u );

  END_TEST;
}

int UtcDaliImageAtlasGetOccupancyRate(void)
{
  ToolkitTestApplication application;

  ImageAtlas atlas = ImageAtlas::New( 100, 100 );

  DALI_TEST_EQUALS( atlas.GetOccupancyRate(), 0.f, TEST_LOCATION );

  Vector4 textureRect1;
  atlas.Upload( textureRect1, gImage_34_RGBA, ImageDimensions(34, 34) );
  DALI_TEST_EQUALS( atlas.GetOccupancyRate(), 34.f*34.f/10000.f, 0.001f, TEST_LOCATION );

  Vector4 textureRect2;
  atlas.Upload( textureRect2, gImage_50_RGBA, ImageDimensions(50, 50) );
  DALI_TEST_EQUALS( atlas.GetOccupancyRate(), (34.f*34.f+50.f*50.f)/10000.f, 0.001f, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasSetBrokenImage(void)
{
  ToolkitTestApplication application;
  unsigned int size = 200;
  ImageAtlas atlas = ImageAtlas::New( size, size );

  // Set broken image
  TestPlatformAbstraction& platform = application.GetPlatform();
  platform.SetClosestImageSize(Vector2( 34, 34));
  atlas.SetBrokenImage( gImage_34_RGBA );

  Vector4 textureRect;

  // the empty image will be replaced with the broken image
  platform.SetClosestImageSize(Vector2( 20, 20));
  atlas.Upload( textureRect, gEmptyImage );
  DALI_TEST_EQUALS( Test::WaitForEventThreadTrigger( 1 ), true, TEST_LOCATION );

  Rect<int> pixelArea = TextureCoordinateToPixelArea(textureRect, size);
  DALI_TEST_EQUALS( pixelArea.width, 20, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.height, 20, TEST_LOCATION );

  END_TEST;
}



int UtcDaliImageAtlasUploadP(void)
{
  ToolkitTestApplication application;
  unsigned int size = 200;
  ImageAtlas atlas = ImageAtlas::New( size, size );

  TraceCallStack& callStack = application.GetGlAbstraction().GetTextureTrace();
  callStack.Reset();
  callStack.Enable(true);

  Vector4 textureRect1;
  atlas.Upload( textureRect1, gImage_34_RGBA, ImageDimensions(34, 34) );
  Vector4 textureRect2;
  atlas.Upload( textureRect2, gImage_50_RGBA, ImageDimensions(50, 50) );
  Vector4 textureRect3;
  atlas.Upload( textureRect3, gImage_128_RGB, ImageDimensions(128, 128) );

  DALI_TEST_EQUALS( Test::WaitForEventThreadTrigger( 3 ), true, TEST_LOCATION );

  application.SendNotification();
  application.Render(RENDER_FRAME_INTERVAL);

  callStack.Enable(false);

  Rect<int> pixelArea1 = TextureCoordinateToPixelArea(textureRect1, size);
  DALI_TEST_EQUALS( pixelArea1.width, 34, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea1.height, 34, TEST_LOCATION );

  TraceCallStack::NamedParams params;
  params["width"] <<pixelArea1.width;
  params["height"] <<pixelArea1.height;
  params["xoffset"] <<pixelArea1.x;
  params["yoffset"] <<pixelArea1.y;
  DALI_TEST_CHECK( callStack.FindMethodAndParams("TexSubImage2D", params ));

  Rect<int> pixelArea2 = TextureCoordinateToPixelArea(textureRect2, size);
  DALI_TEST_EQUALS( pixelArea2.width, 50, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea2.height, 50, TEST_LOCATION );

  params.mParams.clear();
  params["width"] <<pixelArea2.width;
  params["height"] <<pixelArea2.height;
  params["xoffset"] <<pixelArea2.x;
  params["yoffset"] <<pixelArea2.y;
  DALI_TEST_CHECK( callStack.FindMethodAndParams("TexSubImage2D", params ) );

  Rect<int> pixelArea3 = TextureCoordinateToPixelArea(textureRect3, size);
  DALI_TEST_EQUALS( pixelArea3.width, 128, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea3.height, 128, TEST_LOCATION );

  params.mParams.clear();
  params["width"] <<pixelArea3.width;
  params["height"] <<pixelArea3.height;
  params["xoffset"] <<pixelArea3.x;
  params["yoffset"] <<pixelArea3.y;
  DALI_TEST_CHECK( callStack.FindMethodAndParams("TexSubImage2D", params ) );

  DALI_TEST_CHECK( ! IsOverlap(pixelArea1, pixelArea2) );
  DALI_TEST_CHECK( ! IsOverlap(pixelArea1, pixelArea3) );
  DALI_TEST_CHECK( ! IsOverlap(pixelArea2, pixelArea3) );

  END_TEST;
}

int UtcDaliImageAtlasUploadWithObserver01(void)
{
  ToolkitTestApplication application;
  ImageAtlas atlas = ImageAtlas::New( 200, 200 );


  gCountOfTestFuncCall = 0;
  TestUploadObserver uploadObserver;

  Vector4 textureRect1;
  atlas.Upload( textureRect1, gImage_34_RGBA, ImageDimensions(34, 34), FittingMode::DEFAULT, true, &uploadObserver );
  Vector4 textureRect2;
  atlas.Upload( textureRect2, gImage_50_RGBA, ImageDimensions(50, 50), FittingMode::DEFAULT, true, NULL );
  Vector4 textureRect3;
  atlas.Upload( textureRect3, gImage_128_RGB, ImageDimensions(128, 128), FittingMode::DEFAULT, true, &uploadObserver );

  // waiting until all three images are loaded and uploaded to atlas
  DALI_TEST_EQUALS( Test::WaitForEventThreadTrigger( 3 ), true, TEST_LOCATION );
  application.SendNotification();
  application.Render(RENDER_FRAME_INTERVAL);

  // Check that TestFunc is called twice
  DALI_TEST_EQUALS( gCountOfTestFuncCall, 2, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasUploadWithObserver02(void)
{
  ToolkitTestApplication application;
  ImageAtlas atlas = ImageAtlas::New( 200, 200 );

  gCountOfTestFuncCall = 0;
  TestUploadObserver* uploadObserver = new TestUploadObserver;

  Vector4 textureRect1;
  atlas.Upload( textureRect1, gImage_34_RGBA, ImageDimensions(34, 34), FittingMode::DEFAULT, true, uploadObserver );
  Vector4 textureRect2;
  atlas.Upload( textureRect2, gImage_50_RGBA, ImageDimensions(50, 50), FittingMode::DEFAULT, true, uploadObserver );
  Vector4 textureRect3;
  atlas.Upload( textureRect3, gImage_128_RGB, ImageDimensions(128, 128), FittingMode::DEFAULT, true, uploadObserver );

  // destroy the object.
  delete uploadObserver;

 // waiting until all three images are loaded and uploaded to atlas
  DALI_TEST_EQUALS( Test::WaitForEventThreadTrigger( 3 ), true, TEST_LOCATION );

  application.Render(RENDER_FRAME_INTERVAL);
  application.SendNotification();

  // Check that TestFunc is called twice
  DALI_TEST_EQUALS( gCountOfTestFuncCall, 0, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasUploadWithObserver03(void)
{
  ToolkitTestApplication application;

  gCountOfTestFuncCall = 0;
  TestUploadObserver* uploadObserver = new TestUploadObserver;

  {
    ImageAtlas atlas = ImageAtlas::New( 200, 200 );

    Vector4 textureRect1;
    atlas.Upload( textureRect1, gImage_34_RGBA, ImageDimensions(34, 34), FittingMode::DEFAULT, true, uploadObserver );
    Vector4 textureRect2;
    atlas.Upload( textureRect2, gImage_50_RGBA, ImageDimensions(50, 50), FittingMode::DEFAULT, true, uploadObserver );
    Vector4 textureRect3;
    atlas.Upload( textureRect3, gImage_128_RGB, ImageDimensions(128, 128), FittingMode::DEFAULT, true, uploadObserver );
  }

  //ImageAtlas is out of scope, so it will get destroyed

  application.Render(RENDER_FRAME_INTERVAL);
  application.SendNotification();
  application.SendNotification();
  application.Render(RENDER_FRAME_INTERVAL);

  // Check that TestFunc is called twice
  DALI_TEST_EQUALS( gCountOfTestFuncCall, 0, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasRemove(void)
{
  ToolkitTestApplication application;
  unsigned int size = 100;
  ImageAtlas atlas = ImageAtlas::New( size, size );
  Vector4 textureRect1;
  atlas.Upload( textureRect1, gImage_34_RGBA, ImageDimensions(34, 34) );

  atlas.Remove( textureRect1 );

  Vector4 textureRect2;
  atlas.Upload( textureRect2, gImage_50_RGBA, ImageDimensions(50, 50) );

  // one pixel gap
  Rect<int> pixelArea = TextureCoordinateToPixelArea(textureRect2, size);
  DALI_TEST_EQUALS( pixelArea.x, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.y, 0, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasImageView(void)
{
  ToolkitTestApplication application;

  TraceCallStack& callStack = application.GetGlAbstraction().GetTextureTrace();
  callStack.Reset();
  callStack.Enable(true);

  Property::Map imageMap1;

  imageMap1[ ImageVisual::Property::URL ] = gImage_34_RGBA;
  imageMap1[ ImageVisual::Property::DESIRED_HEIGHT ] = 34;
  imageMap1[ ImageVisual::Property::DESIRED_WIDTH ] = 34;
  imageMap1[ ImageVisual::Property::ATLASING] = true;

  Property::Map imageMap2;

  imageMap2[ ImageVisual::Property::URL ] = gImage_50_RGBA;
  imageMap2[ ImageVisual::Property::DESIRED_HEIGHT ] = 50;
  imageMap2[ ImageVisual::Property::DESIRED_WIDTH ] = 50;
  imageMap2[ ImageVisual::Property::ATLASING ] = true;

  ImageView imageView1 = ImageView::New();
  imageView1.SetProperty( ImageView::Property::IMAGE, imageMap1 );

  ImageView imageView2 = ImageView::New();
  imageView2.SetProperty( ImageView::Property::IMAGE, imageMap2 );

  // ImageView doesn't do size negotiation properly: it only listens to OnSizeSet:
  imageView1.SetProperty( Actor::Property::SIZE, Vector2( 100, 100 ) );
  imageView2.SetProperty( Actor::Property::SIZE, Vector2( 100, 100 ) );
  imageView1.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS );
  imageView2.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS );

  application.GetPlatform().SetClosestImageSize(  Vector2(34, 34) );
  application.GetScene().Add( imageView1 );
  application.GetPlatform().SetClosestImageSize(  Vector2(50, 50) );
  application.GetScene().Add( imageView2 );

  DALI_TEST_EQUALS( Test::WaitForEventThreadTrigger( 2 ), true, TEST_LOCATION );

  application.SendNotification();
  application.Render(RENDER_FRAME_INTERVAL);

  callStack.Enable(false);

  TraceCallStack::NamedParams params1;
  params1["width"] << 34;
  params1["height"] << 34;
  params1["xoffset"] << 0;
  params1["yoffset"] << 0;

  TraceCallStack::NamedParams params2;
  params2["width"] << 50;
  params2["height"] << 50;
  params2["xoffset"] << 0;
  params2["yoffset"] << 34;

  DALI_TEST_EQUALS(  callStack.FindMethodAndParams("TexSubImage2D", params1 ), true, TEST_LOCATION );
  DALI_TEST_EQUALS(  callStack.FindMethodAndParams("TexSubImage2D", params2 ), true, TEST_LOCATION );

  callStack.Reset();
  callStack.Enable(true);

  // remove the imageView2 from stage, the second image will also be removed from atlas
  // then the space on the atlas will be used by the third image added.
  application.GetScene().Remove( imageView2 );
  application.SendNotification();
  application.Render(RENDER_FRAME_INTERVAL);

  Property::Map imageMap3;
  imageMap3[ ImageVisual::Property::URL ] = gImage_128_RGB;
  imageMap3[ ImageVisual::Property::DESIRED_HEIGHT ] = 100;
  imageMap3[ ImageVisual::Property::DESIRED_WIDTH ] = 100;
  imageMap3[ ImageVisual::Property::ATLASING ] = true;

  ImageView imageView3 = ImageView::New();
  imageView3.SetProperty( ImageView::Property::IMAGE, imageMap3 );

  application.GetPlatform().SetClosestImageSize(  Vector2(100, 100) );
  application.GetScene().Add( imageView3 );

  DALI_TEST_EQUALS( Test::WaitForEventThreadTrigger( 1 ), true, TEST_LOCATION );

  application.SendNotification();
  application.Render(RENDER_FRAME_INTERVAL);

  callStack.Enable(false);

  TraceCallStack::NamedParams params3;
  params3["width"] << 100;
  params3["height"] << 100;
  params3["xoffset"] << 0;
  params3["yoffset"] << 34;

  DALI_TEST_EQUALS(  callStack.FindMethodAndParams("TexSubImage2D", params3 ), true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliImageAtlasPackToAtlas(void)
{
  ToolkitTestApplication application;

  std::vector<PixelData> pixelDataContainer;
  pixelDataContainer.push_back( CreatePixelData( 20, 30 ) );
  pixelDataContainer.push_back( CreatePixelData( 10, 10 ) );
  pixelDataContainer.push_back( CreatePixelData( 45, 30 ) );
  pixelDataContainer.push_back( CreatePixelData( 20, 20 ) );

  Dali::Vector<Vector4> textureRects;
  Texture texture = ImageAtlas::PackToAtlas( pixelDataContainer, textureRects  );

 // --------------
 // |            |
 // |    45*30   |
//  |            |
//  --------------
//  | 20 |    | 20*20
//  |  * |____|
//  | 30 |  |  10*10
//  --------

  DALI_TEST_EQUALS( texture.GetWidth(), 45, TEST_LOCATION );
  DALI_TEST_EQUALS( texture.GetHeight(), 60, TEST_LOCATION );

  Rect<int> pixelArea = TextureCoordinateToPixelArea(textureRects[0], texture.GetWidth(), texture.GetHeight());
  DALI_TEST_EQUALS( pixelArea.x, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.y, 30, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.width, 20, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.height, 30, TEST_LOCATION );

  pixelArea = TextureCoordinateToPixelArea(textureRects[1], texture.GetWidth(), texture.GetHeight());
  DALI_TEST_EQUALS( pixelArea.x, 20, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.y, 50, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.width, 10, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.height, 10, TEST_LOCATION );

  pixelArea = TextureCoordinateToPixelArea(textureRects[2], texture.GetWidth(), texture.GetHeight());
  DALI_TEST_EQUALS( pixelArea.x, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.y, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.width, 45, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.height, 30, TEST_LOCATION );

  pixelArea = TextureCoordinateToPixelArea(textureRects[3], texture.GetWidth(), texture.GetHeight());
  DALI_TEST_EQUALS( pixelArea.x, 20, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.y, 30, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.width, 20, TEST_LOCATION );
  DALI_TEST_EQUALS( pixelArea.height, 20, TEST_LOCATION );

  END_TEST;
}
