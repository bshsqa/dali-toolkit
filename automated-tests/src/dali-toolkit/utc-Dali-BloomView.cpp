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
#include <dali-toolkit/devel-api/controls/bloom-view/bloom-view.h>
#include <stdlib.h>
#include <iostream>

using namespace Dali;
using namespace Dali::Toolkit;

void bloom_view_startup(void)
{
  test_return_value = TET_UNDEF;
}

void bloom_view_cleanup(void)
{
  test_return_value = TET_PASS;
}

// Negative test case for a method
int UtcDaliBloomViewUninitialized(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliBloomViewUninitialized");

  Toolkit::BloomView view;

  try
  {
    // New() must be called to create a BloomView or it wont be valid.
    Actor a = Actor::New();
    view.Add(a);
    DALI_TEST_CHECK(false);
  }
  catch(Dali::DaliException& e)
  {
    // Tests that a negative test of an assertion succeeds
    DALI_TEST_PRINT_ASSERT(e);
    DALI_TEST_CHECK(!view);
  }
  END_TEST;
}

// Positive test case for a method
int UtcDaliBloomViewNew(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliBloomViewNew");

  Toolkit::BloomView view = Toolkit::BloomView::New();
  DALI_TEST_CHECK(view);

  Toolkit::BloomView view2 = Toolkit::BloomView::New(10, 1.0f, Pixel::RGB888, 0.5f, 0.5f);
  DALI_TEST_CHECK(view2);
  END_TEST;
}

// Positive test case for a method
int UtcDaliBloomViewDownCast(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliBloomViewDownCast");

  Toolkit::BloomView view = Toolkit::BloomView::New();
  BaseHandle         handle(view);

  Toolkit::BloomView bloomView = Toolkit::BloomView::DownCast(handle);
  DALI_TEST_CHECK(view);
  DALI_TEST_CHECK(bloomView);
  DALI_TEST_CHECK(bloomView == view);
  END_TEST;
}

// Positive test case for a method
int UtcDaliBloomViewPropertyNames(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliBloomViewPropertyNames");

  Toolkit::BloomView view = Toolkit::BloomView::New();
  DALI_TEST_CHECK(view);

  // Check the names, this names are used in the shader code,
  // if they change in the shader code, then it has to be updated here.
  DALI_TEST_EQUALS(view.GetBloomThresholdPropertyIndex(), view.GetPropertyIndex("uBloomThreshold"), TEST_LOCATION);
  DALI_TEST_EQUALS(view.GetBlurStrengthPropertyIndex(), view.GetPropertyIndex("BlurStrengthProperty"), TEST_LOCATION);
  DALI_TEST_EQUALS(view.GetBloomIntensityPropertyIndex(), view.GetPropertyIndex("uBloomIntensity"), TEST_LOCATION);
  DALI_TEST_EQUALS(view.GetBloomSaturationPropertyIndex(), view.GetPropertyIndex("uBloomSaturation"), TEST_LOCATION);
  DALI_TEST_EQUALS(view.GetImageIntensityPropertyIndex(), view.GetPropertyIndex("uImageIntensity"), TEST_LOCATION);
  DALI_TEST_EQUALS(view.GetImageSaturationPropertyIndex(), view.GetPropertyIndex("uImageSaturation"), TEST_LOCATION);
  END_TEST;
}

// Positive test case for a method
int UtcDaliBloomViewAddRemove(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliBloomViewAddRemove");

  Toolkit::BloomView view = Toolkit::BloomView::New();
  DALI_TEST_CHECK(view);

  Actor actor = Actor::New();
  DALI_TEST_CHECK(!actor.GetProperty<bool>(Actor::Property::CONNECTED_TO_SCENE));

  view.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  view.SetProperty(Actor::Property::SIZE, application.GetScene().GetSize());
  view.Add(actor);
  application.GetScene().Add(view);

  DALI_TEST_CHECK(actor.GetProperty<bool>(Actor::Property::CONNECTED_TO_SCENE));

  view.Remove(actor);

  DALI_TEST_CHECK(!actor.GetProperty<bool>(Actor::Property::CONNECTED_TO_SCENE));
  END_TEST;
}

// Positive test case for a method
int UtcDaliBloomActivateDeactivate(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliBloomActivateDeactivate");

  Toolkit::BloomView view = Toolkit::BloomView::New();
  DALI_TEST_CHECK(view);

  RenderTaskList taskList = application.GetScene().GetRenderTaskList();
  DALI_TEST_CHECK(1u == taskList.GetTaskCount());

  view.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  view.SetProperty(Actor::Property::SIZE, application.GetScene().GetSize());
  view.Add(Actor::New());
  application.GetScene().Add(view);
  view.Activate();

  RenderTaskList taskList2 = application.GetScene().GetRenderTaskList();
  DALI_TEST_CHECK(1u != taskList2.GetTaskCount());

  view.Deactivate();

  RenderTaskList taskList3 = application.GetScene().GetRenderTaskList();
  DALI_TEST_CHECK(1u == taskList3.GetTaskCount());
  END_TEST;
}

int UtcDaliBloomCopyAndAssignment(void)
{
  ToolkitTestApplication application;

  BloomView view = Toolkit::BloomView::New();
  DALI_TEST_CHECK(view);

  BloomView copy(view);
  DALI_TEST_CHECK(view == copy);

  BloomView assign;
  DALI_TEST_CHECK(!assign);

  assign = copy;
  DALI_TEST_CHECK(assign == view);

  END_TEST;
}

int UtcDaliBloomTypeRegistry(void)
{
  ToolkitTestApplication application;

  TypeRegistry typeRegistry = TypeRegistry::Get();
  DALI_TEST_CHECK(typeRegistry);

  TypeInfo typeInfo = typeRegistry.GetTypeInfo("BloomView");
  DALI_TEST_CHECK(typeInfo);

  BaseHandle handle = typeInfo.CreateInstance();
  DALI_TEST_CHECK(handle);

  BloomView view = BloomView::DownCast(handle);
  DALI_TEST_CHECK(view);

  END_TEST;
}

int UtcDaliBloomOnSizeSet(void)
{
  ToolkitTestApplication application;

  BloomView view = Toolkit::BloomView::New();

  application.GetScene().Add(view);

  application.SendNotification();
  application.Render();

  Vector3 size(200.0f, 300.0f, 0.0f);
  view.SetProperty(Actor::Property::SIZE, size);

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS(view.GetCurrentProperty<Vector3>(Actor::Property::SIZE), size, TEST_LOCATION);

  END_TEST;
}
