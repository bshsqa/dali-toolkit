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
#include "dali-toolkit-test-utils/toolkit-timer.h"

#include <dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/integration-api/events/touch-event-integ.h>

#include <dali-toolkit/devel-api/controls/buttons/button-devel.h>

using namespace Dali;
using namespace Toolkit;


void utc_dali_toolkit_button_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_toolkit_button_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{
static bool gIsCalledButtonCallback = false;
static bool gIsCalledChildButtonCallback = false;

static bool ButtonCallback( Button button )
{
  gIsCalledButtonCallback = true;
  return false;
}

static bool ChildButtonCallback( Button button )
{
  gIsCalledChildButtonCallback = true;
  return false;
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

struct CallbackFunctor
{
  CallbackFunctor(bool* callbackFlag)
  : mCallbackFlag( callbackFlag )
  {
  }

  void operator()()
  {
    *mCallbackFlag = true;
  }
  bool* mCallbackFlag;
};

Dali::Integration::Point GetPointDownInside()
{
  Dali::Integration::Point point;
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 240, 400 ) );
  return point;
}

Dali::Integration::Point GetPointUpInside()
{
  Dali::Integration::Point point;
  point.SetState( PointState::UP );
  point.SetScreenPosition( Vector2( 240, 400 ) );
  return point;
}

Dali::Integration::Point GetPointLeave()
{
  Dali::Integration::Point point;
  point.SetState( PointState::LEAVE );
  point.SetScreenPosition( Vector2( 240, 400 ) );
  return point;
}

Dali::Integration::Point GetPointEnter()
{
  Dali::Integration::Point point;
  point.SetState( PointState::MOTION );
  point.SetScreenPosition( Vector2( 240, 400 ) );
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

} // namespace

int UtcDaliButtonConstructorP(void)
{
  ToolkitTestApplication application;

  Button button;

  DALI_TEST_CHECK( !button );
  END_TEST;
}

int UtcDaliButtonCopyConstructorP(void)
{
  ToolkitTestApplication application;

  // Initialize an object, ref count == 1
  Button button = PushButton::New();

  Button copy( button );
  DALI_TEST_CHECK( copy );
  END_TEST;
}

int UtcDaliButtonMoveConstructor(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();
  DALI_TEST_EQUALS( 1, button.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), false , TEST_LOCATION );
  button.SetProperty( Button::Property::TOGGLABLE, true );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );

  Button moved = std::move( button );
  DALI_TEST_CHECK( moved );
  DALI_TEST_EQUALS( 1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( moved.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );
  DALI_TEST_CHECK( !button );

  END_TEST;
}

int UtcDaliButtonAssignmentOperatorP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  Button copy( button );
  DALI_TEST_CHECK( copy );

  DALI_TEST_CHECK( button == copy );
  END_TEST;
}

int UtcDaliButtonMoveAssignment(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();
  DALI_TEST_EQUALS( 1, button.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), false , TEST_LOCATION );
  button.SetProperty( Button::Property::TOGGLABLE, true );
  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );

  Button moved;
  moved = std::move( button );
  DALI_TEST_CHECK( moved );
  DALI_TEST_EQUALS( 1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( moved.GetProperty<bool>( Button::Property::TOGGLABLE ), true , TEST_LOCATION );
  DALI_TEST_CHECK( !button );

  END_TEST;
}

int UtcDaliButtonDownCastP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  BaseHandle object(button);

  Button button2 = Button::DownCast( object );
  DALI_TEST_CHECK(button2);

  Button button3 = DownCast< Button >(object);
  DALI_TEST_CHECK(button3);
  END_TEST;
}

int UtcDaliButtonDownCastN(void)
{
  ToolkitTestApplication application;

  BaseHandle unInitializedObject;

  Button button1 = Button::DownCast( unInitializedObject );
  DALI_TEST_CHECK( !button1 );

  Button button2 = DownCast< Button >( unInitializedObject );
  DALI_TEST_CHECK( !button2 );
  END_TEST;
}

int UtcDaliButtonDisabledPropertyP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();


  button.SetProperty( button.GetPropertyIndex("disabled"), true );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("disabled")), true, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("disabled"), false );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("disabled")), false, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("disabled"), true );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("disabled")), true, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("disabled"), false );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("disabled")), false, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonSetDisabledWithDifferentStates01P(void)
{
  ToolkitTestApplication application;

  tet_infoline("UtcDaliButtonSetDisabledWithDifferentStates01P\n");

  Button button = PushButton::New();

  bool SELECTED = true;

  button.SetProperty( Button::Property::TOGGLABLE, true);
  button.SetProperty( Button::Property::SELECTED, SELECTED );

  button.SetProperty( Button::Property::DISABLED, true);

  tet_infoline("Set button to SELECTED = false whilst disabled, should not change to false\n");
  button.SetProperty( Button::Property::SELECTED, !SELECTED );

  bool isSelected = button.GetProperty<bool>( Button::Property::SELECTED ) ;

  DALI_TEST_EQUALS( isSelected, SELECTED , TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonSetDisabledWithDifferentStates02P(void)
{
  ToolkitTestApplication application;

  tet_infoline("UtcDaliButtonSetDisabledWithDifferentStates02\n");

  Button button = PushButton::New();

  bool SELECTED = true;

  button.SetProperty( Button::Property::TOGGLABLE, true );
  button.SetProperty( Button::Property::SELECTED, SELECTED );
  button.SetProperty( Button::Property::DISABLED, true );

  bool isSelected =  button.GetProperty<bool>( Button::Property::SELECTED );
  DALI_TEST_EQUALS( isSelected, SELECTED , TEST_LOCATION );
  tet_infoline("Set button to DISABLED = false whilst disabled and then set to unselected\n");

  button.SetProperty( Button::Property::DISABLED, false);
  button.SetProperty( Button::Property::SELECTED, !SELECTED );

  isSelected = button.GetProperty<bool>( Button::Property::SELECTED );
  DALI_TEST_EQUALS( isSelected, !SELECTED , TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonPropertyGetLabelAlignment(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliPushButtonPropertyGetLabelAlignment\n");

  Button button = PushButton::New();
  button.SetProperty( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT, "END"  );
  DALI_TEST_EQUALS( button.GetProperty<std::string>( Toolkit::DevelButton::Property::LABEL_RELATIVE_ALIGNMENT ), "END", TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonIsDisabledP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  button.SetProperty( Button::Property::DISABLED, true);

  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::DISABLED ), true, TEST_LOCATION );

  button.SetProperty( Button::Property::DISABLED, false);

  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::DISABLED ), false, TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonAutoRepeatingPropertyP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  button.SetProperty( button.GetPropertyIndex("autoRepeating"), true );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("autoRepeating")), true, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("autoRepeating"), false );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("autoRepeating")), false, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("autoRepeating"), true );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("autoRepeating")), true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonIsAutoRepeatingP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  button.SetProperty( Button::Property::AUTO_REPEATING, true);

  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::AUTO_REPEATING ), true, TEST_LOCATION );

  button.SetProperty( Button::Property::AUTO_REPEATING, false);

  DALI_TEST_EQUALS( button.GetProperty<bool>( Button::Property::AUTO_REPEATING ), false, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonAutoRepeatingP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliButtonPressedSignalP  Setup Autorepeating and check multiple clicked signals received\n");

  const float AUTO_REPEATING_DELAY = 0.15f;

  Button button = PushButton::New();
  button.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  button.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  button.SetProperty( Actor::Property::POSITION, Vector2( 240, 400 ));
  button.SetProperty( Actor::Property::SIZE, Vector2( 100, 100 ) );
  application.GetScene().Add( button );

  application.SendNotification();
  application.Render();

  button.SetProperty( Toolkit::Button::Property::AUTO_REPEATING, true  );
  button.SetProperty( Toolkit::Button::Property::INITIAL_AUTO_REPEATING_DELAY, AUTO_REPEATING_DELAY );
  // connect to its touch signal
  ConnectionTracker* testTracker = new ConnectionTracker();
  button.PressedSignal().Connect( &ButtonCallback );
  button.ClickedSignal().Connect( &ButtonCallback );
  bool clickedSignal = false;
  bool pressedSignal = false;
  button.ConnectSignal( testTracker, "pressed", CallbackFunctor(&pressedSignal) );
  button.ConnectSignal( testTracker, "clicked", CallbackFunctor(&clickedSignal) );

  Dali::Integration::TouchEvent event;

  // Touch point down and up inside the button.

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  DALI_TEST_EQUALS( gIsCalledButtonCallback, true, TEST_LOCATION );
  DALI_TEST_EQUALS( pressedSignal, true, TEST_LOCATION );
  tet_infoline("Consume first clicked signal then wait\n");

  gIsCalledButtonCallback = false;
  Dali::Timer timer = Timer::New( AUTO_REPEATING_DELAY );
  timer.MockEmitSignal();
  application.Wait( AUTO_REPEATING_DELAY*2 );
  DALI_TEST_EQUALS( clickedSignal, true, TEST_LOCATION );
  tet_infoline("Check gIsCalledButtonCallback was called again after last consumption of it.\n");

  DALI_TEST_EQUALS( gIsCalledButtonCallback, true, TEST_LOCATION );

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_EQUALS( gIsCalledButtonCallback, true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonInitialAutoRepeatingDelayPropertyP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  button.SetProperty( button.GetPropertyIndex("initialAutoRepeatingDelay"), 0.5f );

  DALI_TEST_EQUALS( button.GetProperty<float>( button.GetPropertyIndex("initialAutoRepeatingDelay")), 0.5f, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("initialAutoRepeatingDelay"), 0.2f );

  DALI_TEST_EQUALS( button.GetProperty<float>( button.GetPropertyIndex("initialAutoRepeatingDelay")), 0.2f, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonNextAutoRepeatingDelayPropertyP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  button.SetProperty( button.GetPropertyIndex("nextAutoRepeatingDelay"), 0.5f );

  DALI_TEST_EQUALS( button.GetProperty<float>( button.GetPropertyIndex("nextAutoRepeatingDelay")), 0.5f, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("nextAutoRepeatingDelay"), 0.2f );

  DALI_TEST_EQUALS( button.GetProperty<float>( button.GetPropertyIndex("nextAutoRepeatingDelay")), 0.2f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonTogglableButtonPropertyP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  button.SetProperty( button.GetPropertyIndex("togglable"), true );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("togglable")), true, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("togglable"), false );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("togglable")), false, TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonSelectedPropertyP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();
  button.SetProperty( button.GetPropertyIndex("togglable"), true );

  button.SetProperty( button.GetPropertyIndex("selected"), true );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("selected")), true, TEST_LOCATION );

  button.SetProperty( button.GetPropertyIndex("selected"), false );

  DALI_TEST_EQUALS( button.GetProperty<bool>( button.GetPropertyIndex("selected")), false, TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonSetLabelStringWithPropertyMapP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();
  button.SetProperty( Toolkit::Button::Property::LABEL,
                      Property::Map().Add( Toolkit::Visual::Property::TYPE, Toolkit::Visual::TEXT )
                                     .Add( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f )
                                     .Add( Toolkit::TextVisual::Property::TEXT, "Button Label")
                     );

  DALI_TEST_EQUALS( GetButtonText( button ), "Button Label", TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonSetLabelStringWithPropertyMapStringsP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  tet_infoline(" UtcDaliButtonSetLabelStringWithPropertyMapStringsP Setting Button text using String then replacing with Enum then string");

  Property::Map textVisualMapInitial;
  textVisualMapInitial["visualType"] = "TEXT";
  textVisualMapInitial["pointSize"] =  15.0f;
  textVisualMapInitial["text"] = "button label initial";

  button.SetProperty( Button::Property::LABEL, textVisualMapInitial );

  DALI_TEST_EQUALS( GetButtonText( button ), "button label initial", TEST_LOCATION );

  tet_infoline(" UtcDaliButtonSetLabelStringWithPropertyMapStringsP Intermediate part of test");

  Property::Map propertyMap;
  propertyMap.Insert( Toolkit::Visual::Property::TYPE,  Toolkit::Visual::TEXT );
  propertyMap.Insert( Toolkit::TextVisual::Property::TEXT,  "error if this is the final text" );
  propertyMap.Insert( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f );

  button.SetProperty( Button::Property::LABEL, propertyMap );

  DALI_TEST_EQUALS( GetButtonText( button ), "error if this is the final text", TEST_LOCATION );

  tet_infoline(" UtcDaliButtonSetLabelStringWithPropertyMapStringsP Final part of test");

  Property::Map textVisualMap;
  textVisualMap["visualType"] = "TEXT";
  textVisualMap["pointSize"] =  15.0f;
  textVisualMap["text"] = "Button Label";

  button.SetProperty( Toolkit::Button::Property::LABEL, textVisualMap );

  DALI_TEST_EQUALS( GetButtonText( button ), "Button Label", TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonSetLabelWithStringP(void)
{
  ToolkitTestApplication application;

  Button button = PushButton::New();

  // Set default point size for text visual as style sheet not available.
  button.SetProperty( Toolkit::Button::Property::LABEL,
                      Property::Map().Add( Toolkit::Visual::Property::TYPE, Toolkit::Visual::TEXT )
                                     .Add( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f )
                                     );

  button.SetProperty( Toolkit::Button::Property::LABEL, "Button Label" );

  DALI_TEST_EQUALS( GetButtonText( button ), "Button Label", TEST_LOCATION );
  END_TEST;
}

int UtcDaliButtonSetLabelPropertyP(void)
{
  ToolkitTestApplication application;

  tet_infoline(" UtcDaliButtonSetLabelPropertyP Set text label and then set again with new text");


  const std::string TEST_LABEL1 = "test label one";
  const std::string TEST_LABEL2 = "test label two";

  Button button = PushButton::New();

  button.SetProperty( Toolkit::Button::Property::LABEL,
                        Property::Map().Add( Toolkit::Visual::Property::TYPE, Toolkit::Visual::TEXT )
                                       .Add( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f )
                                       .Add( Toolkit::TextVisual::Property::TEXT, TEST_LABEL1 )
                     );

  DALI_TEST_EQUALS( GetButtonText( button ), TEST_LABEL1,  TEST_LOCATION );

  Property::Map propertyMap;
  propertyMap.Insert( Toolkit::Visual::Property::TYPE,  Toolkit::Visual::TEXT );
  propertyMap.Insert( Toolkit::TextVisual::Property::TEXT,  TEST_LABEL2 );
  propertyMap.Insert( Toolkit::TextVisual::Property::TEXT_COLOR, Color::BLUE );
  propertyMap.Insert( Toolkit::TextVisual::Property::POINT_SIZE, 15.0f );
  button.SetProperty( Button::Property::LABEL, propertyMap );

  DALI_TEST_EQUALS( GetButtonText( button ), TEST_LABEL2,  TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonPressedSignalP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliButtonPressedSignalP");

  Button button = PushButton::New();
  button.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  button.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  button.SetProperty( Actor::Property::POSITION, Vector2( 240, 400 ));
  button.SetProperty( Actor::Property::SIZE, Vector2( 100, 100 ) );

  application.GetScene().Add( button );

  application.SendNotification();
  application.Render();

  // connect to its touch signal
  ConnectionTracker* testTracker = new ConnectionTracker();
  button.PressedSignal().Connect( &ButtonCallback );
  button.ReleasedSignal().Connect( &ButtonCallback );
  bool pressedSignal = false;
  bool releasedSignal = false;
  button.ConnectSignal( testTracker, "pressed",   CallbackFunctor(&pressedSignal) );
  button.ConnectSignal( testTracker, "released",  CallbackFunctor(&releasedSignal) );

  Dali::Integration::TouchEvent event;

  // Test1. Touch point down and up inside the button.

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gIsCalledButtonCallback );
  DALI_TEST_CHECK( pressedSignal );

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gIsCalledButtonCallback );
  DALI_TEST_CHECK( releasedSignal );

  // Test2. Touch point down and up outside the button.

  pressedSignal = false;
  releasedSignal = false;
  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );
  DALI_TEST_CHECK( !pressedSignal );

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );
  DALI_TEST_CHECK( !releasedSignal );

  // Test3. Touch point down inside and up outside the button.

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gIsCalledButtonCallback );

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointLeave() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gIsCalledButtonCallback );

  // Test4. Touch point down outside and up inside the button.

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointEnter() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );
  END_TEST;
}

int UtcDaliButtonClickedSignalP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliButtonClickedSignalP");

  Button button = PushButton::New();
  button.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  button.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  button.SetProperty( Actor::Property::POSITION, Vector2( 240, 400 ));
  button.SetProperty( Actor::Property::SIZE, Vector2( 100, 100 ) );

  application.GetScene().Add( button );

  application.SendNotification();
  application.Render();

  // connect to its touch signal
  button.ClickedSignal().Connect( &ButtonCallback );
  bool clickedSignal = false;
  ConnectionTracker* testTracker = new ConnectionTracker();
  button.ConnectSignal( testTracker, "clicked",   CallbackFunctor(&clickedSignal) );

  Dali::Integration::TouchEvent event;

  // Test1. Touch point down and up inside the button.

  gIsCalledButtonCallback = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( gIsCalledButtonCallback );
  DALI_TEST_CHECK( clickedSignal );

  // Test2. Touch point down and up outside the button.

  gIsCalledButtonCallback = false;
  clickedSignal = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );
  DALI_TEST_CHECK( !clickedSignal );

  // Test3. Touch point down inside and up outside the button.

  gIsCalledButtonCallback = false;
  clickedSignal = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownInside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointLeave() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpOutside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );
  DALI_TEST_CHECK( !clickedSignal );

  // Test4. Touch point down outside and up inside the button.

  gIsCalledButtonCallback = false;
  clickedSignal = false;
  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointDownOutside() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointEnter() );
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  event.AddPoint( GetPointUpInside() );
  application.ProcessEvent( event );

  DALI_TEST_CHECK( !gIsCalledButtonCallback );
  DALI_TEST_CHECK( !clickedSignal );
  END_TEST;
}

int UtcDaliButtonStateChangedSignalP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliButtonStateChangedSignalP");

  Button button = PushButton::New();

  button.SetProperty( Button::Property::TOGGLABLE, true);

  application.GetScene().Add( button );

  application.SendNotification();
  application.Render();

  // connect to its signal
  button.StateChangedSignal().Connect( &ButtonCallback );
  bool stateChangedSignal = false;
  ConnectionTracker* testTracker = new ConnectionTracker();
  button.ConnectSignal( testTracker, "stateChanged",   CallbackFunctor(&stateChangedSignal) );

  gIsCalledButtonCallback = false;
  button.SetProperty( Button::Property::SELECTED, true);

  DALI_TEST_CHECK( gIsCalledButtonCallback );
  DALI_TEST_CHECK( stateChangedSignal );

  gIsCalledButtonCallback = false;
  stateChangedSignal = false;

  button.SetProperty( Button::Property::SELECTED, false);
  DALI_TEST_CHECK( gIsCalledButtonCallback );
  DALI_TEST_CHECK( stateChangedSignal );
  END_TEST;
}

int UtcDaliButtonSetProperty(void)
{
  tet_infoline("UtcDaliButtonSetProperty: ");
  ToolkitTestApplication application;

  PushButton pushButton = PushButton::New();

  pushButton.SetProperty(pushButton.GetPropertyIndex("disabled"), false);

  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( pushButton.GetPropertyIndex("disabled")), false, TEST_LOCATION );

  pushButton.SetProperty(pushButton.GetPropertyIndex("disabled"), true);
  DALI_TEST_EQUALS( pushButton.GetProperty<bool>( pushButton.GetPropertyIndex("disabled")), true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonEventConsumption(void)
{
  /**
   *  [ Parent ]
   *  [ Child  ]
   *
   *  Child parented and positioned under parent.
   *  Touch up and down performed on child.
   *  Should only trigger signal on child.
   */

  ToolkitTestApplication application;

  Button parentButton = PushButton::New();
  parentButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  parentButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  parentButton.SetProperty( Actor::Property::SIZE, Vector2( 20, 20 ) );
  application.GetScene().Add( parentButton );

  Button childButton = PushButton::New();
  childButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  childButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_LEFT );
  childButton.SetProperty( Actor::Property::SIZE, Vector2( 20, 20 ) );
  parentButton.Add( childButton );

  // Reset signal flags
  gIsCalledChildButtonCallback = false;
  gIsCalledButtonCallback = false;

  parentButton.ClickedSignal().Connect( &ButtonCallback );
  childButton.ClickedSignal().Connect( &ChildButtonCallback );

  // Peform a button click at coordinates (10,30) which is the child.
  Dali::Integration::TouchEvent event;
  event = Dali::Integration::TouchEvent();
  Dali::Integration::Point point;
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 10, 30 ) );
  event.AddPoint( point );
  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  point.SetState( PointState::UP );
  point.SetScreenPosition( Vector2( 10, 30 ) );
  event.AddPoint( point );
  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( event );

  DALI_TEST_EQUALS( gIsCalledChildButtonCallback, true, TEST_LOCATION );
  DALI_TEST_EQUALS( ! gIsCalledButtonCallback, true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonRelease(void)
{
  /**
   * Down event followed by interrupted event should signal Release.
   */

  ToolkitTestApplication application;

  Button parentButton = PushButton::New();
  parentButton.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  parentButton.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  parentButton.SetProperty( Actor::Property::SIZE, Vector2( 20, 20 ) );
  application.GetScene().Add( parentButton );
  parentButton.ReleasedSignal().Connect( &ButtonCallback );

  // Reset signal flags
  gIsCalledButtonCallback = false;

  // Peform a button down and then button interrupted at coordinates (10,10).
  Dali::Integration::TouchEvent event;
  event = Dali::Integration::TouchEvent();
  Dali::Integration::Point point;
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 10, 10 ) );
  event.AddPoint( point );
  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( event );

  event = Dali::Integration::TouchEvent();
  point.SetState( PointState::INTERRUPTED );
  event.AddPoint( point );
  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( event );

  DALI_TEST_EQUALS( gIsCalledButtonCallback, true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliButtonMultiTouch(void)
{
  /**
   * Down event followed by a multi touch point event should signal Release.
   */

  ToolkitTestApplication application;

  Button button = PushButton::New();
  button.SetProperty( Button::Property::TOGGLABLE, true);

  button.SetProperty( Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT );
  button.SetProperty( Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT );
  button.SetProperty( Actor::Property::SIZE, Vector2( 20, 20 ) );
  application.GetScene().Add( button );
  button.ReleasedSignal().Connect( &ButtonCallback );

  // Reset signal flags
  gIsCalledButtonCallback = false;

  // Peform a button down and then button interrupted at coordinates (10,10).
  Dali::Integration::TouchEvent downEvent;
  downEvent = Dali::Integration::TouchEvent();
  Dali::Integration::Point point;

  // Add Press button
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 15, 15 ) );
  downEvent.AddPoint( point );
  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( downEvent );

  // Release button
  Dali::Integration::TouchEvent upEvent;
  upEvent = Dali::Integration::TouchEvent();
  point.SetState( PointState::UP );
  point.SetScreenPosition( Vector2( 15, 15 ) );
  upEvent.AddPoint( point );
  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( upEvent );

  tet_infoline("Button should now be selected\n");
  bool isSelected = button.GetProperty<bool>( Button::Property::SELECTED ) ;
  DALI_TEST_EQUALS( isSelected, true, TEST_LOCATION );

  // Add first point
  Dali::Integration::TouchEvent multiEvent;
  multiEvent = Dali::Integration::TouchEvent();
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 10, 10 ) );
  multiEvent.AddPoint( point );

  // Add second point
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 15, 15 ) );
  multiEvent.AddPoint( point );

  tet_infoline("Before a multi touch event\n");

  // flush the queue and render once
  application.SendNotification();
  application.Render();
  application.ProcessEvent( multiEvent );

  DALI_TEST_EQUALS( gIsCalledButtonCallback, true, TEST_LOCATION );

  END_TEST;
}
