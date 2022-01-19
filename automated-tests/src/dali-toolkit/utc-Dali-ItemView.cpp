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
#include <float.h>       // for FLT_MAX

// Need to override adaptor classes for toolkit test harness, so include
// test harness headers before dali headers.
#include <dali-toolkit-test-suite-utils.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <dali/integration-api/events/wheel-event-integ.h>

using namespace Dali;
using namespace Toolkit;

void utc_dali_toolkit_item_view_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_toolkit_item_view_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{

const unsigned int TOTAL_ITEM_NUMBER = 400;
const char* TEST_IMAGE_FILE_NAME = "gallery_image_01.jpg";

const int RENDER_FRAME_INTERVAL = 16;                     ///< Duration of each frame in ms. (at approx 60FPS)

static bool gObjectCreatedCallBackCalled;
static bool gOnLayoutActivatedCalled;                     ///< Whether the LayoutActivated signal was invoked.
static bool gOnScrollUpdateCalled;
static bool gOnWheelEventCalled;                          ///< Whether the WheelEventSignal signal was invoked.

static void TestCallback(BaseHandle handle)
{
  gObjectCreatedCallBackCalled = true;
}

static void OnLayoutActivated()
{
  gOnLayoutActivatedCalled = true;
}

static void OnScrollUpdate( const Vector2& position )
{
  gOnScrollUpdateCalled = true;
}

static bool OnWheelEvent( Actor actor, const Dali::WheelEvent& wheelEvent )
{
  gOnWheelEventCalled = true;
  return false;
}

Integration::TouchEvent GenerateSingleTouch( PointState::Type state, const Vector2& screenPosition, uint32_t time )
{
  Integration::TouchEvent touchEvent;
  Integration::Point point;
  point.SetState( state );
  point.SetDeviceId(4);
  point.SetScreenPosition( screenPosition );
  point.SetDeviceClass( Device::Class::TOUCH );
  point.SetDeviceSubclass( Device::Subclass::NONE );
  touchEvent.points.push_back( point );
  touchEvent.time = time;
  return touchEvent;
}

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

  for(int i = 0; i <= ( duration / RENDER_FRAME_INTERVAL); i++)
  {
    application.SendNotification();
    application.Render(RENDER_FRAME_INTERVAL);
    time += RENDER_FRAME_INTERVAL;
  }

  return time;
}

// Implementation of ItemFactory for providing actors to ItemView
class TestItemFactory : public ItemFactory
{
public:

  /**
   * Constructor
   * @param application class, stored as reference
   */
  TestItemFactory()
  {
  }

public: // From ItemFactory

  /**
   * Query the number of items available from the factory.
   * The maximum available item has an ID of GetNumberOfItems() - 1.
   */
  virtual unsigned int GetNumberOfItems()
  {
    return TOTAL_ITEM_NUMBER;
  }

  /**
   * Create an Actor to represent a visible item.
   * @param itemId
   * @return the created actor.
   */
  virtual Actor NewItem(unsigned int itemId)
  {
    // Create a renderable actor for this item
    return ImageView::New( TEST_IMAGE_FILE_NAME );
  }
};

} // namespace


int UtcDaliItemViewNew(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  DALI_TEST_CHECK(view);

  //Additional check to ensure object is created by checking if it's registered
  ObjectRegistry registry = application.GetCore().GetObjectRegistry();
  DALI_TEST_CHECK( registry );

  gObjectCreatedCallBackCalled = false;
  registry.ObjectCreatedSignal().Connect(&TestCallback);
  {
    TestItemFactory factory;
    ItemView view = ItemView::New(factory);
  }
  DALI_TEST_CHECK( gObjectCreatedCallBackCalled );
  END_TEST;
}

int UtcDaliItemViewCopyConstructor(void)
{
  ToolkitTestApplication application;

  TestItemFactory factory;
  ItemView itemView = ItemView::New( factory );
  DALI_TEST_CHECK( itemView );

  ItemView copy( itemView );
  DALI_TEST_CHECK( copy );

  END_TEST;
}

int UtcDaliItemViewCopyAssignment(void)
{
  ToolkitTestApplication application;

  TestItemFactory factory;
  ItemView itemView = ItemView::New( factory );
  DALI_TEST_CHECK( itemView );

  ItemView copy;
  copy = itemView;
  DALI_TEST_CHECK( copy );
  DALI_TEST_EQUALS( itemView, copy, TEST_LOCATION );

  END_TEST;
}

int UtcDaliItemViewMoveConstructor(void)
{
  ToolkitTestApplication application;

  TestItemFactory factory;
  ItemView itemView = ItemView::New( factory );
  DALI_TEST_EQUALS( 1, itemView.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  itemView.SetProperty( Actor::Property::SENSITIVE, false );
  DALI_TEST_CHECK( false == itemView.GetProperty< bool >( Actor::Property::SENSITIVE ) );

  ItemView moved = std::move( itemView );
  DALI_TEST_CHECK( moved );
  DALI_TEST_EQUALS( 1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_CHECK( false == moved.GetProperty< bool >( Actor::Property::SENSITIVE ) );
  DALI_TEST_CHECK( !itemView );

  END_TEST;
}

int UtcDaliItemViewMoveAssignment(void)
{
  ToolkitTestApplication application;

  TestItemFactory factory;
  ItemView itemView = ItemView::New( factory );
  DALI_TEST_EQUALS( 1, itemView.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  itemView.SetProperty( Actor::Property::SENSITIVE, false );
  DALI_TEST_CHECK( false == itemView.GetProperty< bool >( Actor::Property::SENSITIVE ) );

  ItemView moved;
  moved = std::move( itemView );
  DALI_TEST_CHECK( moved );
  DALI_TEST_EQUALS( 1, moved.GetBaseObject().ReferenceCount(), TEST_LOCATION );
  DALI_TEST_CHECK( false == moved.GetProperty< bool >( Actor::Property::SENSITIVE ) );
  DALI_TEST_CHECK( !itemView );

  END_TEST;
}

int UtcDaliItemViewDownCast(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  const ItemView itemViewConst = ItemView::New(factory);
  ItemView itemView(itemViewConst);

  BaseHandle handle(itemView);

  ItemView newItemView = ItemView::DownCast( handle );
  DALI_TEST_CHECK( itemView );
  DALI_TEST_CHECK( newItemView == itemView );
  END_TEST;
}

int UtcDaliItemViewAddAndGetLayout(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);

  // As we have added one layout, check the number of layout is now 1
  DALI_TEST_CHECK(view.GetLayoutCount() == 1);

  // Create a depth layout and add it to ItemView
  ItemLayoutPtr depthLayout = DefaultItemLayout::New( DefaultItemLayout::DEPTH );
  view.AddLayout(*depthLayout);

  // As we have added another layout, check the number of layout is now 2
  DALI_TEST_CHECK(view.GetLayoutCount() == 2);

  // Create a spiral layout and add it to ItemView
  ItemLayoutPtr spiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  view.AddLayout(*spiralLayout);

  // As we have added another layout, check the number of layout is now 3
  DALI_TEST_CHECK(view.GetLayoutCount() == 3);

  // Check we are getting the correct layout from ItemView
  DALI_TEST_CHECK(view.GetLayout(0) == gridLayout);
  DALI_TEST_CHECK(view.GetLayout(1) == depthLayout);
  DALI_TEST_CHECK(view.GetLayout(2) == spiralLayout);
  END_TEST;
}

int UtcDaliItemViewAddAndRemoveLayout(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);

  // As we have added one layout, check the number of layout is now 1
  DALI_TEST_CHECK(view.GetLayoutCount() == 1);

  // Create a depth layout and add it to ItemView
  ItemLayoutPtr depthLayout = DefaultItemLayout::New( DefaultItemLayout::DEPTH );
  view.AddLayout(*depthLayout);

  // As we have added another layout, check the number of layout is now 2
  DALI_TEST_CHECK(view.GetLayoutCount() == 2);

  // Check we are getting the correct layout from ItemView
  DALI_TEST_CHECK(view.GetLayout(0) == gridLayout);
  DALI_TEST_CHECK(view.GetLayout(1) == depthLayout);

  // Remove the grid layout
  view.RemoveLayout(0);

  // As we have removed the grid layout, check the number of layout is now 1
  DALI_TEST_CHECK(view.GetLayoutCount() == 1);

  // Check we are getting the correct layout from ItemView
  DALI_TEST_CHECK(view.GetLayout(0) == depthLayout);

  // Remove the depth layout
  view.RemoveLayout(0);

  // As we also removed the depth layout, check the number of layout is now 0
  DALI_TEST_CHECK(view.GetLayoutCount() == 0);
  END_TEST;
}

int UtcDaliItemViewActivateLayoutAndGetActiveLayout(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);

  // Create a depth layout and add it to ItemView
  ItemLayoutPtr depthLayout = DefaultItemLayout::New( DefaultItemLayout::DEPTH );
  view.AddLayout(*depthLayout);

  // Create a spiral layout and add it to ItemView
  ItemLayoutPtr spiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  view.AddLayout(*spiralLayout);

  // As we have added three layouts, check the number of layout is now 3
  DALI_TEST_CHECK(view.GetLayoutCount() == 3);

  // Check there is no active layout at the moment
  DALI_TEST_CHECK(!view.GetActiveLayout());

  // Activate the depth layout
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(1, stageSize, 0.5f);

  // Check the current active layout is the depth layout
  DALI_TEST_CHECK(view.GetActiveLayout() == depthLayout);

  // Activate the grid layout
  view.ActivateLayout(0, stageSize, 0.5f);

  // Check the current active layout is the grid layout
  DALI_TEST_CHECK(view.GetActiveLayout() == gridLayout);

  // Activate the spiral layout
  view.ActivateLayout(2, stageSize, 0.5f);

  // Check the current active layout is the spiral layout
  DALI_TEST_CHECK(view.GetActiveLayout() == spiralLayout);
  END_TEST;
}

int UtcDaliItemViewDeactivateCurrentLayout(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  gridLayout->SetOrientation(ControlOrientation::Down);
  view.AddLayout(*gridLayout);

  // Check there is no active layout at the moment
  DALI_TEST_CHECK(!view.GetActiveLayout());

  // Activate the grid layout
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  // Check the current active layout is the grid layout
  DALI_TEST_CHECK(view.GetActiveLayout() == gridLayout);

  // Deactivate the current layout
  view.DeactivateCurrentLayout();

  // Check there is no active layout at the moment
  DALI_TEST_CHECK(!view.GetActiveLayout());
  END_TEST;
}

int UtcDaliItemViewGetItemAndGetItemId(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  gridLayout->SetOrientation(ControlOrientation::Left);
  view.AddLayout(*gridLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  // Get the item given the item ID
  Actor itemActor = view.GetItem(2);

  // Check we are getting the correct Item ID given the specified actor
  DALI_TEST_CHECK(view.GetItemId(itemActor) == 2);
  END_TEST;
}

int UtcDaliItemViewRemoveItem(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  gridLayout->SetOrientation(ControlOrientation::Right);
  view.AddLayout(*gridLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  // Get the item given the item ID 2 and 3
  Actor oldItemActorID2 = view.GetItem(2);
  Actor oldItemActorID3 = view.GetItem(3);

  // Remove the item with ID 2
  view.RemoveItem(2, 0.0f);

  // Get the new item given the item ID 2 and 3
  Actor newItemActorID2 = view.GetItem(2);
  Actor newItemActorID3 = view.GetItem(3);

  // Check the original item with item ID 2 was deleted and now item ID 2 represents the original item with ID 3
  DALI_TEST_CHECK(view.GetItemId(newItemActorID2) == 2);
  DALI_TEST_CHECK(oldItemActorID2 != newItemActorID2);
  DALI_TEST_CHECK(newItemActorID2 == oldItemActorID3);

  // scroll to the end of item view
  view.ScrollToItem(TOTAL_ITEM_NUMBER - 1, 0.00f);

  application.SendNotification();
  application.Render(0);

  // Refresh the item view
  view.Refresh();

  Actor itemActorID390 = view.GetItem(390);
  DALI_TEST_CHECK(view.GetItemId(itemActorID390) == 390);

  // Remove the item with ID 2 (which is now before the current item range)
  view.RemoveItem(2, 0.0f);

  // Check the original item with item ID 2 was deleted and now item ID 389 represents the original item with ID 390
  DALI_TEST_CHECK(view.GetItemId(itemActorID390) == 389);
  DALI_TEST_CHECK(view.GetItem(389) == itemActorID390);

  END_TEST;
}

int UtcDaliItemViewGetCurrentLayoutPosition(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a depth layout and add it to ItemView
  ItemLayoutPtr depthLayout = DefaultItemLayout::New( DefaultItemLayout::DEPTH );
  depthLayout->SetOrientation(ControlOrientation::Up);
  view.AddLayout(*depthLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.0f);

  // Check the current layout position for the 10th items is 9.0f
  DALI_TEST_EQUALS(view.GetCurrentLayoutPosition(9), 9.0f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliItemViewSetAndGetMinimumSwipeSpeed(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Set the minimum swipe speed to be 1.5f
  view.SetMinimumSwipeSpeed(1.5f);

  // Check the minimum swipe speed is 1.5f
  DALI_TEST_EQUALS(view.GetMinimumSwipeSpeed(), 1.5f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliItemViewSetAndGetMinimumSwipeDistance(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Set the minimum swipe distance to be 2.5f
  view.SetMinimumSwipeDistance(2.5f);

  // Check the minimum swipe distance is 2.5f
  DALI_TEST_EQUALS(view.GetMinimumSwipeDistance(), 2.5f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliItemViewSetAndGetAnchoring(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Disable the anchor animation
  view.SetAnchoring(false);

  // Check the anchor animation is disabled
  DALI_TEST_CHECK(view.GetAnchoring() == false);
  END_TEST;
}

int UtcDaliItemViewSetAndGetAnchoringDuration(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Set the duration of anchor animation to be 1.5f
  view.SetAnchoringDuration(1.5f);

  // Check the duration of anchor animation is 1.5f
  DALI_TEST_EQUALS(view.GetAnchoringDuration(), 1.5f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliItemViewSetAndGetRefreshInterval(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Set the interval between refreshes to be 20
  view.SetRefreshInterval(20);

  view.Refresh();

  // Check the interval between refreshes is 20
  DALI_TEST_CHECK(view.GetRefreshInterval() == 20);
  END_TEST;
}

int UtcDaliItemViewScrollToItem(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);
  Vector3 vec(480.0f, 800.0f, 0.0f);
  ItemLayoutPtr layout = DefaultItemLayout::New( DefaultItemLayout::DEPTH );

  view.SetProperty( Dali::Actor::Property::NAME,"view actor");
  view.AddLayout(*layout);
  view.SetProperty( Actor::Property::SIZE, vec );

  application.GetScene().Add(view);
  layout->SetOrientation(ControlOrientation::Down);
  view.ActivateLayout(0, vec, 0.0f);

  application.SendNotification();
  application.Render(0);

  // render 10 frames
  for(int i = 0; i < 10; ++i)
  {
    application.Render(16); // 60hz frames
  }

  // Confirm: we have actors in the view.
  std::vector<unsigned int> indices;
  for(unsigned int i = 0; i < 10; i++)
  {
    Actor testActor = view.GetItem(i);
    if (testActor)
    {
      indices.push_back(i);
    }
  }

  try
  {
    if (!indices.empty())
    {
      const unsigned int firstTargetIndex = indices[indices.size()-1];
      // scroll to last item
      view.ScrollToItem(firstTargetIndex, 0.00f);
      for(int i = 0; i < 10; ++i)
      {
        application.Render(16); // 60hz frames
      }

      std::size_t moveCount = 0;
      for(std::size_t i = 0; i < indices.size(); i++)
      {
        float layoutPosBefore = view.GetCurrentLayoutPosition(i);
        view.ScrollToItem(indices[i], 0.0f);
        float layoutPosAfter = view.GetCurrentLayoutPosition(i);

        if (fabs(layoutPosBefore-layoutPosAfter) <= FLT_EPSILON)
        {
          ++moveCount;
        }
      }

      DALI_TEST_CHECK((moveCount == indices.size()));
    }
  }
  catch(...)
  {
    tet_result(TET_FAIL);
  }

  application.GetScene().Remove(view);
  END_TEST;
}

int UtcDaliItemViewSetAndGetWheelScrollDistanceStep(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Set the scroll distance step for the wheel event to be 100.0f
  view.SetWheelScrollDistanceStep(100.0f);

  // Check the scroll distance step is 100.0f
  DALI_TEST_EQUALS(view.GetWheelScrollDistanceStep(), 100.0f, TEST_LOCATION );
  END_TEST;
}

int UtcDaliItemViewInsertItemP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID);
  gridLayout->SetOrientation(ControlOrientation::Left);
  view.AddLayout(*gridLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.0f);

  // Get the specified item where new item to be inserted before that
  Actor itemActor = view.GetItem(2);

  ItemId id = view.GetItemId( itemActor );

  // Check we are getting the correct Item ID given the specified actor
  DALI_TEST_CHECK(view.GetItemId(itemActor) == 2);

  Actor newActor = Actor::New();

  view.InsertItem(Item(id, newActor), 0.0f);

  DALI_TEST_CHECK(view.GetItem(2) == newActor);

  DALI_TEST_CHECK(view.GetItemId(itemActor) == 3);
  DALI_TEST_CHECK(view.GetItem(3) == itemActor);

  // scroll to the end of item view
  view.ScrollToItem(TOTAL_ITEM_NUMBER - 1, 0.00f);

  application.SendNotification();
  application.Render(0);

  // Refresh the item view
  view.Refresh();

  Actor itemActorID390 = view.GetItem(390);
  DALI_TEST_CHECK(view.GetItemId(itemActorID390) == 390);

  // Insert the item with ID 2 (which is now before the current item range)
  Actor anotherNewActor = Actor::New();
  view.InsertItem(Item(id, anotherNewActor), 0.0f);

  // Check that item ID 391 now represents the original item with ID 390
  DALI_TEST_CHECK(view.GetItemId(itemActorID390) == 391);
  DALI_TEST_CHECK(view.GetItem(391) == itemActorID390);

  END_TEST;
}

int UtcDaliItemViewInsertItemsP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a depth layout and add it to ItemView
  ItemLayoutPtr depthLayout = DefaultItemLayout::New( DefaultItemLayout::DEPTH);
  depthLayout->SetOrientation(ControlOrientation::Right);
  view.AddLayout(*depthLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  unsigned int itemCount = view.GetChildCount();

  // Get the specified item where new items to be inserted before that
  Actor itemActor = view.GetItem(1);

  // Check we are getting the correct Item ID given the specified item
  DALI_TEST_CHECK(view.GetItemId(itemActor) == 1);

  ItemContainer insertList;

  for( unsigned int i = 1u; i < 11; ++i )
  {
    Actor child = view.GetChildAt( i );
    Actor newActor = Actor::New();
    newActor.SetProperty( Dali::Actor::Property::NAME,"Inserted");
    insertList.push_back( Item( view.GetItemId(child), newActor ) );
  }

  if( !insertList.empty() )
  {
    view.InsertItems( insertList, 0.5f );
  }

  DALI_TEST_CHECK(view.GetChildCount() == itemCount + 10);

  // Check that new items are inserted in the correct positions
  DALI_TEST_CHECK(view.GetItemId(itemActor) == 11);
  DALI_TEST_CHECK(view.GetItem(11) == itemActor);

  ItemIdContainer removeList;

  for( unsigned int i = 0u; i < view.GetChildCount(); ++i )
  {
    Actor child = view.GetChildAt( i );

    if( child.GetProperty< std::string >( Dali::Actor::Property::NAME ) == "Inserted" )
    {
      removeList.push_back( view.GetItemId(child) );
    }
  }

  if( ! removeList.empty() )
  {
    view.RemoveItems( removeList, 0.5f );
  }

  DALI_TEST_CHECK(view.GetChildCount() == itemCount);

  // Check that new items are removed correctly so that we are getting the correct Item ID given the specified item
  DALI_TEST_CHECK(view.GetItemId(itemActor) == 1);
  DALI_TEST_CHECK(view.GetItem(1) == itemActor);

  END_TEST;
}

int UtcDaliItemViewReplaceItemP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a spiral layout and add it to ItemView
  ItemLayoutPtr spiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  view.AddLayout(*spiralLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  Actor newActor = Actor::New();

  view.ReplaceItem( Item( 5, newActor ), 0.5f );

  DALI_TEST_CHECK(view.GetItem(5) == newActor);

  END_TEST;
}

int UtcDaliItemViewReplaceItemsP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a spiral layout and add it to ItemView
  ItemLayoutPtr spiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  spiralLayout->SetOrientation( ControlOrientation::Down );
  view.AddLayout(*spiralLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  ItemContainer replaceList;

  for( unsigned int i = 0u; i < 10; ++i )
  {
    Actor child = view.GetItem( i );
    Actor newActor = Actor::New();
    newActor.SetProperty( Dali::Actor::Property::NAME,"Replaced");

    replaceList.push_back( Item( i, newActor ) );
  }

  if( !replaceList.empty() )
  {
    view.ReplaceItems( replaceList, 0.5f );
  }

  DALI_TEST_CHECK(view.GetItem(0).GetProperty< std::string >( Dali::Actor::Property::NAME ) == "Replaced");
  DALI_TEST_CHECK(view.GetItem(8).GetProperty< std::string >( Dali::Actor::Property::NAME ) == "Replaced");
  END_TEST;
}

int UtcDaliItemViewGetItemsRangeP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a spiral layout and add it to ItemView
  ItemLayoutPtr spiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  spiralLayout->SetOrientation( ControlOrientation::Left );
  view.AddLayout(*spiralLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  ItemRange itemRange(0, 0);

  view.GetItemsRange(itemRange);

  DALI_TEST_CHECK(itemRange.Within(0));
  END_TEST;
}

int UtcDaliItemViewSetItemsAnchorPointP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a spiral layout and add it to ItemView
  ItemLayoutPtr spiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  spiralLayout->SetOrientation( ControlOrientation::Right );
  view.AddLayout(*spiralLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  Vector3 anchorPoint(10.0f, 10.0f, 0.0f);

  view.SetItemsAnchorPoint(anchorPoint);

  DALI_TEST_CHECK(view.GetItemsAnchorPoint() == anchorPoint);
  DALI_TEST_CHECK(view.GetItem(0).GetCurrentProperty< Vector3 >( Actor::Property::ANCHOR_POINT ) == anchorPoint);
  END_TEST;
}

int UtcDaliItemViewSetItemsParentOriginP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  Vector3 parentOrigin(10.0f, 10.0f, 0.0f);

  view.SetItemsParentOrigin(parentOrigin);

  DALI_TEST_CHECK(view.GetItemsParentOrigin() == parentOrigin);
  DALI_TEST_CHECK(view.GetItem(0).GetCurrentProperty< Vector3 >( Actor::Property::PARENT_ORIGIN ) == parentOrigin);
  END_TEST;
}

int UtcDaliItemFactoryGetExtention(void)
{
  ToolkitTestApplication application;
  TestItemFactory factory;
  DALI_TEST_CHECK( factory.GetExtension() == NULL );
  END_TEST;
}

int UtcDaliItemViewLayoutActivatedSignalP(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);

  application.GetScene().Add( view );

  // Connect the layout activated signal
  view.LayoutActivatedSignal().Connect( &OnLayoutActivated );

  gOnLayoutActivatedCalled = false;

  // Render and notify
  application.SendNotification();
  application.Render();

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(application.GetScene().GetSize());
  view.ActivateLayout(0, stageSize, 0.1f);

  // Wait for 0.1 second
  Wait(application, 100);

  DALI_TEST_EQUALS( gOnLayoutActivatedCalled, true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliItemViewSetGetProperty(void)
{
  ToolkitTestApplication application;

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);
  DALI_TEST_CHECK(view);

  // Event side properties

  // Test "minimumSwipeSpeed" property
  DALI_TEST_CHECK( view.GetPropertyIndex("minimumSwipeSpeed") == ItemView::Property::MINIMUM_SWIPE_SPEED  );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::MINIMUM_SWIPE_SPEED).Get<float>(), view.GetMinimumSwipeSpeed(), TEST_LOCATION );
  view.SetProperty( ItemView::Property::MINIMUM_SWIPE_SPEED, 2.5f );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::MINIMUM_SWIPE_SPEED).Get<float>(), 2.5f, TEST_LOCATION );

  // Test "minimumSwipeDistance" property
  DALI_TEST_CHECK( view.GetPropertyIndex("minimumSwipeDistance") == ItemView::Property::MINIMUM_SWIPE_DISTANCE  );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::MINIMUM_SWIPE_DISTANCE).Get<float>(), view.GetMinimumSwipeDistance(), TEST_LOCATION );
  view.SetProperty( ItemView::Property::MINIMUM_SWIPE_DISTANCE, 8.725f );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::MINIMUM_SWIPE_DISTANCE).Get<float>(), 8.725f, TEST_LOCATION );

  // Test "wheelScrollDistanceStep" property
  DALI_TEST_CHECK( view.GetPropertyIndex("wheelScrollDistanceStep") == ItemView::Property::WHEEL_SCROLL_DISTANCE_STEP  );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::WHEEL_SCROLL_DISTANCE_STEP).Get<float>(), view.GetWheelScrollDistanceStep(), TEST_LOCATION );
  view.SetProperty( ItemView::Property::WHEEL_SCROLL_DISTANCE_STEP, 5.0f );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::WHEEL_SCROLL_DISTANCE_STEP).Get<float>(), 5.0f, TEST_LOCATION );

  // Test "snapToItemEnabled" property
  DALI_TEST_CHECK( view.GetPropertyIndex("snapToItemEnabled") == ItemView::Property::SNAP_TO_ITEM_ENABLED  );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::SNAP_TO_ITEM_ENABLED).Get<bool>(), view.GetAnchoring(), TEST_LOCATION );
  view.SetProperty( ItemView::Property::SNAP_TO_ITEM_ENABLED, true );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::SNAP_TO_ITEM_ENABLED).Get<bool>(), true, TEST_LOCATION );

  // Test "refreshInterval" property
  DALI_TEST_CHECK( view.GetPropertyIndex("refreshInterval") == ItemView::Property::REFRESH_INTERVAL  );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::REFRESH_INTERVAL).Get<float>(), view.GetRefreshInterval(), TEST_LOCATION );
  view.SetProperty( ItemView::Property::REFRESH_INTERVAL, 11.0f );
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::REFRESH_INTERVAL).Get<float>(), 11.0f, TEST_LOCATION );

  // Test "layout" property
  DALI_TEST_CHECK( view.GetPropertyIndex("layout") == ItemView::Property::LAYOUT  );
  Property::Map gridLayoutProperty;
  gridLayoutProperty.Insert( DefaultItemLayoutProperty::TYPE, Dali::Property::Value((int)DefaultItemLayout::GRID) );
  gridLayoutProperty.Insert( DefaultItemLayoutProperty::ITEM_SIZE, Dali::Property::Value(Vector3(200, 200,50)) );
  gridLayoutProperty.Insert( DefaultItemLayoutProperty::GRID_ROW_SPACING, Dali::Property::Value(50.0f) );
  gridLayoutProperty.Insert( DefaultItemLayoutProperty::GRID_COLUMN_NUMBER, Dali::Property::Value(4) );

  Property::Map depthLayoutProperty;
  depthLayoutProperty.Insert( DefaultItemLayoutProperty::TYPE, Dali::Property::Value((int)DefaultItemLayout::DEPTH) );
  depthLayoutProperty.Insert( DefaultItemLayoutProperty::DEPTH_COLUMN_NUMBER, Dali::Property::Value(3) );
  depthLayoutProperty.Insert( DefaultItemLayoutProperty::DEPTH_ROW_NUMBER, Dali::Property::Value(26.0f) );

  Property::Map spiralLayoutPrperty;
  spiralLayoutPrperty.Insert( DefaultItemLayoutProperty::TYPE, Dali::Property::Value((int)DefaultItemLayout::SPIRAL) );
  spiralLayoutPrperty.Insert( DefaultItemLayoutProperty::SPIRAL_ITEM_SPACING, Dali::Property::Value((Math::PI*2.0f)/9.5f) );
  spiralLayoutPrperty.Insert( DefaultItemLayoutProperty::SPIRAL_TOP_ITEM_ALIGNMENT, Dali::Property::Value(-0.125f) );

  Property::Map listLayoutPrperty;
  listLayoutPrperty.Insert( DefaultItemLayoutProperty::TYPE, Dali::Property::Value((int)DefaultItemLayout::LIST) );
  listLayoutPrperty.Insert( DefaultItemLayoutProperty::ITEM_SIZE, Dali::Property::Value(Vector3(100, 100,50)) );


  Property::Array layoutArray;
  layoutArray.PushBack(gridLayoutProperty);
  layoutArray.PushBack(depthLayoutProperty);
  layoutArray.PushBack(spiralLayoutPrperty);
  layoutArray.PushBack(listLayoutPrperty);

  view.SetProperty( ItemView::Property::LAYOUT, layoutArray);

  Property::Array getLayoutArray;
  DALI_TEST_CHECK( view.GetProperty( ItemView::Property::LAYOUT ).Get( getLayoutArray ) );

  //Check that the result is the same as
  DALI_TEST_EQUALS( layoutArray.Count(), getLayoutArray.Count(), TEST_LOCATION );
  Property::Map firstLayout = *((getLayoutArray.GetElementAt( 0 )).GetMap());

  for( unsigned int mapIdx = 0, mapCount = firstLayout.Count(); mapIdx < mapCount; ++mapIdx )
  {
    KeyValuePair propertyPair( firstLayout.GetKeyValue( mapIdx ) );
    if(propertyPair.first == DefaultItemLayoutProperty::TYPE)
    {
      int layoutType = propertyPair.second.Get<int>();
      DALI_TEST_EQUALS( layoutType, (int)DefaultItemLayout::GRID, TEST_LOCATION );
    }
    else if(propertyPair.first == DefaultItemLayoutProperty::ITEM_SIZE)
    {
      Vector3 size = propertyPair.second.Get<Vector3>();
      DALI_TEST_EQUALS( size, Vector3(200, 200,50), TEST_LOCATION );
    }
    else if(propertyPair.first == DefaultItemLayoutProperty::GRID_ROW_SPACING)
    {
      float spacing = propertyPair.second.Get<float>();
      DALI_TEST_EQUALS( spacing, 50.0f, TEST_LOCATION );
    }
    else if(propertyPair.first == DefaultItemLayoutProperty::GRID_COLUMN_NUMBER)
    {
      int number = propertyPair.second.Get<int>();
      DALI_TEST_EQUALS(number, 4, TEST_LOCATION );
    }
  }
  view.SetProperty( ItemView::Property::LAYOUT, layoutArray);


  // Test "overshootEnabled" property
  DALI_TEST_CHECK( view.GetPropertyIndex("overshootEnabled") == Scrollable::Property::OVERSHOOT_ENABLED  );
  DALI_TEST_EQUALS( view.GetProperty(Scrollable::Property::OVERSHOOT_ENABLED).Get<bool>(), view.IsOvershootEnabled(), TEST_LOCATION );
  view.SetProperty( Scrollable::Property::OVERSHOOT_ENABLED, false );
  DALI_TEST_EQUALS( view.GetProperty(Scrollable::Property::OVERSHOOT_ENABLED).Get<bool>(), false, TEST_LOCATION );

  // Test "overshootSize" property
  DALI_TEST_CHECK( view.GetPropertyIndex("overshootSize") == Scrollable::Property::OVERSHOOT_SIZE  );
  Vector2 overshootSize = Vector2(100.0f,100.0f);
  view.SetProperty( Scrollable::Property::OVERSHOOT_SIZE, overshootSize );
  DALI_TEST_EQUALS( view.GetProperty(Scrollable::Property::OVERSHOOT_SIZE).Get<Vector2>(), overshootSize, TEST_LOCATION );

  // Animatable properties

  // Test "layoutPosition" property
  DALI_TEST_CHECK( view.GetPropertyIndex("layoutPosition") == ItemView::Property::LAYOUT_POSITION  );
  view.SetProperty( ItemView::Property::LAYOUT_POSITION, 20.5f );
  Wait(application);
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::LAYOUT_POSITION).Get<float>(), 20.5f, TEST_LOCATION );

  // Test "scrollSpeed" property
  DALI_TEST_CHECK( view.GetPropertyIndex("scrollSpeed") == ItemView::Property::SCROLL_SPEED  );
  view.SetProperty( ItemView::Property::SCROLL_SPEED, 3.35f );
  Wait(application);
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::SCROLL_SPEED).Get<float>(), 3.35f, TEST_LOCATION );

  // Test "overshoot" property
  DALI_TEST_CHECK( view.GetPropertyIndex("overshoot") == ItemView::Property::OVERSHOOT  );
  view.SetProperty( ItemView::Property::OVERSHOOT, 0.15f );
  Wait(application);
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::OVERSHOOT).Get<float>(), 0.15f, TEST_LOCATION );

  // Test "scrollDirection" property
  DALI_TEST_CHECK( view.GetPropertyIndex("scrollDirection") == ItemView::Property::SCROLL_DIRECTION  );
  view.SetProperty( ItemView::Property::SCROLL_DIRECTION, Vector2(0.85f, 0.5f) );
  Wait(application);
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::SCROLL_DIRECTION).Get<Vector2>(), Vector2(0.85f, 0.5f), TEST_LOCATION );

  // Test "layoutOrientation" property
  DALI_TEST_CHECK( view.GetPropertyIndex("layoutOrientation") == ItemView::Property::LAYOUT_ORIENTATION  );
  view.SetProperty( ItemView::Property::LAYOUT_ORIENTATION, 2 );
  Wait(application);
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::LAYOUT_ORIENTATION).Get<int>(), 2, TEST_LOCATION );

  // Test "scrollContentSize" property
  DALI_TEST_CHECK( view.GetPropertyIndex("scrollContentSize") == ItemView::Property::SCROLL_CONTENT_SIZE  );
  view.SetProperty( ItemView::Property::SCROLL_CONTENT_SIZE, 250.0f );
  Wait(application);
  DALI_TEST_EQUALS( view.GetProperty(ItemView::Property::SCROLL_CONTENT_SIZE).Get<float>(), 250.0f, TEST_LOCATION );

  END_TEST;
}


int UtcDaliItemViewOvershootVertical(void)
{
  ToolkitTestApplication application;
  Dali::Integration::Scene stage = application.GetScene();

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);
  stage.Add(view);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(stage.GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  view.SetProperty( Scrollable::Property::OVERSHOOT_ENABLED, true );
  DALI_TEST_EQUALS( view.GetProperty(Scrollable::Property::OVERSHOOT_ENABLED).Get<bool>(), true, TEST_LOCATION );

  view.SetProperty( Scrollable::Property::OVERSHOOT_SIZE, Vector2(30, 30) );

  Wait(application);

  // Do a pan starting from 100,100 and moving down
  Vector2 pos(100.0f, 100.0f);

  application.ProcessEvent( GenerateSingleTouch(PointState::DOWN, pos, 100 ) );
  application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );

  pos.y += 5.0f;
  Wait(application, 100);

  for(int i = 0;i<200;i++)
  {
    application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );
    pos.y += 5.0f;
    Wait(application);
  }

  application.ProcessEvent( GenerateSingleTouch(PointState::UP, pos, 100 ) );

  Wait(application, 100);

  // Do a pan starting from 100,100 and moving up
  pos = Vector2(100.0f, 300.0f);

  application.ProcessEvent( GenerateSingleTouch(PointState::DOWN, pos, 100 ) );
  application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );

  pos.y -= 5.0f;
  Wait(application, 100);

  for(int i = 0;i<200;i++)
  {
    application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );
    pos.y -= 5.0f;
    Wait(application);
  }

  application.ProcessEvent( GenerateSingleTouch(PointState::UP, pos, 100 ) );
  Wait(application, 100);
  END_TEST;
}

int UtcDaliItemViewOvershootHorizontal(void)
{
  ToolkitTestApplication application;
  Dali::Integration::Scene stage = application.GetScene();

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
  view.AddLayout(*gridLayout);
  stage.Add(view);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(stage.GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  view.SetProperty( Scrollable::Property::OVERSHOOT_ENABLED, true );
  DALI_TEST_EQUALS( view.GetProperty(Scrollable::Property::OVERSHOOT_ENABLED).Get<bool>(), true, TEST_LOCATION );

  view.SetProperty( Scrollable::Property::OVERSHOOT_SIZE, Vector2(30, 30) );

  Wait(application);

  // Do a pan starting from 100,100 and moving left
  Vector2 pos(100.0f, 100.0f);
  application.ProcessEvent( GenerateSingleTouch(PointState::DOWN, pos, 100 ));
  application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ));
  pos.x -= 5.0f;
  Wait(application, 100);

  for(int i = 0;i<200;i++)
  {
    application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );
    pos.x -= 5.0f;
    Wait(application);
  }

  application.ProcessEvent( GenerateSingleTouch(PointState::UP, pos, 100 ) );
  Wait(application, 100);

  // Do a pan starting from 100,100 and moving right
  pos = Vector2(100.0f, 100.0f);
  application.ProcessEvent( GenerateSingleTouch(PointState::DOWN, pos, 100 ) );
  application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );
  pos.x += 5.0f;
  Wait(application, 100);

  for(int i = 0;i<200;i++)
  {
    application.ProcessEvent( GenerateSingleTouch(PointState::MOTION, pos, 100 ) );
    pos.x += 5.0f;
    Wait(application);
  }

  application.ProcessEvent( GenerateSingleTouch(PointState::UP, pos, 100 ) );
  Wait(application, 100);

  END_TEST;
}

int UtcDaliItemEnableDisableRefresh(void)
{
  ToolkitTestApplication application;
  Dali::Integration::Scene stage = application.GetScene();

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New(factory);

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout(*gridLayout);
  stage.Add(view);

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize(stage.GetSize());
  view.ActivateLayout(0, stageSize, 0.5f);

  //Connect to signal scroll updated
  view.ScrollUpdatedSignal().Connect( &OnScrollUpdate );

  Property::Map attributes;
  view.DoAction("enableRefresh", attributes );
  gOnScrollUpdateCalled = true;
  view.SetProperty( ItemView::Property::LAYOUT_POSITION, 100.0f );
  application.SendNotification();
  application.Render(1000);
  DALI_TEST_EQUALS( gOnScrollUpdateCalled, true, TEST_LOCATION );


  view.DoAction("disableRefresh", attributes );
  gOnScrollUpdateCalled = false;
  view.SetProperty( ItemView::Property::LAYOUT_POSITION, 100.0f );
  application.SendNotification();
  application.Render(1000);

  DALI_TEST_EQUALS( gOnScrollUpdateCalled, false, TEST_LOCATION );

  END_TEST;
}

int UtcDaliItemViewWheelEvent(void)
{
  ToolkitTestApplication application;
  Dali::Integration::Scene stage = application.GetScene();

  // Create the ItemView actor
  TestItemFactory factory;
  ItemView view = ItemView::New( factory );

  // Create a grid layout and add it to ItemView
  ItemLayoutPtr gridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );
  view.AddLayout( *gridLayout );
  stage.Add( view );

  // Activate the grid layout so that the items will be created and added to ItemView
  Vector3 stageSize( stage.GetSize() );
  view.ActivateLayout (0, stageSize, 0.5f );

  //Connect to wheel event signal
  view.WheelEventSignal().Connect( &OnWheelEvent );

  DALI_TEST_CHECK( !gOnWheelEventCalled );

  // Render and notify
  application.Render();
  application.SendNotification();
  application.Render();
  application.SendNotification();

  // Perform a wheel event
  Dali::Integration::WheelEvent wheelEvent( Dali::Integration::WheelEvent::MOUSE_WHEEL, 0, 0u, Vector2( 10.0f, 10.0f ), 1, 1000u );
  application.ProcessEvent( wheelEvent );
  DALI_TEST_CHECK( gOnWheelEventCalled );

  END_TEST;
}
