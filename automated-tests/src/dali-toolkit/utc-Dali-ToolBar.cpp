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
#include <dali-toolkit/devel-api/controls/tool-bar/tool-bar.h>
#include <dali-toolkit/devel-api/visual-factory/visual-factory.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <stdlib.h>
#include <iostream>
#include "dummy-control.h"

using namespace Dali;
using namespace Toolkit;

namespace
{
static bool gObjectCreatedCallBackCalled;

static void TestCallback(BaseHandle handle)
{
  gObjectCreatedCallBackCalled = true;
}

Actor CreateColorActor(const Vector4& color)
{
  DummyControl      solidColorActor = DummyControl::New();
  DummyControlImpl& dummyImpl       = static_cast<DummyControlImpl&>(solidColorActor.GetImplementation());

  VisualFactory       factory = VisualFactory::Get();
  Dali::Property::Map map;
  map[Toolkit::Visual::Property::TYPE]  = Visual::COLOR;
  map[ColorVisual::Property::MIX_COLOR] = color;
  Visual::Base colorVisual              = factory.CreateVisual(map);
  dummyImpl.RegisterVisual(Control::CONTROL_PROPERTY_END_INDEX + 1, colorVisual);

  return solidColorActor;
}

} // namespace

void dali_toolbar_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_toolbar_cleanup(void)
{
  test_return_value = TET_PASS;
}

int UtcDaliToolBarNew(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolBarNew");

  ToolBar toolbar;

  DALI_TEST_CHECK(!toolbar);

  toolbar = ToolBar::New();

  DALI_TEST_CHECK(toolbar);

  ToolBar toolbar2(toolbar);

  DALI_TEST_CHECK(toolbar2 == toolbar);

  //Additional check to ensure object is created by checking if it's registered
  ObjectRegistry registry = application.GetCore().GetObjectRegistry();
  DALI_TEST_CHECK(registry);

  gObjectCreatedCallBackCalled = false;
  registry.ObjectCreatedSignal().Connect(&TestCallback);
  {
    ToolBar toolbar = ToolBar::New();
  }
  DALI_TEST_CHECK(gObjectCreatedCallBackCalled);

  Actor actor = toolbar;
  toolbar     = ToolBar::DownCast(actor);

  DALI_TEST_CHECK(toolbar);
  END_TEST;
}

int UtcDaliToolBarAddControl01(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolBarAddControl01");

  try
  {
    Actor control1 = CreateColorActor(Color::RED);
    control1.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));
    Actor control2 = CreateColorActor(Color::RED);
    control2.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));
    Actor control3 = CreateColorActor(Color::RED);
    control3.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));
    Actor control4 = CreateColorActor(Color::RED);
    control4.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));
    Actor control5 = CreateColorActor(Color::RED);
    control5.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));

    ToolBar toolbar = ToolBar::New();
    toolbar.SetProperty(Actor::Property::SIZE, Vector2(600.f, 100.f));

    application.Render();
    application.SendNotification();
    application.Render();
    application.SendNotification();

    toolbar.Add(control1);
    toolbar.AddControl(control2, 0.1f, Alignment::HORIZONTAL_LEFT, Alignment::Padding(1.f, 1.f, 1.f, 1.f));
    toolbar.AddControl(control3, 0.1f, Alignment::HORIZONTAL_CENTER, Alignment::Padding(1.f, 1.f, 1.f, 1.f));
    toolbar.AddControl(control4, 0.1f, Alignment::HORIZONTAL_CENTER, Alignment::Padding(1.f, 1.f, 1.f, 1.f));
    toolbar.AddControl(control5, 0.1f, Alignment::HORIZONTAL_RIGHT, Alignment::Padding(1.f, 1.f, 1.f, 1.f));

    Actor control6 = CreateColorActor(Color::RED);
    control6.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));
    Actor control7 = CreateColorActor(Color::RED);
    control7.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));
    Actor control8 = CreateColorActor(Color::RED);
    control8.SetProperty(Actor::Property::SIZE, Vector2(100.f, 100.f));

    application.Render();
    application.SendNotification();
    application.Render();
    application.SendNotification();

    toolbar.AddControl(control6, 0.4f, Alignment::HORIZONTAL_LEFT, Alignment::Padding(1.f, 1.f, 1.f, 1.f));
    toolbar.AddControl(control7, 0.2f, Alignment::HORIZONTAL_CENTER, Alignment::Padding(1.f, 1.f, 1.f, 1.f));
    toolbar.AddControl(control8, 0.2f, Alignment::HORIZONTAL_RIGHT, Alignment::Padding(1.f, 1.f, 1.f, 1.f));
  }
  catch(...)
  {
    tet_result(TET_FAIL);
  }

  tet_result(TET_PASS);
  END_TEST;
}

int UtcDaliToolBarAddControl02(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolBarAddControl02");

  bool daliAssert = false;

  try
  {
    Actor control = CreateColorActor(Color::RED);

    ToolBar toolbar = ToolBar::New();

    toolbar.AddControl(control, 0.1f, static_cast<Alignment::Type>(99), Alignment::Padding(1.f, 1.f, 1.f, 1.f));
  }
  catch(DaliException e)
  {
    daliAssert = true;
    tet_result(TET_PASS);
  }
  catch(...)
  {
    tet_result(TET_FAIL);
  }

  if(!daliAssert)
  {
    tet_result(TET_FAIL);
  }
  END_TEST;
}

int UtcDaliToolBarRemoveControl01(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolBarRemoveControl01");

  try
  {
    Actor control = CreateColorActor(Color::RED);

    ToolBar toolbar = ToolBar::New();
    toolbar.AddControl(control, 0.1f, Alignment::HORIZONTAL_LEFT);

    toolbar.RemoveControl(control);
  }
  catch(...)
  {
    tet_result(TET_FAIL);
  }

  tet_result(TET_PASS);
  END_TEST;
}

int UtcDaliToolBarRemoveControl02(void)
{
  ToolkitTestApplication application;
  tet_infoline(" UtcDaliToolBarRemoveControl02");

  try
  {
    Actor control01 = CreateColorActor(Color::RED);
    Actor control02 = CreateColorActor(Color::RED);

    ToolBar toolbar01 = ToolBar::New();
    ToolBar toolbar02 = ToolBar::New();
    toolbar01.AddControl(control01, 0.1f, Alignment::HORIZONTAL_LEFT);
    toolbar02.AddControl(control02, 0.1f, Alignment::HORIZONTAL_LEFT);

    toolbar02.RemoveControl(control01);
  }
  catch(Dali::DaliException& e)
  {
    DALI_TEST_PRINT_ASSERT(e);
    DALI_TEST_EQUALS(e.condition, "false", TEST_LOCATION);
  }
  catch(...)
  {
    tet_result(TET_FAIL);
  }

  try
  {
    Actor control = CreateColorActor(Color::RED);

    ToolBar toolbar = ToolBar::New();
    toolbar.AddControl(control, 0.1f, Alignment::HORIZONTAL_LEFT);

    toolbar.RemoveControl(control);
    toolbar.RemoveControl(control);
  }
  catch(...)
  {
    tet_result(TET_FAIL);
  }

  tet_result(TET_PASS);
  END_TEST;
}
