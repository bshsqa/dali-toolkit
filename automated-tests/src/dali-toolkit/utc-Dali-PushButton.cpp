/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#include <iostream>
#include <stdlib.h>

// Need to override adaptor classes for toolkit test harness, so include
// test harness headers before dali headers.
#include <dali-toolkit-test-suite-utils.h>

#include <dali.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <dali-toolkit/dali-toolkit.h>

#include <dali-toolkit/devel-api/controls/buttons/button-devel.h>

#include <dali/devel-api/adaptor-framework/image-loading.h>

using namespace Dali;
using namespace Toolkit;

void utc_dali_toolkit_pushbutton_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_toolkit_pushbutton_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{
static const char* TEST_IMAGE_ONE = TEST_RESOURCE_DIR "/gallery-small-1.jpg";

static const Vector2 INSIDE_TOUCH_POINT_POSITON  = Vector2( 240, 400 );
static const Vector3 BUTTON_POSITON_TO_GET_INSIDE_TOUCH_EVENTS  = Vector3( 200, 360, 0 );
static const Size BUTTON_SIZE_TO_GET_INSIDE_TOUCH_EVENTS  = Size( 100, 100 );

static bool gPushButtonSelectedState = false;
bool PushButtonSelected( Button button )
{
  gPushButtonSelectedState = button.GetProperty<bool>(button.GetPropertyIndex("selected") );
  return true;
}

static bool gPushButtonPressed = false;

static bool PushButtonPressed( Button button )
{
  gPushButtonPressed = true;
  return true;
}

static bool gPushButtonReleased = false;

static bool PushButtonReleased( Button button )
{
  gPushButtonReleased = true;
  return true;
}

static bool gPushButtonClicked = false;

static bool PushButtonClicked( Button button )
{
  gPushButtonClicked = true;
  return gPushButtonClicked;
}

Dali::Integration::Point GetPointDownInside()
{
  Dali::Integration::Point point;
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( INSIDE_TOUCH_POINT_POSITON );
  return point;
}

Dali::Integration::Point GetPointUpInside()
{
  Dali::Integration::Point point;
  point.SetState( PointState::UP );
  point.SetScreenPosition( INSIDE_TOUCH_POINT_POSITON );
  return point;
}

Dali::Integration::Point GetPointLeave()
{
  Dali::Integration::Point point;
  point.SetState( PointState::LEAVE );
  point.SetScreenPosition( INSIDE_TOUCH_POINT_POSITON );
  return point;
}

Dali::Integration::Point GetPointEnter()
{
  Dali::Integration::Point point;
  point.SetState( PointState::MOTION );
  point.SetScreenPosition( INSIDE_TOUCH_POINT_POSITON );
  return point;
}

Dali::Integration::Point GetPointDownOutside()
{
  Dali::Integration::Point point;
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 10, 10 ) );
  return point;
}

Dali::Integration::Point GetPointUpOutside()
{
  Dali::Integration::Point point;
  point.SetState( PointState::UP );
  point.SetScreenPosition( Vector2( 10, 10 ) );
  return point;
}

// Set up the position of the button for the default test events
void SetupButtonForTestTouchEvents( ToolkitTestApplication& application, Button& button, bool useDefaultImages )
{
  button.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  button.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  button.SetProperty( Actor::Property::POSITION, BUTTON_POSITON_TO_GET_INSIDE_TOUCH_EVENTS );
  if ( useDefaultImages )
  {
    const Vector2 TEST_IMAGE_SIZE = Vector2( BUTTON_SIZE_TO_GET_INSIDE_TOUCH_EVENTS );
    TestPlatformAbstraction& platform = application.GetPlatform();
    platform.SetClosestImageSize( TEST_IMAGE_SIZE );
    button.SetProperty( Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, TEST_IMAGE_ONE );
    button.SetProperty( Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, TEST_IMAGE_ONE );
  }
}

static std::string GetButtonText( Button button )
{
  Property::Value value = button.GetProperty( Toolkit::Button::Property::LABEL );

  Property::Map *labelProperty = value.GetMap();

  std::string textLabel;

  if ( labelProperty )
  {
    Property::Value* value = labelProperty->Find( Toolkit::TextVisual::Property::TEXT );
    value->Get( textLabel );
  }

  return textLabel;
}

} //namespace

int UtcDaliPushButtonConstructorP(void)
{
  ToolkitTestApplication application;

  PushButton button;

  DALI_TEST_CHECK( !button );
  END_TEST;
}

int UtcDaliPushButtonCopyConstructorP(void)
{
  ToolkitTestApplication application;

  // Initialize an object, ref count == 1
  PushButton button = PushButton::New();

  PushButton copy( button );
  DALI_TEST_CHECK( copy );
  END_TEST;
}

int UtcDaliPushButtonMoveConstructor(void)
{
  ToolkitTestApplication application;

  PushButton button = PushButton::New();
  DALI_TEST_EQUALS( 1, button.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), false , TEST_LOCATION );
  button.SetProperty( Button::Property::TOGGLABLE, true );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );

  PushButton moved = std::move( button );
  DALI_TEST_CHECK( moved );
  DALI_TEST_EQUALS( 1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( moved.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );
  DALI_TEST_CHECK( !button );

  END_TEST;
}

int UtcDaliPushButtonAssignmentOperatorP(void)
{
  ToolkitTestApplication application;

  PushButton button = PushButton::New();

  PushButton copy( button );
  DALI_TEST_CHECK( copy );

  DALI_TEST_CHECK( button == copy );
  END_TEST;
}

int UtcDaliPushButtonMoveAssignment(void)
{
  ToolkitTestApplication application;

  PushButton button = PushButton::New();
  DALI_TEST_EQUALS( 1, button.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), false , TEST_LOCATION );
  button.SetProperty( Button::Property::TOGGLABLE, true );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );

  PushButton moved;
  moved = std::move( button );
  DALI_TEST_CHECK( moved );
  DALI_TEST_EQUALS( 1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( moved.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );
  DALI_TEST_CHECK( !button );

  END_TEST;
}

int UtcDaliPushButtonNewP(void)
{
  ToolkitTestApplication application;

  PushButton button = PushButton::New();

  DALI_TEST_CHECK( button );
  END_TEST;
}

int UtcDaliPushButtonDownCastP(void)
{
  ToolkitTestApplication application;

  PushButton button = PushButton::New();

  BaseHandle object(button);

  PushButton button2 = PushButton::DownCast( object );
  DALI_TEST_CHECK(button2);

  PushButton button3 = DownCast< PushButton >(object);
  DALI_TEST_CHECK(button3);
  END_TEST;
}

int UtcDaliPushButtonDownCastN(void)
{
  ToolkitTestApplication application;

  BaseHandle unInitializedObject;

  PushButton button1 = PushButton::DownCast( unInitializedObject );
  DALI_TEST_CHECK( !button1 );

  PushButton button2 = DownCast< PushButton >( unInitializedObject );
  DALI_TEST_CHECK( !button2 );
  END_TEST;
}

int UtcDaliPushButtonAutoRepeatingProperty(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutoRepeating");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( pushButton.GetPropertyIndex("autoRepeating"), true );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("autoRepeating")), true, TEST_LOCATION );

  pushButton.SetProperty( pushButton.GetPropertyIndex("autoRepeating"), false );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("autoRepeating")), false, TEST_LOCATION );

  pushButton.SetProperty( pushButton.GetPropertyIndex("autoRepeating"), true );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("autoRepeating")), true, TEST_LOCATION );

    END_TEST;
}

int UtcDaliPushButtonSetAutoRepeating(void)
{
  ToolkitTestApplication application;
  tet_infoline("UtcDaliPushButtonSetAutoRepeating\n");
  tet_infoline("Ensure setting AutoRepeating on a SELECTED Toggle button switches off Toggle\n");
  PushButton pushButton = PushButton::New();

  const bool INITIAL_TOGGLE_VALUE = true;
  const bool INITIAL_SELECTED_VALUE = true;

  pushButton.SetProperty( Button::Property::TOGGLABLE, INITIAL_TOGGLE_VALUE);
  pushButton.SetProperty( Button::Property::SELECTED, INITIAL_SELECTED_VALUE );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE  ), INITIAL_TOGGLE_VALUE , TEST_LOCATION );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), INITIAL_SELECTED_VALUE , TEST_LOCATION );

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE ), !INITIAL_TOGGLE_VALUE , TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonTogglableProperty(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetTogglableButton");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), true );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("togglable")), true, TEST_LOCATION );

  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), false );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("togglable")), false, TEST_LOCATION );

  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), true );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("togglable")), true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonAutoRepeatingPropertyAndTogglableButton(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutoRepeatingAndTogglableButton");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );
  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("togglable")), true, TEST_LOCATION );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("autoRepeating")), false, TEST_LOCATION );

  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), true );
  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("autoRepeating")), true, TEST_LOCATION );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("togglable")), false, TEST_LOCATION );
  END_TEST;
}

int UtcDaliPushButtonSelectedProperty01(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetSelected01");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), true );

  pushButton.StateChangedSignal().Connect( &PushButtonSelected );

  gPushButtonSelectedState = false;
  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), true , TEST_LOCATION );
  DALI_TEST_CHECK( gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), false , TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), true , TEST_LOCATION );
  DALI_TEST_CHECK( gPushButtonSelectedState );
  END_TEST;
}

int UtcDaliPushButtonSelectedProperty02(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetSelected02");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( pushButton.GetPropertyIndex("togglable"), false );
  pushButton.StateChangedSignal().Connect( &PushButtonSelected );

  gPushButtonSelectedState = false;
  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), false , TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), false , TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED  ), false , TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );
  END_TEST;
}

int UtcDaliPushButtonAutorepeatingDelayPropertyValues01(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutorepeatingDelayValues01");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  pushButton.SetProperty( Button::Property::INITIAL_AUTO_REPEATING_DELAY, 1.f );

  DALI_TEST_EQUALS( pushButton.GetProperty<float>(pushButton.GetPropertyIndex("initialAutoRepeatingDelay") ), 1.f, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonAutorepeatingDelayPropertyValues02(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutorepeatingDelayValues02");

  PushButton pushButton = PushButton::New();

  bool assert1( false );
  bool assert2( false );

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  try
  {
    pushButton.SetProperty( Button::Property::INITIAL_AUTO_REPEATING_DELAY, -1.f );
  }
  catch( Dali::DaliException& e )
  {
    DALI_TEST_PRINT_ASSERT( e );
    DALI_TEST_EQUALS(e.condition, "initialAutoRepeatingDelay > 0.f", TEST_LOCATION);
    assert1 = true;
  }

  try
  {
    pushButton.SetProperty( Button::Property::NEXT_AUTO_REPEATING_DELAY, -1.f );
  }
  catch( Dali::DaliException& e )
  {
    DALI_TEST_PRINT_ASSERT( e );
    DALI_TEST_EQUALS(e.condition, "nextAutoRepeatingDelay > 0.f", TEST_LOCATION);
    assert2 = true;
  }

  DALI_TEST_CHECK( assert1 && assert2 );
  END_TEST;
}

int UtcDaliPushButtonLabelProperty(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetLabelText");

  const std::string STR( "Hola!" );

  PushButton pushButton = PushButton::New();

  application.SendNotification();
  application.Render();

  pushButton.SetProperty( Toolkit::Button::Property::LABEL,
                            Property::Map().Add( Toolkit::Visual::Property::TYPE, Toolkit::Visual::TEXT )
                                           .Add( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f )
                          );


  pushButton.SetProperty( Toolkit::Button::Property::LABEL, STR );

  DALI_TEST_EQUALS( GetButtonText( pushButton ), STR, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonPressed(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonPressed");

  PushButton pushButton = PushButton::New();
  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::POSITION, BUTTON_POSITON_TO_GET_INSIDE_TOUCH_EVENTS );
  pushButton.SetProperty( Actor::Property::SIZE, BUTTON_SIZE_TO_GET_INSIDE_TOUCH_EVENTS );

  application.GetScene().Add( pushButton );

  application.SendNotification();
  application.Render();

  gPushButtonPressed = false;

  // connect to its touch signal
  pushButton.PressedSignal().Connect( &PushButtonPressed );

  Dali::Integration::TouchEvent eventDown;
  eventDown.AddPoint( GetPointDownInside() );

  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( eventDown );

  DALI_TEST_CHECK( gPushButtonPressed );
  END_TEST;
}

int UtcDaliPushButtonReleased(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonReleased");

  PushButton pushButton = PushButton::New();
  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::POSITION, BUTTON_POSITON_TO_GET_INSIDE_TOUCH_EVENTS );
  pushButton.SetProperty( Actor::Property::SIZE, BUTTON_SIZE_TO_GET_INSIDE_TOUCH_EVENTS );

  application.GetScene().Add( pushButton );

  application.SendNotification();
  application.Render();

  // connect to its touch signal
  pushButton.ReleasedSignal().Connect( &PushButtonReleased );

  Dali::Integration::TouchEvent event;

  // Test1. Touch point down and up inside the button.

  gPushButtonReleased = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gPushButtonReleased );

  // Test2. Touch point down and up outside the button.

  gPushButtonReleased = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gPushButtonReleased );

  // Test3. Touch point down inside and up outside the button.

  gPushButtonReleased = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointLeave() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gPushButtonReleased );

  // Test4. Touch point down outside and up inside the button.

  gPushButtonReleased = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointEnter() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gPushButtonReleased );
  END_TEST;
}

int UtcDaliPushButtonSelected(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSelected");

  PushButton pushButton = PushButton::New();
  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::POSITION, BUTTON_POSITON_TO_GET_INSIDE_TOUCH_EVENTS );
  pushButton.SetProperty( Actor::Property::SIZE, BUTTON_SIZE_TO_GET_INSIDE_TOUCH_EVENTS );

  application.GetScene().Add( pushButton );

  application.SendNotification();
  application.Render();

  // connect to its touch signal
  pushButton.StateChangedSignal().Connect( &PushButtonSelected );

  Dali::Integration::TouchEvent event;

  // Test1. No togglable button.

  gPushButtonSelectedState = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gPushButtonSelectedState );

  // Set togglable property.
  pushButton.SetProperty( Button::Property::TOGGLABLE, true );

  // Test2. Touch point down and up inside the button twice.
  gPushButtonSelectedState = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gPushButtonSelectedState );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gPushButtonSelectedState );

  // Test3. Touch point down and up outside the button.

  gPushButtonSelectedState = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gPushButtonSelectedState );

  // Test4. Touch point down inside and up outside the button.
  //        State changes on Button down
  gPushButtonSelectedState = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointLeave() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gPushButtonSelectedState );

  // Test5. Touch point down outside and up inside the button.
  // Start in unselected state
  pushButton.SetProperty( Button::Property::SELECTED, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>(pushButton.GetPropertyIndex("selected") ),false , TEST_LOCATION );

  gPushButtonSelectedState = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointEnter() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gPushButtonSelectedState );
  END_TEST;
}

int UtcDaliPushButtonPropertySetLabelPadding(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonPropertySetLabelPadding");

  PushButton pushButton = PushButton::New();
  pushButton.SetProperty( Toolkit::PushButton::Property::LABEL_PADDING, Vector4( 1.0f, 1.0f, 1.0f, 1.0f ) );
  DALI_TEST_EQUALS( pushButton.GetProperty<Vector4>( Toolkit::PushButton::Property::LABEL_PADDING ), Vector4( 1.0f, 1.0f, 1.0f, 1.0f ), Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  pushButton.SetProperty( Toolkit::PushButton::Property::LABEL_PADDING, Vector4( 10.0f, 10.0f, 10.0f, 10.0f ) );
  DALI_TEST_EQUALS( pushButton.GetProperty<Vector4>( Toolkit::PushButton::Property::LABEL_PADDING ), Vector4( 10.0f, 10.0f, 10.0f, 10.0f ), Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonPropertySetIconPadding(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonPropertySetIconPadding");

  PushButton pushButton = PushButton::New();
  pushButton.SetProperty( Toolkit::PushButton::Property::ICON_PADDING, Vector4( 1.0f, 1.0f, 1.0f, 1.0f ) );
  DALI_TEST_EQUALS( pushButton.GetProperty<Vector4>( Toolkit::PushButton::Property::ICON_PADDING ), Vector4( 1.0f, 1.0f, 1.0f, 1.0f ), Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  pushButton.SetProperty( Toolkit::PushButton::Property::ICON_PADDING, Vector4( 10.0f, 10.0f, 10.0f, 10.0f ) );
  DALI_TEST_EQUALS( pushButton.GetProperty<Vector4>( Toolkit::PushButton::Property::ICON_PADDING ), Vector4( 10.0f, 10.0f, 10.0f, 10.0f ), Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonPaddingLayout(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonPaddingLayout");

  // This test creates padding for an icon and a label.
  // The icon and label are each enabled and disabled to confirm the correct padding is used.
  PushButton pushButton = PushButton::New();

  const Vector4 TEST_ICON_PADDING( 20.0f, 20.0f, 20.0f, 20.0f );
  const Vector4 TEST_LABEL_PADDING( 10.0f, 10.0f, 10.0f, 10.0f );

  // Get actual size of test image
  ImageDimensions testImageSize = Dali::GetClosestImageSize( TEST_IMAGE_ONE );
  const Vector2 TEST_IMAGE_SIZE( testImageSize.GetWidth(), testImageSize.GetHeight() );

  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::POSITION, Vector2( 0.0f, 0.0f ));
  pushButton.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );

  application.GetScene().Add( pushButton );

  application.SendNotification();
  application.Render();

  // First test the size is zero.
  // No padding should be added as there is no label or icon.
  Vector2 size( Vector2::ZERO );
  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button Natural Size(%f,%f)\n", pushButton.GetNaturalSize().width, pushButton.GetNaturalSize().height );

  DALI_TEST_EQUALS( size, Vector2::ZERO, Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  // Check label only padding
  pushButton.SetProperty( Toolkit::Button::Property::LABEL, "Label" );

  application.SendNotification();
  application.Render();

  Vector2 sizeWithLabelWithoutPadding( Vector2::ZERO );
  sizeWithLabelWithoutPadding.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  sizeWithLabelWithoutPadding.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );

  tet_printf( "Button RelayoutSize label without padding (%f,%f)\n", sizeWithLabelWithoutPadding.width, sizeWithLabelWithoutPadding.height );

  // Add label padding to label
  pushButton.SetProperty( Toolkit::PushButton::Property::LABEL_PADDING, TEST_LABEL_PADDING );
  application.SendNotification();
  application.Render();

  Vector2 sizeLabelAndPadding( Vector2::ZERO );
  sizeLabelAndPadding.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  sizeLabelAndPadding.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button RelayoutSize after label padding(%f,%f)\n", sizeLabelAndPadding.width, sizeLabelAndPadding.height );

  // If control size has increased beyond size of just label then padding has been applied
  DALI_TEST_GREATER( sizeLabelAndPadding.width, sizeWithLabelWithoutPadding.width+TEST_LABEL_PADDING.x, TEST_LOCATION );
  DALI_TEST_GREATER( sizeLabelAndPadding.height, sizeWithLabelWithoutPadding.height+TEST_LABEL_PADDING.w, TEST_LOCATION );

  // Re-initialise the button so we can setup icon-only padding.
  pushButton.Unparent();
  pushButton = PushButton::New();

  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::POSITION, Vector2( 0.0f, 0.0f ));
  pushButton.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );

  application.GetScene().Add( pushButton );


  pushButton.SetProperty( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT, "BEGIN" );
  pushButton.SetProperty( Toolkit::Button::Property::UNSELECTED_VISUAL, TEST_IMAGE_ONE );
  pushButton.SetProperty( Toolkit::Button::Property::SELECTED_VISUAL, TEST_IMAGE_ONE );

  application.SendNotification();
  application.Render();

  // Size of button with just icon
  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button RelayoutSize with icon(%f,%f)\n", size.width, size.height );

  pushButton.SetProperty( Toolkit::PushButton::Property::ICON_PADDING, TEST_ICON_PADDING );

  application.SendNotification();
  application.Render();
  DALI_TEST_EQUALS( size, TEST_IMAGE_SIZE, Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button RelayoutSize after icon padding(%f,%f)\n", size.width, size.height );
  const Vector2 expectedIconAndPaddingSize( TEST_ICON_PADDING.x+TEST_ICON_PADDING.y+TEST_IMAGE_SIZE.width, TEST_ICON_PADDING.w + TEST_ICON_PADDING.z + TEST_IMAGE_SIZE.height );
  DALI_TEST_EQUALS( size, expectedIconAndPaddingSize, Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  // Now test padding for both label and icon simultaneously.
  pushButton.SetProperty( Toolkit::Button::Property::LABEL, "Label" );

  application.SendNotification();
  application.Render();

  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button RelayoutSize after label added(%f,%f)\n", size.width, size.height );

  pushButton.SetProperty( Toolkit::PushButton::Property::LABEL_PADDING, TEST_LABEL_PADDING );

  application.SendNotification();
  application.Render();

  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button RelayoutSize after icon and label padding(%f,%f)\n", size.width, size.height );

  DALI_TEST_EQUALS( size.width, sizeLabelAndPadding.width + expectedIconAndPaddingSize.width, TEST_LOCATION );
  // Test height of control is same as icon and padding, as Text is smaller than icon
  DALI_TEST_EQUALS( size.height, expectedIconAndPaddingSize.height, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonAlignmentLayout(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonAlignmentLayout");

  /*
   * This test checks different alignments for the icon against the label.
   * The icon is then moved around the label in each of it's alignments.
   * The final relayed out size is checked to confirm the layout has been done correctly.
   *
   * There is an Icon which has 0 width and height, but with 75 padding on all sides.
   *  - Therefore total width and height are both 150.
   *
   * There is a Label which has "an unknown" width and height, but with 30 padding on all sides.
   *  - Therefore total width and height are 60+x and 60+y respectively.
   *    Where x & y are the width and height of the text.
   *
   * The width of the button will always expand to the largest of the icon and label sizes (plus padding).
   * So We use the padding to help us determine the orientation is correct for each alignment.
   *
   * |<- 150 ->|         |<-- 60+x -->|
   *
   * +---------+   -
   * |         |   ^     +------------+   -
   * |         |   |     |            |   ^
   * |  Icon   |  150    |   Label    |  60+y
   * |         |   |     |            |   v
   * |         |   v     +------------+   -
   * +---------+   -
   */

  const Vector4 TEST_ICON_PADDING( 70.0f, 70.0f, 70.0f, 70.0f );
  const Vector4 TEST_LABEL_PADDING( 30.0f, 30.0f, 30.0f, 30.0f );

  // Get actual size of test image
  ImageDimensions testImageSize = Dali::GetClosestImageSize( TEST_IMAGE_ONE );
  const Vector2 TEST_IMAGE_SIZE( testImageSize.GetWidth(), testImageSize.GetHeight() );

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::POSITION, Vector2( 0.0f, 0.0f ));
  pushButton.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );

  application.GetScene().Add( pushButton );

  // Add a label and get size of control
  pushButton.SetProperty( Toolkit::Button::Property::LABEL, "Label" );
  application.SendNotification();
  application.Render();

  // First get the size of control with just label
  Vector2 justLabelSize( Vector2::ZERO );
  justLabelSize.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  justLabelSize.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );
  tet_printf( "Button RelayoutSize with just label and no padding(%f,%f)\n", justLabelSize.width, justLabelSize.height );

  pushButton.SetProperty( Toolkit::PushButton::Property::LABEL_PADDING, TEST_LABEL_PADDING );
  application.SendNotification();
  application.Render();

  // Size of Label and Padding
  Vector2 expectedLabelAndPaddingSize( Vector2::ZERO );
  expectedLabelAndPaddingSize.width = justLabelSize.width + TEST_LABEL_PADDING.x + TEST_LABEL_PADDING.y;
  expectedLabelAndPaddingSize.height = justLabelSize.height + TEST_LABEL_PADDING.w + TEST_LABEL_PADDING.z;

  Vector2 labelAndPaddingSize( Vector2::ZERO );
  labelAndPaddingSize.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  labelAndPaddingSize.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );

  DALI_TEST_EQUALS( labelAndPaddingSize, expectedLabelAndPaddingSize , Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  const Vector2 testImageWithPaddingSize = Vector2 ( ( TEST_IMAGE_SIZE.width + TEST_ICON_PADDING.x + TEST_ICON_PADDING.y ),
                                                     ( TEST_IMAGE_SIZE.height + TEST_ICON_PADDING.w + TEST_ICON_PADDING.z ) );

  // Add Icon and set its alignment
  pushButton.SetProperty( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT, "BEGIN" );
  pushButton.SetProperty( Toolkit::Button::Property::UNSELECTED_VISUAL, TEST_IMAGE_ONE );
  pushButton.SetProperty( Toolkit::Button::Property::SELECTED_VISUAL, TEST_IMAGE_ONE );
  pushButton.SetProperty( Toolkit::PushButton::Property::ICON_PADDING, TEST_ICON_PADDING );

  application.SendNotification();
  application.Render();

  Vector2 size( Vector2::ZERO );
  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );

  /*
   * Test Icon right alignment.
   * Height grows to largest of Icon or Label (+ padding).
   * Normally this will be Icons height, except with very large font sizes.
   *
   *  +------------+---------+
   *  |............+         |
   *  |            |         |
   *  |   Label    |  Icon   |
   *  |            |         |
   *  |............+         |
   *  +------------+---------+
   */
  DALI_TEST_EQUALS( size.width, ( testImageWithPaddingSize.width + labelAndPaddingSize.width ) , TEST_LOCATION );
  DALI_TEST_EQUALS( size.height, ( std::max( testImageWithPaddingSize.height, labelAndPaddingSize.height) ) , Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  // Now test left alignment matches right for size.
  pushButton.SetProperty( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT, "END" );

  application.SendNotification();
  application.Render();

  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );

  /*
   * Test Icon left alignment.
   * Height grows to largest of Icon or Label (+ padding).
   * Normally this will be Icons height, except with very large font sizes.
   *
   *  +---------+------------+
   *  |         +............|
   *  |         |            |
   *  |  Icon   |   Label    |
   *  |         |            |
   *  |         +............|
   *  +---------+------------+
   */
  DALI_TEST_EQUALS( size.width, ( testImageWithPaddingSize.width + labelAndPaddingSize.width ) , TEST_LOCATION );
  DALI_TEST_EQUALS( size.height, ( std::max( testImageWithPaddingSize.height, labelAndPaddingSize.height) ) , Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  tet_infoline(" Test Icon TOP alignment - Width grows to largest of Icon or label (plus padding)");
  /*
   *
   *  +---------+
   *  |         |
   *  |         |
   *  |  Icon   |
   *  |         |
   *  |         |
   *  +---------+
   *  |         |
   *  |  Label  |
   *  |         |
   *  +---------+
   *
   */

  tet_infoline("SetProperty on LABEL_RELATIVE_ALIGNMENT should relayout the Button");
  pushButton.SetProperty( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT, "BOTTOM" );

  application.SendNotification();
  application.Render();

  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );

  tet_printf("Natural width (%f)\n",pushButton.GetNaturalSize().width);
  tet_printf("Natural height (%f)\n",pushButton.GetNaturalSize().height);

  tet_printf(" UtcDaliPushButtonAlignmentLayout Top layout - Image and Padding size (%f,%f)\n", testImageWithPaddingSize.width, testImageWithPaddingSize.height );
  tet_printf(" UtcDaliPushButtonAlignmentLayout Top layout - Text and Padding size (%f,%f)\n", labelAndPaddingSize.width, labelAndPaddingSize.height );

  DALI_TEST_EQUALS( size.width, ( std::max( testImageWithPaddingSize.width, labelAndPaddingSize.width ) ) , TEST_LOCATION );

  DALI_TEST_EQUALS( size.height,( testImageWithPaddingSize.height + labelAndPaddingSize.height ) , TEST_LOCATION );

  /*
   * Test Icon bottom alignment.
   * Width grows to largest of Icon or Label (+ padding).
   *
   *  +---------+
   *  |         |
   *  |  Label  |
   *  |         |
   *  +---------+
   *  |         |
   *  |         |
   *  |  Icon   |
   *  |         |
   *  |         |
   *  +---------+
   */
  tet_infoline(" Test Icon BOTTOM alignment - Width grows to largest of Icon or label (plus padding)");
  pushButton.SetProperty( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT, "TOP" );

  application.SendNotification();
  application.Render();

  size.width = pushButton.GetRelayoutSize( Dimension::WIDTH );
  size.height = pushButton.GetRelayoutSize( Dimension::HEIGHT );

  DALI_TEST_EQUALS( size.width, ( std::max(testImageWithPaddingSize.width, labelAndPaddingSize.width )) , TEST_LOCATION );
  DALI_TEST_EQUALS( size.height,( testImageWithPaddingSize.height + labelAndPaddingSize.height ) , TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonSetUnSelectedVisual01P(void)
{
  tet_infoline(" Test adding a visual for the UNSELECTED_VISUAL property, removing Button from stage and counting renderers\n");
  ToolkitTestApplication application;

  PushButton pushButton = PushButton::New();
  pushButton.SetProperty( Actor::Property::SIZE, Vector2( BUTTON_SIZE_TO_GET_INSIDE_TOUCH_EVENTS ) );

  application.GetScene().Add( pushButton );

  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR, Color::BLUE);

  pushButton.SetProperty( Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, propertyMap );

  tet_infoline(" UNSELECTED_VISUAL Added to button\n");

  application.SendNotification();
  application.Render(0);

  unsigned int rendererCount = pushButton.GetRendererCount();
  tet_printf("After adding UNSELECTED_BACKGROUND_VISUAL the renderer count is(%d)\n", rendererCount );

  DALI_TEST_EQUALS( pushButton.GetRendererCount(), 1 , TEST_LOCATION );

  tet_printf("Remove button from stage\n" );

  application.GetScene().Remove( pushButton );

  rendererCount = pushButton.GetRendererCount();
  tet_printf("After removing pushbutton from stage the renderer count is(%d)\n ", rendererCount );

  DALI_TEST_EQUALS( pushButton.GetRendererCount(), 0, TEST_LOCATION );

  tet_printf("After removing pushbutton from stage the renderer count is(%d)\n ", rendererCount );

  Property::Map propertyMap2;
  propertyMap2.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap2.Insert(ColorVisual::Property::MIX_COLOR, Color::RED);
  pushButton.SetProperty( Toolkit::Button::Property::UNSELECTED_VISUAL, propertyMap2 );

  tet_printf("Added UNSELECTED_VISUAL and add button back to Stage\n");

  application.GetScene().Add( pushButton );

  tet_printf("With UNSELECTED_BACKGROUND_VISUAL and UNSELECTED_VISUAL the renderer count is(%d)\n", pushButton.GetRendererCount() );

  DALI_TEST_EQUALS( pushButton.GetRendererCount(), 2, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonSetSelectedVisualN(void)
{
  tet_infoline(" Test adding a broken visual for the UNSELECTED_VISUAL property");

  ToolkitTestApplication application;

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  pushButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  pushButton.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );

  application.GetScene().Add( pushButton );
  application.SendNotification();
  application.Render(0);

  unsigned int preRendererCount = pushButton.GetRendererCount();
  tet_printf("RendererCount prior to adding visual(%d)\n",preRendererCount);
  DALI_TEST_EQUALS( preRendererCount, 0, TEST_LOCATION );

  application.GetScene().Remove( pushButton );
  application.SendNotification();
  application.Render(0);

  Property::Map colorMap;
  const int BROKEN_VISUAL_TYPE = 999999999;

  colorMap.Insert(Visual::Property::TYPE,  BROKEN_VISUAL_TYPE);
  colorMap.Insert(BorderVisual::Property::COLOR,  Color::BLUE);
  colorMap.Insert(BorderVisual::Property::SIZE,  5.f);
  pushButton.SetProperty( Toolkit::Button::Property::UNSELECTED_VISUAL, colorMap );

  application.GetScene().Add( pushButton );
  application.SendNotification();
  application.Render(0);

  unsigned int postRendererCount  = pushButton.GetRendererCount();
  tet_printf("RendererCount post broken visual (%d)\n", postRendererCount);
  DALI_TEST_EQUALS( postRendererCount, 0, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonToggleSignalP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliButtonToggleSignalP Ensure Signals emitted");

  PushButton button = PushButton::New();
  button.SetProperty( Button::Property::TOGGLABLE, true);

  SetupButtonForTestTouchEvents( application, button, true );

  application.GetScene().Add( button );

  application.SendNotification();
  application.Render();

  // connect to its signal
  button.ClickedSignal().Connect( &PushButtonClicked );
  gPushButtonClicked = false;

  tet_infoline(" Touch down and up within button");
  Dali::Integration::TouchEvent event;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_EQUALS( gPushButtonClicked, true, TEST_LOCATION );

  END_TEST;
}

// Deprecated API Tests

int UtcDaliPushButtonSetGetAutoRepeating(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutoRepeating");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::AUTO_REPEATING ), true, TEST_LOCATION );

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::AUTO_REPEATING ), false, TEST_LOCATION );

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::AUTO_REPEATING ), true, TEST_LOCATION );
  END_TEST;
}

int UtcDaliPushButtonSetGetTogglableButton(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetTogglableButton");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::TOGGLABLE, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE ), true, TEST_LOCATION );

  pushButton.SetProperty( Button::Property::TOGGLABLE, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE ), false, TEST_LOCATION );

  pushButton.SetProperty( Button::Property::TOGGLABLE, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE ), true, TEST_LOCATION );
  END_TEST;
}

int UtcDaliPushButtonSetGetAutoRepeatingAndTogglableButton(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutoRepeatingAndTogglableButton");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );
  pushButton.SetProperty( Button::Property::TOGGLABLE, true);

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE ), true, TEST_LOCATION );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::AUTO_REPEATING ), false, TEST_LOCATION );

  pushButton.SetProperty( Button::Property::TOGGLABLE, true);
  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::AUTO_REPEATING ), true, TEST_LOCATION );
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::TOGGLABLE ), false, TEST_LOCATION );

  END_TEST;
}

int UtcDaliPushButtonSetGetSelected01(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetSelected01");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::TOGGLABLE, true);
  pushButton.StateChangedSignal().Connect( &PushButtonSelected );

  gPushButtonSelectedState = false;
  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED), true, TEST_LOCATION );
  DALI_TEST_CHECK( gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED), false, TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED), true, TEST_LOCATION );
  DALI_TEST_CHECK( gPushButtonSelectedState );
  END_TEST;
}

int UtcDaliPushButtonSetGetSelected02(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetSelected02");

  PushButton pushButton = PushButton::New();

  tet_infoline(" Set Toggle feature off");
  pushButton.SetProperty( Button::Property::TOGGLABLE, false);
  pushButton.StateChangedSignal().Connect( &PushButtonSelected );

  gPushButtonSelectedState = false;
  tet_infoline(" Try to set to selected, expecting failure as not a toggle button");
  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED), false, TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, false );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED), false, TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  pushButton.SetProperty( Button::Property::SELECTED, true );

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( Button::Property::SELECTED ), false, TEST_LOCATION );
  DALI_TEST_CHECK( !gPushButtonSelectedState );

  END_TEST;
}

int UtcDaliPushButtonSetGetAutorepeatingDelayValues01(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutorepeatingDelayValues01");

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  pushButton.SetProperty( Button::Property::INITIAL_AUTO_REPEATING_DELAY, 1.f);
  DALI_TEST_EQUALS( pushButton.GetProperty<float>( Button::Property::INITIAL_AUTO_REPEATING_DELAY ), 1.f, TEST_LOCATION );

  pushButton.SetProperty( Button::Property::NEXT_AUTO_REPEATING_DELAY, 1.f);

  DALI_TEST_EQUALS( pushButton.GetProperty<float>( Button::Property::NEXT_AUTO_REPEATING_DELAY ), 1.f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliPushButtonSetGetAutorepeatingDelayValues02(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetGetAutorepeatingDelayValues02");

  PushButton pushButton = PushButton::New();

  bool assert1( false );
  bool assert2( false );

  pushButton.SetProperty( Button::Property::AUTO_REPEATING, true );

  try
  {
    pushButton.SetProperty( Button::Property::INITIAL_AUTO_REPEATING_DELAY, -1.f );
  }
  catch( Dali::DaliException& e )
  {
    DALI_TEST_PRINT_ASSERT( e );
    DALI_TEST_EQUALS(e.condition, "initialAutoRepeatingDelay > 0.f", TEST_LOCATION);
    assert1 = true;
  }

  try
  {
    pushButton.SetProperty( Button::Property::NEXT_AUTO_REPEATING_DELAY, -1.f );
  }
  catch( Dali::DaliException& e )
  {
    DALI_TEST_PRINT_ASSERT( e );
    DALI_TEST_EQUALS(e.condition, "nextAutoRepeatingDelay > 0.f", TEST_LOCATION);
    assert2 = true;
  }

  DALI_TEST_CHECK( assert1 && assert2 );
  END_TEST;
}

int UtcDaliPushButtonSetLabelText(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonSetLabelText");

  const std::string STR( "Hola!" );

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty( Toolkit::Button::Property::LABEL,
                          Property::Map().Add( Toolkit::Visual::Property::TYPE, Toolkit::Visual::TEXT )
                                         .Add( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f )
                        );

  application.SendNotification();
  application.Render();

  pushButton.SetProperty( Button::Property::LABEL, STR );

  DALI_TEST_EQUALS(GetButtonText( pushButton ), STR, TEST_LOCATION);

  END_TEST;
}
