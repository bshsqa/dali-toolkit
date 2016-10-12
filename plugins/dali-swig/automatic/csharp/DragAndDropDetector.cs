//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.9
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Dali {

public class DragAndDropDetector : BaseHandle {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal DragAndDropDetector(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NDalicPINVOKE.DragAndDropDetector_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(DragAndDropDetector obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~DragAndDropDetector() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicPINVOKE.delete_DragAndDropDetector(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public DragAndDropDetector() : this(NDalicPINVOKE.new_DragAndDropDetector(), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public string GetContent() {
    string ret = NDalicPINVOKE.DragAndDropDetector_GetContent(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector2 GetCurrentScreenPosition() {
    Vector2 ret = new Vector2(NDalicPINVOKE.DragAndDropDetector_GetCurrentScreenPosition(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t EnteredSignal() {
    SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t ret = new SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t(NDalicPINVOKE.DragAndDropDetector_EnteredSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t ExitedSignal() {
    SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t ret = new SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t(NDalicPINVOKE.DragAndDropDetector_ExitedSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t MovedSignal() {
    SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t ret = new SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t(NDalicPINVOKE.DragAndDropDetector_MovedSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t DroppedSignal() {
    SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t ret = new SWIGTYPE_p_Dali__SignalT_void_fDali__DragAndDropDetectorF_t(NDalicPINVOKE.DragAndDropDetector_DroppedSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
