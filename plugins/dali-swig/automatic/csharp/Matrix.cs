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

public class Matrix : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Matrix(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Matrix obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~Matrix() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicPINVOKE.delete_Matrix(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public static Vector4 operator*(Matrix arg1, Vector4 arg2) {
    return arg1.Multiply(arg2);
  }

  public Matrix() : this(NDalicPINVOKE.new_Matrix__SWIG_0(), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Matrix(bool initialize) : this(NDalicPINVOKE.new_Matrix__SWIG_1(initialize), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Matrix(float[] array) : this(NDalicPINVOKE.new_Matrix__SWIG_2(array), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Matrix(Quaternion rotation) : this(NDalicPINVOKE.new_Matrix__SWIG_3(Quaternion.getCPtr(rotation)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Matrix(Matrix matrix) : this(NDalicPINVOKE.new_Matrix__SWIG_4(Matrix.getCPtr(matrix)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Matrix Assign(Matrix matrix) {
    Matrix ret = new Matrix(NDalicPINVOKE.Matrix_Assign(swigCPtr, Matrix.getCPtr(matrix)), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static Matrix IDENTITY {
    get {
      global::System.IntPtr cPtr = NDalicPINVOKE.Matrix_IDENTITY_get();
      Matrix ret = (cPtr == global::System.IntPtr.Zero) ? null : new Matrix(cPtr, false);
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public void SetIdentity() {
    NDalicPINVOKE.Matrix_SetIdentity(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetIdentityAndScale(Vector3 scale) {
    NDalicPINVOKE.Matrix_SetIdentityAndScale(swigCPtr, Vector3.getCPtr(scale));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InvertTransform(Matrix result) {
    NDalicPINVOKE.Matrix_InvertTransform(swigCPtr, Matrix.getCPtr(result));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Invert() {
    bool ret = NDalicPINVOKE.Matrix_Invert(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Transpose() {
    NDalicPINVOKE.Matrix_Transpose(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Vector3 GetXAxis() {
    Vector3 ret = new Vector3(NDalicPINVOKE.Matrix_GetXAxis(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector3 GetYAxis() {
    Vector3 ret = new Vector3(NDalicPINVOKE.Matrix_GetYAxis(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector3 GetZAxis() {
    Vector3 ret = new Vector3(NDalicPINVOKE.Matrix_GetZAxis(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetXAxis(Vector3 axis) {
    NDalicPINVOKE.Matrix_SetXAxis(swigCPtr, Vector3.getCPtr(axis));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetYAxis(Vector3 axis) {
    NDalicPINVOKE.Matrix_SetYAxis(swigCPtr, Vector3.getCPtr(axis));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetZAxis(Vector3 axis) {
    NDalicPINVOKE.Matrix_SetZAxis(swigCPtr, Vector3.getCPtr(axis));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Vector4 GetTranslation() {
    Vector4 ret = new Vector4(NDalicPINVOKE.Matrix_GetTranslation(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector3 GetTranslation3() {
    Vector3 ret = new Vector3(NDalicPINVOKE.Matrix_GetTranslation3(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetTranslation(Vector4 translation) {
    NDalicPINVOKE.Matrix_SetTranslation__SWIG_0(swigCPtr, Vector4.getCPtr(translation));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetTranslation(Vector3 translation) {
    NDalicPINVOKE.Matrix_SetTranslation__SWIG_1(swigCPtr, Vector3.getCPtr(translation));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void OrthoNormalize() {
    NDalicPINVOKE.Matrix_OrthoNormalize(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public SWIGTYPE_p_float AsFloat() {
    global::System.IntPtr cPtr = NDalicPINVOKE.Matrix_AsFloat__SWIG_0(swigCPtr);
    SWIGTYPE_p_float ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_float(cPtr, false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static void Multiply(Matrix result, Matrix lhs, Matrix rhs) {
    NDalicPINVOKE.Matrix_Multiply__SWIG_0(Matrix.getCPtr(result), Matrix.getCPtr(lhs), Matrix.getCPtr(rhs));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public static void Multiply(Matrix result, Matrix lhs, Quaternion rhs) {
    NDalicPINVOKE.Matrix_Multiply__SWIG_1(Matrix.getCPtr(result), Matrix.getCPtr(lhs), Quaternion.getCPtr(rhs));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Vector4 Multiply(Vector4 rhs) {
    Vector4 ret = new Vector4(NDalicPINVOKE.Matrix_Multiply__SWIG_2(swigCPtr, Vector4.getCPtr(rhs)), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool EqualTo(Matrix rhs) {
    bool ret = NDalicPINVOKE.Matrix_EqualTo(swigCPtr, Matrix.getCPtr(rhs));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool NotEqualTo(Matrix rhs) {
    bool ret = NDalicPINVOKE.Matrix_NotEqualTo(swigCPtr, Matrix.getCPtr(rhs));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetTransformComponents(Vector3 scale, Quaternion rotation, Vector3 translation) {
    NDalicPINVOKE.Matrix_SetTransformComponents(swigCPtr, Vector3.getCPtr(scale), Quaternion.getCPtr(rotation), Vector3.getCPtr(translation));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetInverseTransformComponents(Vector3 scale, Quaternion rotation, Vector3 translation) {
    NDalicPINVOKE.Matrix_SetInverseTransformComponents__SWIG_0(swigCPtr, Vector3.getCPtr(scale), Quaternion.getCPtr(rotation), Vector3.getCPtr(translation));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetInverseTransformComponents(Vector3 xAxis, Vector3 yAxis, Vector3 zAxis, Vector3 translation) {
    NDalicPINVOKE.Matrix_SetInverseTransformComponents__SWIG_1(swigCPtr, Vector3.getCPtr(xAxis), Vector3.getCPtr(yAxis), Vector3.getCPtr(zAxis), Vector3.getCPtr(translation));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void GetTransformComponents(Vector3 position, Quaternion rotation, Vector3 scale) {
    NDalicPINVOKE.Matrix_GetTransformComponents(swigCPtr, Vector3.getCPtr(position), Quaternion.getCPtr(rotation), Vector3.getCPtr(scale));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
