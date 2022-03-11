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

#include <dali-toolkit-test-suite-utils.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <stdlib.h>
#include <iostream>

using namespace Dali;
using namespace Dali::Toolkit;

void dali_radio_button_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_radio_button_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{
static bool gObjectCreatedCallBackCalled;

static void TestCallback(BaseHandle handle)
{
  gObjectCreatedCallBackCalled = true;
}

static std::string GetButtonText(Button button)
{
  Property::Value value = button.GetProperty(Toolkit::Button::Property::LABEL);

  Property::Map* labelProperty = value.GetMap();

  std::string textLabel;

  if(labelProperty)
  {
    Property::Value* value = labelProperty->Find(Toolkit::TextVisual::Property::TEXT);
    value->Get(textLabel);
  }

  return textLabel;
}

} // namespace

int UtcDaliRadioButtonConstructorP(void)
{
  ToolkitTestApplication application;

  RadioButton button;

  DALI_TEST_CHECK(!button);
  END_TEST;
}

int UtcDaliRadioButtonCopyConstructorP(void)
{
  ToolkitTestApplication application;

  // Initialize an object, ref count == 1
  RadioButton button = RadioButton::New();

  RadioButton copy(button);
  DALI_TEST_CHECK(copy);
  END_TEST;
}

int UtcDaliRadioButtonMoveConstructor(void)
{
  ToolkitTestApplication application;

  RadioButton button = RadioButton::New();
  DALI_TEST_EQUALS(1, button.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  DALI_TEST_EQUALS(button.GetProperty<bool>(Button::Property::TOGGLABLE), true, TEST_LOCATION);
  button.SetProperty(Button::Property::TOGGLABLE, false);
  DALI_TEST_EQUALS(button.GetProperty<bool>(Button::Property::TOGGLABLE), false, TEST_LOCATION);

  RadioButton moved = std::move(button);
  DALI_TEST_CHECK(moved);
  DALI_TEST_EQUALS(1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  DALI_TEST_EQUALS(moved.GetProperty<bool>(Button::Property::TOGGLABLE), false, TEST_LOCATION);
  DALI_TEST_CHECK(!button);

  END_TEST;
}

int UtcDaliRadioButtonAssignmentOperatorP(void)
{
  ToolkitTestApplication application;

  RadioButton button = RadioButton::New();

  RadioButton copy(button);
  DALI_TEST_CHECK(copy);

  DALI_TEST_CHECK(button == copy);
  END_TEST;
}

int UtcDaliRadioButtonMoveAssignment(void)
{
  ToolkitTestApplication application;

  RadioButton button = RadioButton::New();
  DALI_TEST_EQUALS(1, button.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  DALI_TEST_EQUALS(button.GetProperty<bool>(Button::Property::TOGGLABLE), true, TEST_LOCATION);
  button.SetProperty(Button::Property::TOGGLABLE, false);
  DALI_TEST_EQUALS(button.GetProperty<bool>(Button::Property::TOGGLABLE), false, TEST_LOCATION);

  RadioButton moved;
  moved = std::move(button);
  DALI_TEST_CHECK(moved);
  DALI_TEST_EQUALS(1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  DALI_TEST_EQUALS(moved.GetProperty<bool>(Button::Property::TOGGLABLE), false, TEST_LOCATION);
  DALI_TEST_CHECK(!button);

  END_TEST;
}

int UtcDaliRadioButtonNewP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliRadioButtonNewP");

  // Create the Slider actor
  RadioButton radioButton;

  DALI_TEST_CHECK(!radioButton);

  radioButton = RadioButton::New();

  DALI_TEST_CHECK(radioButton);

  RadioButton radioButton2(radioButton);

  DALI_TEST_CHECK(radioButton2 == radioButton);

  //Additional check to ensure object is created by checking if it's registered
  ObjectRegistry registry = application.GetCore().GetObjectRegistry();
  DALI_TEST_CHECK(registry);

  gObjectCreatedCallBackCalled = false;
  registry.ObjectCreatedSignal().Connect(&TestCallback);
  {
    RadioButton radioButton = RadioButton::New();
  }
  DALI_TEST_CHECK(gObjectCreatedCallBackCalled);
  END_TEST;
}

int UtcDaliRadioButtonDestructorP(void)
{
  ToolkitTestApplication application;

  RadioButton* radioButton = new RadioButton();
  delete radioButton;

  DALI_TEST_CHECK(true);
  END_TEST;
}

int UtcDaliRadioButtonDownCast(void)
{
  ToolkitTestApplication application;

  Handle handle = RadioButton::New();

  RadioButton radioButton = RadioButton::DownCast(handle);

  DALI_TEST_CHECK(radioButton == handle);
  END_TEST;
}

int UtcDaliRadioButtonLabelProperty(void)
{
  ToolkitTestApplication application;

  const std::string labelText = "test actor 1";

  RadioButton radioButton = RadioButton::New();

  radioButton.SetProperty(Toolkit::Button::Property::LABEL,
                          Property::Map().Add(Toolkit::Visual::Property::TYPE, Toolkit::Visual::TEXT).Add(Toolkit::TextVisual::Property::POINT_SIZE, 15.0f));

  radioButton.SetProperty(Toolkit::Button::Property::LABEL, labelText);
  DALI_TEST_EQUALS(GetButtonText(radioButton), labelText, TEST_LOCATION);

  std::string labelText2 = "test actor 2";
  radioButton.SetProperty(Toolkit::Button::Property::LABEL, labelText2);

  DALI_TEST_EQUALS(GetButtonText(radioButton), labelText2, TEST_LOCATION);

  END_TEST;
}

int UtcDaliRadioButtonSelectedProperty(void)
{
  ToolkitTestApplication application; // Exceptions require ToolkitTestApplication
  tet_infoline(" UtcDaliRadioButtonSelectedProperty");

  // Create the RadioButton actor
  RadioButton radioButton = RadioButton::New();
  application.GetScene().Add(radioButton);
  radioButton.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  radioButton.SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::TOP_LEFT);
  radioButton.SetProperty(Actor::Property::POSITION, Vector2(0.0f, 0.0f));

  // Default selected
  DALI_TEST_CHECK(radioButton.GetProperty<bool>(Button::Property::SELECTED) == false);

  // Setting false selected
  radioButton.SetProperty(Button::Property::SELECTED, false);
  DALI_TEST_CHECK(radioButton.GetProperty<bool>(Button::Property::SELECTED) == false);

  // Setting true selected
  radioButton.SetProperty(Button::Property::SELECTED, true);
  DALI_TEST_CHECK(radioButton.GetProperty<bool>(Button::Property::SELECTED) == true);

  // Setting false again
  radioButton.SetProperty(Button::Property::SELECTED, false);
  DALI_TEST_CHECK(radioButton.GetProperty<bool>(Button::Property::SELECTED) == false);

  // Test selecting radio buttons
  RadioButton radioButton2 = RadioButton::New("label");
  radioButton2.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  radioButton2.SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::TOP_LEFT);
  radioButton2.SetProperty(Actor::Property::POSITION, Vector2(0.0f, 0.0f));

  RadioButton radioButton3 = RadioButton::New("label");
  radioButton3.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  radioButton3.SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::TOP_LEFT);
  radioButton3.SetProperty(Actor::Property::POSITION, Vector2(0.0f, 40.0f));

  Actor radioGroup = Actor::New();
  application.GetScene().Add(radioGroup);
  radioGroup.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  radioGroup.SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::TOP_LEFT);
  radioGroup.SetProperty(Actor::Property::POSITION, Vector2(0.0f, 0.0f));
  radioGroup.SetProperty(Actor::Property::SIZE, Vector2(400.0f, 400.0));

  radioGroup.Add(radioButton2);
  radioGroup.Add(radioButton3);

  application.SendNotification();
  application.Render();

  // Simulate touch events
  DALI_TEST_CHECK(radioButton2.GetProperty<bool>(Button::Property::SELECTED) == false);
  DALI_TEST_CHECK(radioButton3.GetProperty<bool>(Button::Property::SELECTED) == false);

  // Select first radio
  {
    Dali::Integration::TouchEvent event1 = Dali::Integration::TouchEvent();
    Dali::Integration::TouchEvent event2 = Dali::Integration::TouchEvent();

    Dali::Integration::Point pointDown;
    pointDown.SetState(PointState::DOWN);
    pointDown.SetScreenPosition(Vector2(1.0f, 1.0f));

    Dali::Integration::Point pointUp(pointDown);
    pointUp.SetState(PointState::UP);

    event1.AddPoint(pointDown);
    application.ProcessEvent(event1);

    event2.AddPoint(pointUp);
    application.ProcessEvent(event2);

    application.SendNotification();
    application.Render();

    DALI_TEST_CHECK(radioButton2.GetProperty<bool>(Button::Property::SELECTED) == true);
    DALI_TEST_CHECK(radioButton3.GetProperty<bool>(Button::Property::SELECTED) == false);
  }

  // Select an already selected radio
  {
    Dali::Integration::TouchEvent event1 = Dali::Integration::TouchEvent();
    Dali::Integration::TouchEvent event2 = Dali::Integration::TouchEvent();

    Dali::Integration::Point pointDown;
    pointDown.SetState(PointState::DOWN);
    pointDown.SetScreenPosition(Vector2(1.0f, 1.0f));

    Dali::Integration::Point pointUp(pointDown);
    pointUp.SetState(PointState::UP);

    event1.AddPoint(pointDown);
    application.ProcessEvent(event1);

    event2.AddPoint(pointUp);
    application.ProcessEvent(event2);

    application.SendNotification();
    application.Render();

    DALI_TEST_CHECK(radioButton2.GetProperty<bool>(Button::Property::SELECTED) == true);
    DALI_TEST_CHECK(radioButton3.GetProperty<bool>(Button::Property::SELECTED) == false);
  }

  // Select second radio
  {
    Dali::Integration::TouchEvent event1 = Dali::Integration::TouchEvent();
    Dali::Integration::TouchEvent event2 = Dali::Integration::TouchEvent();

    Dali::Integration::Point pointDown;
    pointDown.SetState(PointState::DOWN);
    pointDown.SetScreenPosition(Vector2(1.0f, 41.0f));

    Dali::Integration::Point pointUp(pointDown);
    pointUp.SetState(PointState::UP);

    event1.AddPoint(pointDown);
    application.ProcessEvent(event1);

    event2.AddPoint(pointUp);
    application.ProcessEvent(event2);

    application.SendNotification();
    application.Render();

    DALI_TEST_CHECK(radioButton2.GetProperty<bool>(Button::Property::SELECTED) == false);
    DALI_TEST_CHECK(radioButton3.GetProperty<bool>(Button::Property::SELECTED) == true);
  }

  // Select outside radio group
  {
    Dali::Integration::TouchEvent event1 = Dali::Integration::TouchEvent();
    Dali::Integration::TouchEvent event2 = Dali::Integration::TouchEvent();

    Dali::Integration::Point pointDown;
    pointDown.SetState(PointState::DOWN);
    pointDown.SetScreenPosition(Vector2(1.0f, 500.0f));

    Dali::Integration::Point pointUp(pointDown);
    pointUp.SetState(PointState::UP);

    event1.AddPoint(pointDown);
    application.ProcessEvent(event1);

    event2.AddPoint(pointUp);
    application.ProcessEvent(event2);

    application.SendNotification();
    application.Render();

    DALI_TEST_CHECK(radioButton2.GetProperty<bool>(Button::Property::SELECTED) == false);
    DALI_TEST_CHECK(radioButton3.GetProperty<bool>(Button::Property::SELECTED) == true);
  }

  END_TEST;
}
