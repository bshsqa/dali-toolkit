/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

// CLASS HEADER
#include "control-devel.h"

// EXTERNAL INCLUDES
#include <dali/public-api/animation/animation.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/controls/control/control-data-impl.h>
#include <dali-toolkit/public-api/controls/control-impl.h>
#include <dali-toolkit/devel-api/visual-factory/transition-data.h>

namespace Dali
{

namespace Toolkit
{

namespace DevelControl
{

ResourceReadySignalType&  ResourceReadySignal( Control& control )
{
  Internal::Control& internalControl = Toolkit::Internal::GetImplementation(control);
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( internalControl );

  return controlImpl.mResourceReadySignal;
}

bool IsResourceReady( const Control& control )
{
  const Internal::Control& internalControl = Toolkit::Internal::GetImplementation(control);
  const Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( internalControl );

  return controlImpl.IsResourceReady();
}

void RegisterVisual( Internal::Control& control, Dali::Property::Index index, Toolkit::Visual::Base& visual )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  controlImpl.RegisterVisual( index, visual );
}

void RegisterVisual( Internal::Control& control, Dali::Property::Index index, Toolkit::Visual::Base& visual, float depthIndex )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  controlImpl.RegisterVisual( index, visual, depthIndex );
}

void RegisterVisual( Internal::Control& control, Dali::Property::Index index, Toolkit::Visual::Base& visual, bool enabled )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  controlImpl.RegisterVisual( index, visual, enabled );
}

void RegisterVisual( Internal::Control& control, Dali::Property::Index index, Toolkit::Visual::Base& visual, bool enabled, float depthIndex )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  controlImpl.RegisterVisual( index, visual, enabled, depthIndex );
}

void UnregisterVisual( Internal::Control& control, Dali::Property::Index index )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  controlImpl.UnregisterVisual( index );
}

Toolkit::Visual::Base GetVisual( const Internal::Control& control, Dali::Property::Index index )
{
  const Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  return controlImpl.GetVisual( index );
}

void EnableVisual( Internal::Control& control, Dali::Property::Index index, bool enable )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  controlImpl.EnableVisual( index, enable );
}

bool IsVisualEnabled( const Internal::Control& control, Dali::Property::Index index )
{
  const Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  return controlImpl.IsVisualEnabled( index );
}

Dali::Animation CreateTransition( Internal::Control& control, const Toolkit::TransitionData& handle )
{
  Internal::Control::Impl& controlImpl = Internal::Control::Impl::Get( control );
  return controlImpl.CreateTransition( handle );
}


} // namespace DevelControl

} // namespace Toolkit

} // namespace Dali
