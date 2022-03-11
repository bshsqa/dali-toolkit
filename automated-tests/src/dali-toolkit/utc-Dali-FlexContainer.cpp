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

#include <stdlib.h>
#include <iostream>

#include <dali-toolkit-test-suite-utils.h>

#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/actors/actor-devel.h>

using namespace Dali;
using namespace Toolkit;

void dali_flexflexContainer_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_flexflexContainer_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{
const char* const PROPERTY_NAME_CONTENT_DIRECTION = "contentDirection";
const char* const PROPERTY_NAME_FLEX_DIRECTION    = "flexDirection";
const char* const PROPERTY_NAME_FLEX_WRAP         = "flexWrap";
const char* const PROPERTY_NAME_JUSTIFY_CONTENT   = "justifyContent";
const char* const PROPERTY_NAME_ALIGN_ITEMS       = "alignItems";
const char* const PROPERTY_NAME_ALIGN_CONTENT     = "alignContent";
const char* const CHILD_PROPERTY_NAME_FLEX        = "flex";
const char* const CHILD_PROPERTY_NAME_ALIGN_SELF  = "alignSelf";
const char* const CHILD_PROPERTY_NAME_FLEX_MARGIN = "flexMargin";

} // namespace

int UtcDaliToolkitFlexContainerConstructorP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerConstructorP");
  FlexContainer flexContainer;
  DALI_TEST_CHECK(!flexContainer);
  END_TEST;
}

int UtcDaliToolkitFlexContainerNewP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerNewP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);
  END_TEST;
}

int UtcDaliToolkitFlexContainerDownCastP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerDownCastP");
  FlexContainer flexContainer1 = FlexContainer::New();
  BaseHandle    object(flexContainer1);

  FlexContainer flexContainer2 = FlexContainer::DownCast(object);
  DALI_TEST_CHECK(flexContainer2);

  FlexContainer flexContainer3 = DownCast<FlexContainer>(object);
  DALI_TEST_CHECK(flexContainer3);
  END_TEST;
}

int UtcDaliToolkitFlexContainerDownCastN(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerDownCastN");
  BaseHandle    uninitializedObject;
  FlexContainer flexContainer1 = FlexContainer::DownCast(uninitializedObject);
  DALI_TEST_CHECK(!flexContainer1);

  FlexContainer flexContainer2 = DownCast<FlexContainer>(uninitializedObject);
  DALI_TEST_CHECK(!flexContainer2);
  END_TEST;
}

int UtcDaliToolkitFlexContainerCopyConstructorP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerCopyConstructorP");
  FlexContainer flexContainer = FlexContainer::New();
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, FlexContainer::ROW_REVERSE);

  FlexContainer copy(flexContainer);
  DALI_TEST_CHECK(copy);
  DALI_TEST_CHECK(copy.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION) == flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION));

  END_TEST;
}

int UtcDaliFlexContainerMoveConstructor(void)
{
  ToolkitTestApplication application;

  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_EQUALS(1, flexContainer.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, FlexContainer::ROW_REVERSE);
  DALI_TEST_CHECK(flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION) == FlexContainer::ROW_REVERSE);

  FlexContainer moved = std::move(flexContainer);
  DALI_TEST_CHECK(moved);
  DALI_TEST_EQUALS(1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  DALI_TEST_CHECK(moved.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION) == FlexContainer::ROW_REVERSE);
  DALI_TEST_CHECK(!flexContainer);

  END_TEST;
}

int UtcDaliToolkitFlexContainerAssignmentOperatorP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerAssingmentOperatorP");
  FlexContainer flexContainer = FlexContainer::New();
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, FlexContainer::ROW_REVERSE);

  FlexContainer copy = flexContainer;
  DALI_TEST_CHECK(copy);
  DALI_TEST_CHECK(copy.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION) == flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION));
  END_TEST;
}

int UtcDaliFlexContainerMoveAssignment(void)
{
  ToolkitTestApplication application;

  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_EQUALS(1, flexContainer.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, FlexContainer::ROW_REVERSE);
  DALI_TEST_CHECK(flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION) == FlexContainer::ROW_REVERSE);

  FlexContainer moved;
  moved = std::move(flexContainer);
  DALI_TEST_CHECK(moved);
  DALI_TEST_EQUALS(1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION);
  DALI_TEST_CHECK(moved.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION) == FlexContainer::ROW_REVERSE);
  DALI_TEST_CHECK(!flexContainer);

  END_TEST;
}

// Positive test case for a method
int UtcDaliToolkitFlexContainerGetPropertyP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerGetPropertyP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  // Check Property Indices are correct
  DALI_TEST_CHECK(flexContainer.GetPropertyIndex(PROPERTY_NAME_CONTENT_DIRECTION) == FlexContainer::Property::CONTENT_DIRECTION);
  DALI_TEST_CHECK(flexContainer.GetPropertyIndex(PROPERTY_NAME_FLEX_DIRECTION) == FlexContainer::Property::FLEX_DIRECTION);
  DALI_TEST_CHECK(flexContainer.GetPropertyIndex(PROPERTY_NAME_FLEX_WRAP) == FlexContainer::Property::FLEX_WRAP);
  DALI_TEST_CHECK(flexContainer.GetPropertyIndex(PROPERTY_NAME_JUSTIFY_CONTENT) == FlexContainer::Property::JUSTIFY_CONTENT);
  DALI_TEST_CHECK(flexContainer.GetPropertyIndex(PROPERTY_NAME_ALIGN_ITEMS) == FlexContainer::Property::ALIGN_ITEMS);
  DALI_TEST_CHECK(flexContainer.GetPropertyIndex(PROPERTY_NAME_ALIGN_CONTENT) == FlexContainer::Property::ALIGN_CONTENT);

  END_TEST;
}

int UtcDaliToolkitFlexContainerSetPropertyP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerSetPropertyP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  // Add flex container to the stage
  application.GetScene().Add(flexContainer);

  // Create two actors and add them to the container
  Actor actor1 = Actor::New();
  Actor actor2 = Actor::New();
  DALI_TEST_CHECK(actor1);
  DALI_TEST_CHECK(actor2);

  flexContainer.Add(actor1);
  flexContainer.Add(actor2);

  // Check content direction property.
  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, FlexContainer::RTL);
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::RTL, TEST_LOCATION);

  // Check flex direction property.
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, FlexContainer::COLUMN_REVERSE);
  DALI_TEST_EQUALS((FlexContainer::FlexDirection)flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION), FlexContainer::COLUMN_REVERSE, TEST_LOCATION);

  // Check flex wrap property.
  flexContainer.SetProperty(FlexContainer::Property::FLEX_WRAP, FlexContainer::WRAP);
  DALI_TEST_EQUALS((FlexContainer::WrapType)flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_WRAP), FlexContainer::WRAP, TEST_LOCATION);

  // Check justify content property.
  flexContainer.SetProperty(FlexContainer::Property::JUSTIFY_CONTENT, FlexContainer::JUSTIFY_SPACE_BETWEEN);
  DALI_TEST_EQUALS((FlexContainer::Justification)flexContainer.GetProperty<int>(FlexContainer::Property::JUSTIFY_CONTENT), FlexContainer::JUSTIFY_SPACE_BETWEEN, TEST_LOCATION);

  // Check align items property.
  flexContainer.SetProperty(FlexContainer::Property::ALIGN_ITEMS, FlexContainer::ALIGN_FLEX_START);
  DALI_TEST_EQUALS((FlexContainer::Alignment)flexContainer.GetProperty<int>(FlexContainer::Property::ALIGN_ITEMS), FlexContainer::ALIGN_FLEX_START, TEST_LOCATION);

  // Check align content property.
  flexContainer.SetProperty(FlexContainer::Property::ALIGN_CONTENT, FlexContainer::ALIGN_STRETCH);
  DALI_TEST_EQUALS((FlexContainer::Alignment)flexContainer.GetProperty<int>(FlexContainer::Property::ALIGN_CONTENT), FlexContainer::ALIGN_STRETCH, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitFlexContainerSetPropertyEnumP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerSetPropertyEnumP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  // Add flex container to the stage
  application.GetScene().Add(flexContainer);

  // Create two actors and add them to the container
  Actor actor1 = Actor::New();
  Actor actor2 = Actor::New();
  DALI_TEST_CHECK(actor1);
  DALI_TEST_CHECK(actor2);

  flexContainer.Add(actor1);
  flexContainer.Add(actor2);

  // Check content direction property.
  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "RTL");
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::RTL, TEST_LOCATION);

  // Check flex direction property.
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, "columnReverse");
  DALI_TEST_EQUALS((FlexContainer::FlexDirection)flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION), FlexContainer::COLUMN_REVERSE, TEST_LOCATION);

  // Check flex wrap property.
  flexContainer.SetProperty(FlexContainer::Property::FLEX_WRAP, "wrap");
  DALI_TEST_EQUALS((FlexContainer::WrapType)flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_WRAP), FlexContainer::WRAP, TEST_LOCATION);

  // Check justify content property.
  flexContainer.SetProperty(FlexContainer::Property::JUSTIFY_CONTENT, "spaceBetween");
  DALI_TEST_EQUALS((FlexContainer::Justification)flexContainer.GetProperty<int>(FlexContainer::Property::JUSTIFY_CONTENT), FlexContainer::JUSTIFY_SPACE_BETWEEN, TEST_LOCATION);

  // Check align items property.
  flexContainer.SetProperty(FlexContainer::Property::ALIGN_ITEMS, "flexStart");
  DALI_TEST_EQUALS((FlexContainer::Alignment)flexContainer.GetProperty<int>(FlexContainer::Property::ALIGN_ITEMS), FlexContainer::ALIGN_FLEX_START, TEST_LOCATION);

  // Check align content property.
  flexContainer.SetProperty(FlexContainer::Property::ALIGN_CONTENT, "stretch");
  DALI_TEST_EQUALS((FlexContainer::Alignment)flexContainer.GetProperty<int>(FlexContainer::Property::ALIGN_CONTENT), FlexContainer::ALIGN_STRETCH, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitFlexContainerSetChildPropertyP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerSetChildPropertyP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  // Add flex container to the stage
  application.GetScene().Add(flexContainer);

  // Create an actor and add it to the container
  Actor actor = Actor::New();
  DALI_TEST_CHECK(actor);

  flexContainer.Add(actor);

  // Check flex child property.
  actor.SetProperty(FlexContainer::ChildProperty::FLEX, 2.0f);
  DALI_TEST_EQUALS(actor.GetProperty<float>(FlexContainer::ChildProperty::FLEX), 2.0f, TEST_LOCATION);
  DALI_TEST_CHECK(actor.GetPropertyIndex(CHILD_PROPERTY_NAME_FLEX) == FlexContainer::ChildProperty::FLEX);

  // Check align self child property.
  actor.SetProperty(FlexContainer::ChildProperty::ALIGN_SELF, FlexContainer::ALIGN_FLEX_END);
  DALI_TEST_EQUALS((FlexContainer::Alignment)actor.GetProperty<int>(FlexContainer::ChildProperty::ALIGN_SELF), FlexContainer::ALIGN_FLEX_END, TEST_LOCATION);
  DALI_TEST_CHECK(actor.GetPropertyIndex(CHILD_PROPERTY_NAME_ALIGN_SELF) == FlexContainer::ChildProperty::ALIGN_SELF);

  // Check flex margin child property.
  actor.SetProperty(FlexContainer::ChildProperty::FLEX_MARGIN, Vector4(10.0f, 10.0f, 10.0f, 10.0f));
  DALI_TEST_EQUALS(actor.GetProperty<Vector4>(FlexContainer::ChildProperty::FLEX_MARGIN), Vector4(10.0f, 10.0f, 10.0f, 10.0f), TEST_LOCATION);
  DALI_TEST_CHECK(actor.GetPropertyIndex(CHILD_PROPERTY_NAME_FLEX_MARGIN) == FlexContainer::ChildProperty::FLEX_MARGIN);

  application.SendNotification();
  application.Render();

  END_TEST;
}

//Functor to test whether RelayoutSignal is emitted
class RelayoutSignalHandler : public Dali::ConnectionTracker
{
public:
  RelayoutSignalHandler(FlexContainer& actor)
  : mSignalVerified(false),
    mActor(actor)
  {
  }

  // callback to be connected to RelayoutSignal
  void RelayoutCallback(Actor actor)
  {
    if(mActor == actor)
    {
      mSignalVerified = true;
    }
  }

  void Reset()
  {
    mSignalVerified = false;
  }

  bool   mSignalVerified;
  Actor& mActor;
};

int UtcDaliToolkitFlexContainerRemoveChildP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerSetPropertyP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  // Add flex container to the stage
  application.GetScene().Add(flexContainer);

  RelayoutSignalHandler relayoutSignal(flexContainer);
  flexContainer.OnRelayoutSignal().Connect(&relayoutSignal, &RelayoutSignalHandler::RelayoutCallback);

  // Create two actors and add them to the container
  Actor actor1 = Actor::New();
  Actor actor2 = Actor::New();
  DALI_TEST_CHECK(actor1);
  DALI_TEST_CHECK(actor2);

  flexContainer.Add(actor1);
  flexContainer.Add(actor2);

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS(relayoutSignal.mSignalVerified, true, TEST_LOCATION);
  relayoutSignal.Reset();

  DALI_TEST_CHECK(actor1);
  DALI_TEST_CHECK(actor2);

  flexContainer.Remove(actor1);
  application.SendNotification();
  application.Render();
  DALI_TEST_EQUALS(relayoutSignal.mSignalVerified, true, TEST_LOCATION);
  relayoutSignal.Reset();

  flexContainer.Remove(actor2);
  application.SendNotification();
  application.Render();
  DALI_TEST_EQUALS(relayoutSignal.mSignalVerified, true, TEST_LOCATION);
  relayoutSignal.Reset();

  END_TEST;
}

namespace
{
// Functors to test whether PreFocusChange signal is emitted when the keyboard focus is about to change
class PreFocusChangeCallback : public Dali::ConnectionTracker
{
public:
  PreFocusChangeCallback(bool& signalReceived, Actor firstFocusActor)
  : mSignalVerified(signalReceived),
    mFirstFocusActor(firstFocusActor),
    mDirection(Control::KeyboardFocus::LEFT)
  {
  }

  Actor Callback(Actor currentFocusedActor, Actor proposedActorToFocus, Control::KeyboardFocus::Direction direction)
  {
    tet_infoline("Verifying PreFocusChangeCallback()");

    mSignalVerified = true;
    mDirection      = direction;
    if(!proposedActorToFocus)
    {
      return mFirstFocusActor;
    }
    else
    {
      return proposedActorToFocus;
    }
  }

  void Reset()
  {
    mSignalVerified = false;
    mDirection      = Control::KeyboardFocus::LEFT;
  }

  bool&                             mSignalVerified;
  Actor                             mFirstFocusActor;
  Control::KeyboardFocus::Direction mDirection;
};

// Functors to test whether focus changed signal is emitted when the keyboard focus is changed
class FocusChangedCallback : public Dali::ConnectionTracker
{
public:
  FocusChangedCallback(bool& signalReceived)
  : mSignalVerified(signalReceived),
    mOriginalFocusedActor(),
    mCurrentFocusedActor()
  {
  }

  void Callback(Actor originalFocusedActor, Actor currentFocusedActor)
  {
    tet_infoline("Verifying FocusChangedCallback()");

    if(originalFocusedActor == mCurrentFocusedActor)
    {
      mSignalVerified = true;
    }

    mOriginalFocusedActor = originalFocusedActor;
    mCurrentFocusedActor  = currentFocusedActor;
  }

  void Reset()
  {
    mSignalVerified = false;
  }

  bool& mSignalVerified;
  Actor mOriginalFocusedActor;
  Actor mCurrentFocusedActor;
};

} // anonymous namespace

int UtcDaliToolkitFlexContainerMoveFocus(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerSetPropertyP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, FlexContainer::ROW);

  // Add flex container to the stage
  application.GetScene().Add(flexContainer);
  Size stageSize = application.GetScene().GetSize();

  RelayoutSignalHandler relayoutSignal(flexContainer);
  flexContainer.OnRelayoutSignal().Connect(&relayoutSignal, &RelayoutSignalHandler::RelayoutCallback);
  flexContainer.SetProperty(Actor::Property::SIZE, stageSize);

  // Create two actors and add them to the container
  Actor actor1 = Actor::New();
  Actor actor2 = Actor::New();
  actor1.SetProperty(Actor::Property::KEYBOARD_FOCUSABLE, true);
  actor2.SetProperty(Actor::Property::KEYBOARD_FOCUSABLE, true);
  DALI_TEST_CHECK(actor1);
  DALI_TEST_CHECK(actor2);

  flexContainer.Add(actor1);
  flexContainer.Add(actor2);

  application.SendNotification();
  application.Render();

  KeyboardFocusManager manager = KeyboardFocusManager::Get();
  DALI_TEST_CHECK(manager);

  bool                   preFocusChangeSignalVerified = false;
  PreFocusChangeCallback preFocusChangeCallback(preFocusChangeSignalVerified, actor1);
  manager.PreFocusChangeSignal().Connect(&preFocusChangeCallback, &PreFocusChangeCallback::Callback);

  bool                 focusChangedSignalVerified = false;
  FocusChangedCallback focusChangedCallback(focusChangedSignalVerified);
  manager.FocusChangedSignal().Connect(&focusChangedCallback, &FocusChangedCallback::Callback);

  // Move the focus to the right
  DALI_TEST_EQUALS(manager.MoveFocus(Control::KeyboardFocus::RIGHT), true, TEST_LOCATION);
  DALI_TEST_CHECK(preFocusChangeCallback.mSignalVerified);
  DALI_TEST_EQUALS(manager.GetCurrentFocusActor(), actor1, TEST_LOCATION);
  preFocusChangeCallback.Reset();
  DALI_TEST_EQUALS(focusChangedCallback.mCurrentFocusedActor, actor1, TEST_LOCATION);
  focusChangedCallback.Reset();

  // Move the focus towards right
  DALI_TEST_EQUALS(manager.MoveFocus(Control::KeyboardFocus::RIGHT), true, TEST_LOCATION);
  DALI_TEST_EQUALS(preFocusChangeCallback.mSignalVerified, true, TEST_LOCATION);
  DALI_TEST_EQUALS(manager.GetCurrentFocusActor(), actor2, TEST_LOCATION);
  DALI_TEST_EQUALS(focusChangedCallback.mSignalVerified, true, TEST_LOCATION);
  DALI_TEST_EQUALS(focusChangedCallback.mCurrentFocusedActor, actor2, TEST_LOCATION);

  preFocusChangeCallback.Reset();
  focusChangedCallback.Reset();

  // Move the focus towards left
  DALI_TEST_EQUALS(manager.MoveFocus(Control::KeyboardFocus::LEFT), true, TEST_LOCATION);
  DALI_TEST_EQUALS(preFocusChangeCallback.mSignalVerified, true, TEST_LOCATION);
  DALI_TEST_EQUALS(manager.GetCurrentFocusActor(), actor1, TEST_LOCATION);
  DALI_TEST_EQUALS(focusChangedCallback.mSignalVerified, true, TEST_LOCATION);
  DALI_TEST_EQUALS(focusChangedCallback.mCurrentFocusedActor, actor1, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitFlexContainerRTLSupportP(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolkitFlexContainerRTLSupportP");
  FlexContainer flexContainer = FlexContainer::New();
  DALI_TEST_CHECK(flexContainer);

  Actor actor0 = Actor::New();

  application.GetScene().Add(actor0);
  actor0.Add(flexContainer);

  // Create two actors and add them to the container
  Actor actor1 = Actor::New();
  Actor actor2 = Actor::New();
  DALI_TEST_CHECK(actor1);
  DALI_TEST_CHECK(actor2);

  flexContainer.Add(actor1);
  flexContainer.Add(actor2);

  // Check flex direction property.
  flexContainer.SetProperty(FlexContainer::Property::FLEX_DIRECTION, "row");
  DALI_TEST_EQUALS((FlexContainer::FlexDirection)flexContainer.GetProperty<int>(FlexContainer::Property::FLEX_DIRECTION), FlexContainer::ROW, TEST_LOCATION);

  // Check content direction property.
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::INHERIT, TEST_LOCATION);

  actor0.SetProperty(Dali::Actor::Property::LAYOUT_DIRECTION, Dali::LayoutDirection::RIGHT_TO_LEFT);
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::RTL, TEST_LOCATION);

  actor0.SetProperty(Dali::Actor::Property::LAYOUT_DIRECTION, Dali::LayoutDirection::LEFT_TO_RIGHT);
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::LTR, TEST_LOCATION);

  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "RTL");
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::RTL, TEST_LOCATION);

  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "LTR");
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::LTR, TEST_LOCATION);

  actor0.SetProperty(Dali::Actor::Property::LAYOUT_DIRECTION, Dali::LayoutDirection::RIGHT_TO_LEFT);
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::LTR, TEST_LOCATION);

  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "inherit");
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::RTL, TEST_LOCATION);

  actor0.SetProperty(Dali::Actor::Property::LAYOUT_DIRECTION, Dali::LayoutDirection::LEFT_TO_RIGHT);
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::LTR, TEST_LOCATION);

  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "inherit");
  DALI_TEST_EQUALS((FlexContainer::ContentDirection)flexContainer.GetProperty<int>(FlexContainer::Property::CONTENT_DIRECTION), FlexContainer::LTR, TEST_LOCATION);

  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "LTR");
  application.SendNotification();
  application.Render();

  flexContainer.SetProperty(FlexContainer::Property::CONTENT_DIRECTION, "RTL");
  application.SendNotification();
  application.Render();

  END_TEST;
}
