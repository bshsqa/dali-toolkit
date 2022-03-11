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
#include <string>

#include <dali-toolkit-test-suite-utils.h>
#include "dali-toolkit-test-utils/toolkit-timer.h"

#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/controls/scroll-bar/scroll-bar.h>
#include <dali/devel-api/actors/actor-devel.h>

using namespace Dali;
using namespace Toolkit;

void dali_scrollbar_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_scrollbar_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{
const int RENDER_FRAME_INTERVAL = 16; ///< Duration of each frame in ms. (at approx 60FPS)

/*
 * Simulate time passed by.
 *
 * @note this will always process at least 1 frame (1/60 sec)
 *
 * @param application Test application instance
 * @param duration Time to pass in milliseconds.
 * @return The actual time passed in milliseconds
 */
int Wait(ToolkitTestApplication& application, int duration = 0)
{
  int time = 0;

  for(int i = 0; i <= (duration / RENDER_FRAME_INTERVAL); i++)
  {
    application.SendNotification();
    application.Render(RENDER_FRAME_INTERVAL);
    time += RENDER_FRAME_INTERVAL;
  }

  return time;
}

// Callback probes.

static bool gOnPanFinishedCalled;                         ///< Whether the PanFinished signal was invoked.
static bool gOnScrollPositionIntervalReachedSignalCalled; ///< Whether the ScrollPositionIntervalReached signal was invoked.

/**
 * Invoked when pan gesture is finished on the scroll indicator.
 */
static void OnPanFinished()
{
  gOnPanFinishedCalled = true;
}

struct CallbackFunctor
{
  CallbackFunctor(bool* callbackFlag)
  : mCallbackFlag(callbackFlag)
  {
  }

  void operator()()
  {
    *mCallbackFlag = true;
  }
  bool* mCallbackFlag;
};

/**
 * Invoked when the current scroll position of the scrollable content goes above or below the values
 * specified by SCROLL_POSITION_INTERVALS property.
 *
 * @param[in] position The current scroll position.
 */
static void OnScrollPositionIntervalReached(float position)
{
  gOnScrollPositionIntervalReachedSignalCalled = true;
}

static Vector2 PerformGestureSwipe(ToolkitTestApplication& application, Vector2 start, Vector2 direction, int frames, uint32_t start_time)
{
  gOnPanFinishedCalled = false;

  // Now do a pan starting from (start) and heading (direction)
  Vector2  pos(start + (direction * 15));
  uint32_t time = start_time;

  TestStartPan(application, start, pos, time);

  Wait(application);

  for(int i = 0; i < frames; i++)
  {
    Test::EmitGlobalTimerSignal();

    pos += direction; // Move in this direction
    time += RENDER_FRAME_INTERVAL;
    TestMovePan(application, pos, time);
    Wait(application);
  }

  pos += direction; // Move in this direction.
  time += RENDER_FRAME_INTERVAL;
  TestEndPan(application, pos, time);
  Wait(application);

  return pos;
}

} // namespace

int UtcDaliToolkitScrollBarConstructorP(void)
{
  ToolkitTestApplication application;

  ScrollBar scrollBar;
  DALI_TEST_CHECK(!scrollBar);
  END_TEST;
}

int UtcDaliToolkitScrollBarCopyConstructorP(void)
{
  ToolkitTestApplication application;

  ScrollBar scrollBar = ScrollBar::New();
  scrollBar.SetProperty(ScrollBar::Property::INDICATOR_FIXED_HEIGHT, 38.2f);

  ScrollBar copy(scrollBar);
  DALI_TEST_CHECK(copy);
  DALI_TEST_CHECK(copy.GetProperty<float>(ScrollBar::Property::INDICATOR_FIXED_HEIGHT) == scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_FIXED_HEIGHT));
  END_TEST;
}

int UtcDaliToolkitScrollBarAssignmentOperatorP(void)
{
  ToolkitTestApplication application;

  ScrollBar scrollBar = ScrollBar::New();
  scrollBar.SetProperty(ScrollBar::Property::INDICATOR_FIXED_HEIGHT, 38.2f);

  ScrollBar copy = scrollBar;
  DALI_TEST_CHECK(copy);
  DALI_TEST_CHECK(copy.GetProperty<float>(ScrollBar::Property::INDICATOR_FIXED_HEIGHT) == scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_FIXED_HEIGHT));
  END_TEST;
}

int UtcDaliScrollBarDestructorP(void)
{
  ToolkitTestApplication application;

  ScrollBar* scrollBar = new ScrollBar();
  delete scrollBar;

  DALI_TEST_CHECK(true);
  END_TEST;
}

int UtcDaliToolkitScrollBarNewP(void)
{
  ToolkitTestApplication application;

  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);
  END_TEST;

  ScrollBar vertical = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(vertical);
  DALI_TEST_CHECK(vertical.GetScrollDirection() == ScrollBar::VERTICAL);

  Property::Value value           = vertical.GetProperty(ScrollBar::Property::SCROLL_DIRECTION);
  std::string     scrollDirection = value.Get<std::string>();
  DALI_TEST_EQUALS(scrollDirection, "VERTICAL", TEST_LOCATION);

  ScrollBar horizontal = ScrollBar::New(ScrollBar::HORIZONTAL);
  DALI_TEST_CHECK(horizontal);
  DALI_TEST_CHECK(horizontal.GetScrollDirection() == ScrollBar::HORIZONTAL);
  value           = vertical.GetProperty(ScrollBar::Property::SCROLL_DIRECTION);
  scrollDirection = value.Get<std::string>();
  DALI_TEST_EQUALS(scrollDirection, "HORIZONTAL", TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarCreateP(void)
{
  ToolkitTestApplication application;

  TypeRegistry typeRegistry = TypeRegistry::Get();
  DALI_TEST_CHECK(typeRegistry);

  TypeInfo typeInfo = typeRegistry.GetTypeInfo("ScrollBar");
  DALI_TEST_CHECK(typeInfo);

  BaseHandle handle = typeInfo.CreateInstance();
  DALI_TEST_CHECK(handle);

  ScrollBar scrollBar = ScrollBar::DownCast(handle);
  DALI_TEST_CHECK(scrollBar);

  scrollBar.SetProperty(ScrollBar::Property::SCROLL_DIRECTION, "VERTICAL");
  scrollBar.SetProperty(ScrollBar::Property::INDICATOR_HEIGHT_POLICY, "FIXED");

  DALI_TEST_EQUALS(scrollBar.GetScrollDirection(), Toolkit::ScrollBar::VERTICAL, TEST_LOCATION);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHeightPolicy(), Toolkit::ScrollBar::FIXED, TEST_LOCATION);

  scrollBar.SetProperty(ScrollBar::Property::SCROLL_DIRECTION, "HORIZONTAL");
  scrollBar.SetProperty(ScrollBar::Property::INDICATOR_HEIGHT_POLICY, "VARIABLE");

  DALI_TEST_EQUALS(scrollBar.GetScrollDirection(), Toolkit::ScrollBar::HORIZONTAL, TEST_LOCATION);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHeightPolicy(), Toolkit::ScrollBar::VARIABLE, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarDownCastP(void)
{
  ToolkitTestApplication application;

  ScrollBar  scrollBar1 = ScrollBar::New();
  BaseHandle object(scrollBar1);

  ScrollBar scrollBar2 = ScrollBar::DownCast(object);
  DALI_TEST_CHECK(scrollBar2);

  ScrollBar scrollBar3 = DownCast<ScrollBar>(object);
  DALI_TEST_CHECK(scrollBar3);
  END_TEST;
}

int UtcDaliToolkitScrollBarDownCastN(void)
{
  ToolkitTestApplication application;

  BaseHandle uninitializedObject;
  ScrollBar  scrollBar1 = ScrollBar::DownCast(uninitializedObject);
  DALI_TEST_CHECK(!scrollBar1);

  ScrollBar scrollBar2 = DownCast<ScrollBar>(uninitializedObject);
  DALI_TEST_CHECK(!scrollBar2);
  END_TEST;
}

int UtcDaliToolkitScrollBarSetScrollPropertySourceP(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);
  DALI_TEST_CHECK(scrollBar.GetScrollDirection() == ScrollBar::VERTICAL);

  float scrollBarHeight = 100.0f;
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, scrollBarHeight, 0.0f));
  application.GetScene().Add(scrollBar);

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 100.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 500.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Check that the indicator size should be: scroll bar size * (scroll bar size / content size).
  // i.e. The bigger the content size, the smaller the indicator size
  float indicatorHeight = indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y;
  DALI_TEST_EQUALS(indicatorHeight, scrollBarHeight * scrollBarHeight / 500.0f, TEST_LOCATION);

  // Decrease the content length
  sourceActor.SetProperty(propertyScrollContentSize, 250.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator size is changed accordingly
  indicatorHeight = indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y;
  DALI_TEST_EQUALS(indicatorHeight, scrollBarHeight * scrollBarHeight / 250.0f, TEST_LOCATION);

  // As scroll position is 0, check that the indicator position should be 0.0f.
  float indicatorPosition = indicator.GetCurrentProperty<Vector3>(Actor::Property::POSITION).y;
  DALI_TEST_EQUALS(indicatorPosition, 0.0f, TEST_LOCATION);

  // Set the scroll position to the middle
  sourceActor.SetProperty(propertyScrollPosition, -50.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator should be in the middle of the scroll bar
  indicatorPosition = indicator.GetCurrentProperty<Vector3>(Actor::Property::POSITION).y;
  DALI_TEST_EQUALS(indicatorPosition, (scrollBarHeight - indicatorHeight) * 0.5f, TEST_LOCATION);

  // Set the scroll position to the maximum
  sourceActor.SetProperty(propertyScrollPosition, -100.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator should be in the end of the scroll bar
  indicatorPosition = indicator.GetCurrentProperty<Vector3>(Actor::Property::POSITION).y;
  DALI_TEST_EQUALS(indicatorPosition, scrollBarHeight - indicatorHeight, TEST_LOCATION);

  // Increase the maximum scroll position to double
  sourceActor.SetProperty(propertyMaxScrollPosition, 200.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator should be now in the middle of the scroll bar
  indicatorPosition = indicator.GetCurrentProperty<Vector3>(Actor::Property::POSITION).y;
  DALI_TEST_EQUALS(indicatorPosition, (scrollBarHeight - indicatorHeight) * 0.5f, TEST_LOCATION);

  // Create another source actor
  Actor newSourceActor = Actor::New();
  application.GetScene().Add(newSourceActor);

  // Register the scroll properties
  Property::Index newPropertyScrollPosition    = newSourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index newPropertyMinScrollPosition = newSourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index newPropertyMaxScrollPosition = newSourceActor.RegisterProperty("sourcePositionMax", 200.0f);
  Property::Index newPropertyScrollContentSize = newSourceActor.RegisterProperty("sourceContentSize", 400.0f);

  DALI_TEST_EQUALS(newSourceActor.GetPropertyIndex("sourcePosition"), newPropertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(newSourceActor.GetPropertyIndex("sourcePositionMin"), newPropertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(newSourceActor.GetPropertyIndex("sourcePositionMax"), newPropertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(newSourceActor.GetPropertyIndex("sourceContentSize"), newPropertyScrollContentSize, TEST_LOCATION);

  // Change the source of the scroll position properties to be the new source actor.
  scrollBar.SetScrollPropertySource(newSourceActor, newPropertyScrollPosition, newPropertyMinScrollPosition, newPropertyMaxScrollPosition, newPropertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator size is changed accordingly
  indicatorHeight = indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y;
  DALI_TEST_EQUALS(indicatorHeight, scrollBarHeight * scrollBarHeight / 400.0f, TEST_LOCATION);

  // Check that the indicator position goes back to the beginning of the scroll bar
  indicatorPosition = indicator.GetCurrentProperty<Vector3>(Actor::Property::POSITION).y;
  DALI_TEST_EQUALS(indicatorPosition, 0.0f, TEST_LOCATION);

  // Set the scroll position to one fifth of the maximum
  newSourceActor.SetProperty(propertyScrollPosition, -40.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator should be in one fifth from the beginning of the scroll bar
  indicatorPosition = indicator.GetCurrentProperty<Vector3>(Actor::Property::POSITION).y;
  DALI_TEST_EQUALS(indicatorPosition, (scrollBarHeight - indicatorHeight) * 0.2f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetScrollPropertySourceN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  // Set empty handle of source object and invalid source property index.
  Actor sourceActor;
  scrollBar.SetScrollPropertySource(sourceActor, Property::INVALID_INDEX, Property::INVALID_INDEX, Property::INVALID_INDEX, Property::INVALID_INDEX);

  DALI_TEST_CHECK(true);
  END_TEST;
}

int UtcDaliToolkitScrollBarSetScrollIndicatorP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Set a new indicator
  Actor newIndicator = Actor::New();
  scrollBar.SetScrollIndicator(newIndicator);

  // Check that the new indicator is successfully set
  DALI_TEST_CHECK(indicator != scrollBar.GetScrollIndicator());
  DALI_TEST_CHECK(newIndicator == scrollBar.GetScrollIndicator());

  // Check that the new control indicator is successfully set
  Control controlIndicator = Control::New();
  scrollBar.SetScrollIndicator(controlIndicator);

  DALI_TEST_CHECK(controlIndicator == Control::DownCast(scrollBar.GetScrollIndicator()));
  END_TEST;
}

int UtcDaliToolkitScrollBarSetScrollIndicatorN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Try to set an uninitialized actor as the indicator
  Actor uninitializedIndicator;
  scrollBar.SetScrollIndicator(uninitializedIndicator);

  // Check that the uninitialized actor can not be set as the indicator
  DALI_TEST_CHECK(indicator == scrollBar.GetScrollIndicator());
  DALI_TEST_CHECK(uninitializedIndicator != scrollBar.GetScrollIndicator());

  END_TEST;
}

int UtcDaliToolkitScrollBarGetScrollIndicatorP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Set a new indicator
  Actor newIndicator = Actor::New();
  scrollBar.SetScrollIndicator(newIndicator);

  // Check that the new indicator is successfully set
  DALI_TEST_CHECK(scrollBar.GetScrollIndicator());
  DALI_TEST_CHECK(indicator != scrollBar.GetScrollIndicator());
  DALI_TEST_CHECK(newIndicator == scrollBar.GetScrollIndicator());

  END_TEST;
}

int UtcDaliToolkitScrollBarGetScrollIndicatorN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Try to set an uninitialized actor as the indicator
  Actor uninitializedIndicator;
  scrollBar.SetScrollIndicator(uninitializedIndicator);

  // Check that the indicator has not been changed
  DALI_TEST_CHECK(indicator == scrollBar.GetScrollIndicator());
  DALI_TEST_CHECK(uninitializedIndicator != scrollBar.GetScrollIndicator());

  END_TEST;
}

int UtcDaliToolkitScrollBarSetScrollPositionIntervalsP(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);

  scrollBar.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, 800.0f, 0.0f));

  application.GetScene().Add(scrollBar);

  // Connect to the ScrollPositionIntervalReached signal
  scrollBar.ScrollPositionIntervalReachedSignal().Connect(&OnScrollPositionIntervalReached);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 800.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 2000.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Set the values to get notified when the scroll positions of the source actor goes above or below these values
  Dali::Vector<float> positionIntervals;
  for(size_t i = 0; i != 10; ++i)
  {
    positionIntervals.PushBack(-80.0f * i); // should get notified for each 80 pixels
  }
  scrollBar.SetScrollPositionIntervals(positionIntervals);

  // Get the list of scroll position intervals for notification
  Dali::Vector<float> results = scrollBar.GetScrollPositionIntervals();

  // Check that the result is the same as the list previously set.
  DALI_TEST_EQUALS(positionIntervals.Count(), results.Count(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[0], results[0], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[1], results[1], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[2], results[2], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[3], results[3], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[4], results[4], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[5], results[5], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[6], results[6], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[7], results[7], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[8], results[8], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[9], results[9], TEST_LOCATION);

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;

  // Animate the scroll position to cross the specified value
  Animation animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -85.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;

  // Rest and clear the animation
  animation.Clear();
  animation.Reset();

  // Animate the scroll position to cross another specified value
  animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -170.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;

  // Rest and clear the animation
  animation.Clear();
  animation.Reset();

  // Animate the scroll position back to the previous value
  animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -85.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarGetScrollPositionIntervalsP(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);

  // Set the values to get notified when the scroll positions of the source actor goes above or below these values
  Dali::Vector<float> positionIntervals;
  for(size_t i = 0; i != 10; ++i)
  {
    positionIntervals.PushBack(-80.0f * i); // should get notified for each 80 pixels
  }
  scrollBar.SetScrollPositionIntervals(positionIntervals);

  // Get the list of scroll position intervals for notification
  Dali::Vector<float> results = scrollBar.GetScrollPositionIntervals();

  // Check that the result is the same as the list previously set.
  DALI_TEST_EQUALS(positionIntervals.Count(), results.Count(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[0], results[0], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[1], results[1], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[2], results[2], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[3], results[3], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[4], results[4], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[5], results[5], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[6], results[6], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[7], results[7], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[8], results[8], TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[9], results[9], TEST_LOCATION);

  Property::Array resultArray = scrollBar.GetProperty<Property::Array>(Toolkit::ScrollBar::Property::SCROLL_POSITION_INTERVALS);

  DALI_TEST_EQUALS(positionIntervals.Count(), resultArray.Count(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[0], resultArray[0].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[1], resultArray[1].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[2], resultArray[2].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[3], resultArray[3].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[4], resultArray[4].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[5], resultArray[5].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[6], resultArray[6].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[7], resultArray[7].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[8], resultArray[8].Get<float>(), TEST_LOCATION);
  DALI_TEST_EQUALS(positionIntervals[9], resultArray[9].Get<float>(), TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarGetScrollDirectionP(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);
  DALI_TEST_CHECK(scrollBar.GetScrollDirection() == ScrollBar::VERTICAL);

  // Change the direction of scroll bar to horizontal
  scrollBar.SetScrollDirection(ScrollBar::HORIZONTAL);
  DALI_TEST_CHECK(scrollBar.GetScrollDirection() == ScrollBar::HORIZONTAL);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetIndicatorHeightPolicyP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  float scrollBarHeight = 100.0f;
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, scrollBarHeight, 0.0f));
  application.GetScene().Add(scrollBar);

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 100.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 500.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Check that the indicator size should be: scroll bar size * (scroll bar size / content size).
  // i.e. The bigger the content size, the smaller the indicator size
  float indicatorHeight = indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y;
  DALI_TEST_EQUALS(indicatorHeight, scrollBarHeight * scrollBarHeight / 500.0f, TEST_LOCATION);

  // Set the indicator height to be fixed to 50.0f
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::FIXED);
  scrollBar.SetIndicatorFixedHeight(50.0f);

  Property::Value value = scrollBar.GetProperty(ScrollBar::Property::INDICATOR_HEIGHT_POLICY);
  DALI_TEST_EQUALS(value.Get<std::string>(), "FIXED", TEST_LOCATION);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator size should be 50.0f
  indicatorHeight = indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y;
  DALI_TEST_EQUALS(indicatorHeight, 50.0f, TEST_LOCATION);

  // Set the indicator height to be variable
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::VARIABLE);
  value = scrollBar.GetProperty(ScrollBar::Property::INDICATOR_HEIGHT_POLICY);
  DALI_TEST_EQUALS(value.Get<std::string>(), "VARIABLE", TEST_LOCATION);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator size should be: scroll bar size * (scroll bar size / content size).
  indicatorHeight = indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y;
  DALI_TEST_EQUALS(indicatorHeight, scrollBarHeight * scrollBarHeight / 500.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarGetIndicatorHeightPolicyP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  // Set the indicator height to be fixed
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::FIXED);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHeightPolicy(), Toolkit::ScrollBar::FIXED, TEST_LOCATION);

  // Set the indicator height to be variable
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::VARIABLE);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHeightPolicy(), Toolkit::ScrollBar::VARIABLE, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetIndicatorFixedHeightP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  float scrollBarHeight = 100.0f;
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, scrollBarHeight, 0.0f));
  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Set the indicator height to be fixed to 50.0f
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::FIXED);
  scrollBar.SetIndicatorFixedHeight(50.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator size should be 50.0f
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y, 50.0f, TEST_LOCATION);

  // Set the indicator height to be fixed to 25.0f
  scrollBar.SetIndicatorFixedHeight(25.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator size should be 25.0f
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<Vector3>(Actor::Property::SIZE).y, 25.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarGetIndicatorFixedHeightP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  // Set the fixed indicator height to be 50.0f
  scrollBar.SetIndicatorFixedHeight(50.0f);

  // Check that the indicator size should be 50.0f
  DALI_TEST_EQUALS(scrollBar.GetIndicatorFixedHeight(), 50.0f, TEST_LOCATION);

  // Set the indicator height to be fixed to 25.0f
  scrollBar.SetIndicatorFixedHeight(25.0f);

  // Check that the indicator size should be 50.0f
  DALI_TEST_EQUALS(scrollBar.GetIndicatorFixedHeight(), 25.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetIndicatorShowDurationP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Set the duration to show the indicator to be 0.35 second
  scrollBar.SetIndicatorShowDuration(0.35);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorShowDuration(), 0.35f, TEST_LOCATION);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Show the indicator
  scrollBar.ShowIndicator();

  // Wait for 0.35 second
  Wait(application, 350);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Set the duration to show the indicator to be 0.75 second
  scrollBar.SetIndicatorShowDuration(0.75);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorShowDuration(), 0.75f, TEST_LOCATION);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Show the indicator
  scrollBar.ShowIndicator();

  // Wait for 0.35 second first
  Wait(application, 350);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is not fully visible yet
  DALI_TEST_CHECK(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY) != 1.0f);

  // Wait for another 0.4 second
  Wait(application, 400);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now fully visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetIndicatorShowDurationN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to show the indicator
  float duration = scrollBar.GetIndicatorShowDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Show the indicator
  scrollBar.ShowIndicator();

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Now set the duration to show the indicator to be a negative value (which should be ignored and therefore means instant)
  scrollBar.SetIndicatorShowDuration(-0.25f);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorShowDuration(), -0.25f, TEST_LOCATION);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Show the indicator
  scrollBar.ShowIndicator();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator becomes instantly visible in the next frame
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarGetIndicatorShowDurationP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  // Set the duration to show the indicator to be 0.35 second
  scrollBar.SetIndicatorShowDuration(0.35f);

  // Check that the duration to show the indicator is 0.35 second
  DALI_TEST_EQUALS(scrollBar.GetIndicatorShowDuration(), 0.35f, TEST_LOCATION);

  // Set the duration to show the indicator to be 0.75 second
  scrollBar.SetIndicatorShowDuration(0.75f);

  // Check that the duration to show the indicator is 0.75 second
  DALI_TEST_EQUALS(scrollBar.GetIndicatorShowDuration(), 0.75f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetIndicatorHideDurationP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Set the duration to hide the indicator to be 0.15 second
  scrollBar.SetIndicatorHideDuration(0.15f);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHideDuration(), 0.15f, TEST_LOCATION);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Wait for 0.15 second
  Wait(application, 150);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Set the duration to hide the indicator to be 0.65 second
  scrollBar.SetIndicatorHideDuration(0.65f);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHideDuration(), 0.65f, TEST_LOCATION);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Wait for 0.15 second first
  Wait(application, 150);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is not fully invisible yet
  DALI_TEST_CHECK(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY) != 0.0f);

  // Wait for another 0.5 second
  Wait(application, 500);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now fully invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarSetIndicatorHideDurationN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetIndicatorHideDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Now set the duration to hide the indicator to be a negative value (which should be ignored and therefore means instant)
  scrollBar.SetIndicatorHideDuration(-0.25f);
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHideDuration(), -0.25f, TEST_LOCATION);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator becomes instantly invisible in the next frame
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarGetIndicatorHideDurationP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  // Set the duration to hide the indicator to be 0.15 second
  scrollBar.SetIndicatorHideDuration(0.15f);

  // Check that the duration to hide the indicator is 0.15 second
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHideDuration(), 0.15f, TEST_LOCATION);

  // Set the duration to hide the indicator to be 0.65 second
  scrollBar.SetIndicatorHideDuration(0.65f);

  // Check that the duration to hide the indicator is 0.65 second
  DALI_TEST_EQUALS(scrollBar.GetIndicatorHideDuration(), 0.65f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarShowIndicatorP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to show the indicator
  float duration = scrollBar.GetIndicatorShowDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Show the indicator
  scrollBar.ShowIndicator();

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarShowIndicatorN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Make the indicator initially visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is initially visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Get the default duration to show the indicator
  float duration = scrollBar.GetIndicatorShowDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Show the indicator
  scrollBar.ShowIndicator();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is still visible in the very next frame
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarHideIndicatorP(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetIndicatorHideDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarHideIndicatorN(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Make the indicator initially invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is initially invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetIndicatorHideDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is still invisible in the very next frame
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarActionShowIndicator(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_SHOW_DURATION);

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Do the "ShowIndicator" action
  Property::Map emptyMap;
  scrollBar.DoAction("ShowIndicator", emptyMap);

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarActionHideIndicator(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_HIDE_DURATION);

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Do the "HideIndicator" action
  Property::Map emptyMap;
  scrollBar.DoAction("HideIndicator", emptyMap);

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarActionShowTransientIndicator(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_SHOW_DURATION);

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  // Do the "ShowIndicator" action
  Property::Map emptyMap;
  scrollBar.DoAction("ShowTransientIndicator", emptyMap);

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Get the default duration to hide the indicator
  float hideDuration         = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_HIDE_DURATION);
  float transientDuration    = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_TRANSIENT_DURATION);
  float totalVisibleDuration = hideDuration + transientDuration;

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(totalVisibleDuration > 0.0f);

  // Wait for the specified duration
  Wait(application, totalVisibleDuration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarActionShowTransientIndicatorImmediate(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Make the indicator invisible
  indicator.SetProperty(Actor::Property::OPACITY, 0.0f);

  // Don't use a show animation; the indicator should appear immediately
  scrollBar.SetProperty(ScrollBar::Property::INDICATOR_SHOW_DURATION, 0.0f);
  float duration = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_SHOW_DURATION);
  DALI_TEST_EQUALS(duration, 0.0f, TEST_LOCATION);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Do the "ShowIndicator" action
  Property::Map emptyMap;
  scrollBar.DoAction("ShowTransientIndicator", emptyMap);

  // Wait for the specified duration
  Wait(application);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Get the default duration to hide the indicator
  float hideDuration         = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_HIDE_DURATION);
  float transientDuration    = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_TRANSIENT_DURATION);
  float totalVisibleDuration = hideDuration + transientDuration;

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(totalVisibleDuration > 0.0f);

  // Wait for the specified duration
  Wait(application, totalVisibleDuration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarActionShowTransientIndicatorDuringHide(void)
{
  ToolkitTestApplication application;

  // Create a scroll bar
  ScrollBar scrollBar = ScrollBar::New();
  DALI_TEST_CHECK(scrollBar);

  application.GetScene().Add(scrollBar);

  Actor indicator = scrollBar.GetScrollIndicator();
  DALI_TEST_CHECK(indicator);

  // Get the default duration to hide the indicator
  float duration = scrollBar.GetIndicatorHideDuration();

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Make the indicator visible
  indicator.SetProperty(Actor::Property::OPACITY, 1.0f);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Hide the indicator
  scrollBar.HideIndicator();

  // Wait for half the specified duration
  Wait(application, duration * 0.5f * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now partially hidden
  DALI_TEST_CHECK(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY) < 1.0f);

  // Now interrupt the Hide with a DoAction( "ShowTransientIndicator" )

  // Get the default duration to hide the indicator
  duration = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_SHOW_DURATION);

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(duration > 0.0f);

  // Do the "ShowIndicator" action
  Property::Map emptyMap;
  scrollBar.DoAction("ShowTransientIndicator", emptyMap);

  // Wait for the specified duration
  Wait(application, duration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now visible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 1.0f, TEST_LOCATION);

  // Get the default duration to hide the indicator
  float hideDuration         = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_HIDE_DURATION);
  float transientDuration    = scrollBar.GetProperty<float>(ScrollBar::Property::INDICATOR_TRANSIENT_DURATION);
  float totalVisibleDuration = hideDuration + transientDuration;

  // Check that the default duration is greater than 0
  DALI_TEST_CHECK(totalVisibleDuration > 0.0f);

  // Wait for the specified duration
  Wait(application, totalVisibleDuration * 1000);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Check that the indicator is now invisible
  DALI_TEST_EQUALS(indicator.GetCurrentProperty<float>(Actor::Property::OPACITY), 0.0f, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarPanFinishedSignalP(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);

  scrollBar.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, 800.0f, 0.0f));

  // Set the indicator height to be fixed to 50.0f
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::FIXED);
  scrollBar.SetIndicatorFixedHeight(50.0f);

  application.GetScene().Add(scrollBar);

  // Connect the pan finished signal
  ConnectionTracker connectionTracker;
  bool              panFinished = false;
  scrollBar.PanFinishedSignal().Connect(&OnPanFinished);
  scrollBar.ConnectSignal(&connectionTracker, "panFinished", CallbackFunctor(&panFinished));

  // Render and notify
  application.SendNotification();
  application.Render();

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 100.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 500.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Perform a swipe gesture on the indicator
  PerformGestureSwipe(application, Vector2(1.0f, 1.0f), Vector2(Vector2::YAXIS * 1.0f), 1, 500);
  DALI_TEST_EQUALS(gOnPanFinishedCalled, true, TEST_LOCATION);
  DALI_TEST_EQUALS(panFinished, true, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarPanFinishedSignalN(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);

  scrollBar.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, 800.0f, 0.0f));

  // Set the indicator height to be fixed to 50.0f
  scrollBar.SetIndicatorHeightPolicy(Toolkit::ScrollBar::FIXED);
  scrollBar.SetIndicatorFixedHeight(50.0f);

  application.GetScene().Add(scrollBar);

  // Connect the pan finished signal
  ConnectionTracker connectionTracker;
  bool              panFinished = false;
  scrollBar.PanFinishedSignal().Connect(&OnPanFinished);
  scrollBar.ConnectSignal(&connectionTracker, "panFinished", CallbackFunctor(&panFinished));

  // Render and notify
  application.SendNotification();
  application.Render();

  // Perform a vertical swipe gesture on the indicator when there is no source object set on the scroll bar
  PerformGestureSwipe(application, Vector2(1.0f, 1.0f), Vector2(Vector2::YAXIS * 1.0f), 20, 500);
  DALI_TEST_EQUALS(gOnPanFinishedCalled, false, TEST_LOCATION);

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 100.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 500.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Perform a swipe gesture on the scroll bar but not on the indicator
  PerformGestureSwipe(application, Vector2(1.0f, 780.0f), Vector2(Vector2::YAXIS * -1.0f), 20, 2000);
  DALI_TEST_EQUALS(gOnPanFinishedCalled, false, TEST_LOCATION);
  DALI_TEST_EQUALS(panFinished, false, TEST_LOCATION);

  // Perform a swipe gesture on the indicator
  PerformGestureSwipe(application, Vector2(1.0f, 1.0f), Vector2(Vector2::YAXIS * 1.0f), 20, 4000);
  DALI_TEST_EQUALS(gOnPanFinishedCalled, true, TEST_LOCATION);
  DALI_TEST_EQUALS(panFinished, true, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarScrollPositionIntervalReachedSignalP(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);

  scrollBar.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, 800.0f, 0.0f));

  application.GetScene().Add(scrollBar);
  ConnectionTracker connectionTracker;

  // Connect to the ScrollPositionIntervalReached signal
  bool intervalReached = false;
  scrollBar.ScrollPositionIntervalReachedSignal().Connect(&OnScrollPositionIntervalReached);
  scrollBar.ConnectSignal(&connectionTracker, "scrollPositionIntervalReached", CallbackFunctor(&intervalReached));

  // Render and notify
  application.SendNotification();
  application.Render();

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 800.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 2000.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Set the values to get notified when the scroll positions of the source actor goes above or below these values
  Dali::Vector<float> positionIntervals;
  for(size_t i = 0; i != 10; ++i)
  {
    positionIntervals.PushBack(-80.0f * i); // should get notified for each 80 pixels
  }
  scrollBar.SetScrollPositionIntervals(positionIntervals);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;

  // Animate the scroll position to cross the specified value
  Animation animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -85.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);
  DALI_TEST_EQUALS(intervalReached, true, TEST_LOCATION);

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;
  intervalReached                              = false;

  // Rest and clear the animation
  animation.Clear();
  animation.Reset();

  // Animate the scroll position to cross another specified value
  animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -170.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);
  DALI_TEST_EQUALS(intervalReached, true, TEST_LOCATION);

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;
  intervalReached                              = false;

  // Rest and clear the animation
  animation.Clear();
  animation.Reset();

  // Animate the scroll position back to the previous value
  animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -85.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);
  DALI_TEST_EQUALS(intervalReached, true, TEST_LOCATION);

  END_TEST;
}

int UtcDaliToolkitScrollBarScrollPositionIntervalReachedSignalN(void)
{
  ToolkitTestApplication application;

  // Create a vertical scroll bar
  ScrollBar scrollBar = ScrollBar::New(ScrollBar::VERTICAL);
  DALI_TEST_CHECK(scrollBar);

  scrollBar.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
  scrollBar.SetProperty(Actor::Property::SIZE, Vector3(20.0f, 800.0f, 0.0f));

  application.GetScene().Add(scrollBar);

  // Connect to the ScrollPositionIntervalReached signal
  scrollBar.ScrollPositionIntervalReachedSignal().Connect(&OnScrollPositionIntervalReached);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Create a source actor that owns the scroll properties required by the scroll bar
  Actor sourceActor = Actor::New();
  application.GetScene().Add(sourceActor);

  // Register the scroll properties
  Property::Index propertyScrollPosition    = sourceActor.RegisterProperty("sourcePosition", 0.0f);
  Property::Index propertyMinScrollPosition = sourceActor.RegisterProperty("sourcePositionMin", 0.0f);
  Property::Index propertyMaxScrollPosition = sourceActor.RegisterProperty("sourcePositionMax", 800.0f);
  Property::Index propertyScrollContentSize = sourceActor.RegisterProperty("sourceContentSize", 2000.0f);

  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePosition"), propertyScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMin"), propertyMinScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourcePositionMax"), propertyMaxScrollPosition, TEST_LOCATION);
  DALI_TEST_EQUALS(sourceActor.GetPropertyIndex("sourceContentSize"), propertyScrollContentSize, TEST_LOCATION);

  // Set the source of the scroll position properties.
  scrollBar.SetScrollPropertySource(sourceActor, propertyScrollPosition, propertyMinScrollPosition, propertyMaxScrollPosition, propertyScrollContentSize);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Set the values to get notified when the scroll positions of the source actor goes above or below these values
  Dali::Vector<float> positionIntervals;
  for(size_t i = 0; i != 10; ++i)
  {
    positionIntervals.PushBack(-80.0f * i); // should get notified for each 80 pixels
  }
  scrollBar.SetScrollPositionIntervals(positionIntervals);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Reset the flag
  gOnScrollPositionIntervalReachedSignalCalled = false;

  // Animate the scroll position not to cross the specified value
  Animation animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -70.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is not called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, false, TEST_LOCATION);

  // Rest and clear the animation
  animation.Clear();
  animation.Reset();

  // Animate the scroll position to cross another specified value
  animation = Animation::New(0.1f);
  animation.AnimateTo(Property(sourceActor, propertyScrollPosition), -85.0f);
  animation.Play();

  // Wait for 0.1 second
  Wait(application, 100);

  // Check that the signal callback is called
  DALI_TEST_EQUALS(gOnScrollPositionIntervalReachedSignalCalled, true, TEST_LOCATION);

  END_TEST;
}
