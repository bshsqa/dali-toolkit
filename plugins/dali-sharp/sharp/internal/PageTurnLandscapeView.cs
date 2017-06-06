/** Copyright (c) 2017 Samsung Electronics Co., Ltd.
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
// This File has been auto-generated by SWIG and then modified using DALi Ruby Scripts
//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.10
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Dali {

internal class PageTurnLandscapeView : PageTurnView {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal PageTurnLandscapeView(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NDalicPINVOKE.PageTurnLandscapeView_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(PageTurnLandscapeView obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~PageTurnLandscapeView() {
    DisposeQueue.Instance.Add(this);
  }

  public override void Dispose() {
    if (!Window.IsInstalled()) {
      DisposeQueue.Instance.Add(this);
      return;
    }

    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicPINVOKE.delete_PageTurnLandscapeView(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }


  public PageTurnLandscapeView (PageFactory pageFactory, Vector2 pageSize) : this (NDalicPINVOKE.PageTurnLandscapeView_New(PageFactory.getCPtr(pageFactory), Vector2.getCPtr(pageSize)), true) {
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();

  }
  public PageTurnLandscapeView(PageTurnLandscapeView pageTurnLandscapeView) : this(NDalicPINVOKE.new_PageTurnLandscapeView__SWIG_1(PageTurnLandscapeView.getCPtr(pageTurnLandscapeView)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public PageTurnLandscapeView Assign(PageTurnLandscapeView pageTurnLandscapeView) {
    PageTurnLandscapeView ret = new PageTurnLandscapeView(NDalicPINVOKE.PageTurnLandscapeView_Assign(swigCPtr, PageTurnLandscapeView.getCPtr(pageTurnLandscapeView)), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public new static PageTurnLandscapeView DownCast(BaseHandle handle) {
    PageTurnLandscapeView ret = new PageTurnLandscapeView(NDalicPINVOKE.PageTurnLandscapeView_DownCast(BaseHandle.getCPtr(handle)), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
