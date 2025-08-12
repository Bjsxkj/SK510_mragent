
%module(directors="1") mragent_cs

%{

#include <string>

// #define USE_ANDROID_API 1

#include "ImiMrAgent.h"

%}

// cpp 初始化
%init %{  
    // cpp init int abc=123;

    //public delegate int PerformCalculation(int x, int y);
%}  

// 引进公共数据类的定义
%import "mragent_data_cs.i"


//增加新的C#代码
// %extend imimr::IIDeviceStateListener {
// %proxycode  %{
//     public delegate void OnDeviceStateChanged(ImiDeviceState state, String deviceInfo);
// %}
// }

// %extend ABC {
//     // %rename("toString") str;

// //增加新的C#代码
// %proxycode %{
//     public string  getInfo1() {
//         return "I am C#";
//     }
//     %}
// }

%include <typemaps.i>
%include <std_string.i>
%include <std_shared_ptr.i>
%include <std_vector.i>
%include <arrays_csharp.i>
// %include "arrays_csharp.i"
// ARGOUT_ARRAY1
// %template(AbcList) std::vector<ABC>;

// %apply void* VOID_INT_PTR { void* }
// 定义 void* 到 C# IntPtr 的类型映射
%typemap(ctype) void*  "void *"
%typemap(imtype) void* "global::System.IntPtr"
// %typemap(ctype , out="void* ") (std::vector<cname>)  "int $1_len, cname *"
// %typemap(imtype, out="global::System.IntPtr") (std::vector<cname>) "int $1_len, [global::System.Runtime.InteropServices.In, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)] csname[]"
%typemap(cstype) void* "global::System.IntPtr"
// %typemap(in) void* (void* temp) {
//     temp = (void*)$input;
//     $1 = temp;
// }
%typemap(out) void* {
    $result = $1; //(global::System.IntPtr)
}
%typemap(csout) void* {
    return $imcall;
}
%typemap(ctype) unsigned long "unsigned long"
%typemap(imtype) unsigned long "ulong"
%typemap(cstype) unsigned long "ulong"
%typemap(csout) unsigned long {
    ulong ret = mragent_csPINVOKE.IImiImageFrame_getDataPointer(swigCPtr);
    if (mragent_csPINVOKE.SWIGPendingException.Pending) throw mragent_csPINVOKE.SWIGPendingException.Retrieve();
    return ret;
}

// 有回调函数的
%feature("director") imimr::IDeviceStateListener;

%shared_ptr(imimr::IImiBaseDevice);
%shared_ptr(imimr::IImiBaseSensor);
%shared_ptr(imimr::IImiBaseFrame);
%shared_ptr(imimr::IImiImageFrame);
%shared_ptr(imimr::IImiColorSensor);
%shared_ptr(imimr::IImiColorFrame);
%shared_ptr(imimr::IImiDepthSensor);
%shared_ptr(imimr::IImiDepthFrame);
%shared_ptr(imimr::IImiIRSensor);
%shared_ptr(imimr::IImiIRFrame);
%shared_ptr(imimr::IImiFaceDetectSensor);
%shared_ptr(imimr::IImiFaceDetectFrame);
%shared_ptr(imimr::IImiUserTrackSensor);
%shared_ptr(imimr::IImiUserTrackFrame);
%shared_ptr(imimr::IImiMoveTrackSensor);
%shared_ptr(imimr::IImiMoveTrackFrame);
%shared_ptr(imimr::IImiPositionTrackSensor);
%shared_ptr(imimr::IImiPositionTrackFrame);
%shared_ptr(imimr::IDeviceStateListener);


// %shared_ptr(imimr::ImiBodySkeleton);
%shared_ptr(imimr::ImiHandSkeleton);
%shared_ptr(imimr::ImiFaceFeatureData);

//%shared_ptr(imimr::IImiActionSensor);
//%shared_ptr(imimr::IImiActionFrame);

// %template(ImiDeviceList) std::vector<std::shared_ptr<imimr::IImiBaseDevice>>;
// %template(ImiDeviceAttributeList) std::vector<imimr::ImiDeviceAttribute>;
// %template(ImiFaceDetectDataList) std::vector<imimr::ImiFaceDetectData>;

// %shared_ptr(ABC);
// [global::System.Runtime.InteropServices.In, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)]


%apply (unsigned char OUTPUT[])  { (unsigned char* array_ref)};
%apply (int OUTPUT[])  { (int* array_out)};
%apply (double OUTPUT[])  { (double* array_out)};

%include "mragent_data_cs.i"

// %csmethodmodifiers SWIGTYPE::ToString() "public override ";

%extend imimr::IImiBaseDevice {
%proxycode %{
    public bool isNull(){
        return swigCPtr.Handle == global::System.IntPtr.Zero;
    }
%}
}

%extend imimr::ImiContext {
//     %rename("createDevice0") createDevice;

// %proxycode %{
//     public IImiBaseDevice createDevice(ImiDeviceAttribute deviceAttribue){
//         IImiBaseDevice r = createDevice0(deviceAttribue);
//         return (r !=null && !r.isNull())? r : null;
//     }
//     //     frame = self.readFrame0(timeout)
//     //     return FrameCast(frame)
// %}

}

// %ignore imimr::ImiForegroundData::foregroundData;
// %ignore imimr::ImiForegroundData::size;
// %ignore imimr::ImiMattingData::mattingData;
// %ignore imimr::ImiMattingData::size;

// typedef int int32_t;
// typedef unsigned int uint32_t;
// typedef unsigned char uint8_t;

// smp_base_array(int,int)
// smp_base_array(imimr::ImiBodySkeletonData::ImiBodySkeletonPositionTrackState,int)
// smp_base_array(imimr::ImiBodySkeletonData::ImiSkeletonOrientationState,int)
// smp_base_array(imimr::ImiMoveTrackData::ButtonSate,int)
// smp_base_array(float,float)


// struct_array_to_list(imimr::ImiFaceDetectData, ImiFaceDetectData, landmark, 5, imimr::ImiPoint2D, ImiPoint2DList)





// struct_array_to_list(imimr::ImiBodySkeletonData, ImiBodySkeletonData, skeletonPositions, 20, imimr::ImiVector4, ImiVector4List)
// struct_array_to_list(imimr::ImiBodySkeletonData, ImiBodySkeletonData, skeletonOrientations, 20, imimr::ImiSkeletonOrientationData, ImiSkeletonOrientationDataList)

// struct_array_to_list(imimr::ImiHandSkeletonData, ImiHandSkeletonData, skeletonPositions, 21, imimr::ImiVector4, ImiVector4List)
// struct_array_to_list(imimr::ImiHandSkeletonData, ImiHandSkeletonData, skeletonOrientations, 21, imimr::ImiSkeletonOrientationData, ImiSkeletonOrientationDataList)


// struct_array_to_list(imimr::ImiMoveData, ImiMoveData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)
// struct_array_to_list(imimr::ImiPositionTrackData, ImiPositionTrackData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)

// struct_array_to_list(imimr::ImiBodySkeleton, ImiBodySkeleton, skeletonDatas, 6, imimr::ImiBodySkeletonData, ImiBodySkeletonDataList)
// struct_array_to_list(imimr::ImiBodySkeleton, skeletonDatas, 6, imimr::ImiBodySkeletonData, ImiBodySkeletonDataList)
// struct_array_to_list(imimr::ImiHandSkeleton, handSkeletonDatas, 2, imimr::ImiHandSkeletonData, ImiHandSkeletonDataList)
// struct_array_to_list(imimr::ImiMoveData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)
// struct_array_to_list(imimr::ImiPositionTrackData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)

%feature("director") imimr::AppContext;
%feature("director") imimr::IDeviceStateListener;

// %include "ImiMrTypes.h"

// %extend imimr::ImiBodySkeleton {
//     const ImiBodySkeletonData* atBodySkeleton(int index){
//         return & self->skeletonDatas[index];
//     }
// }



%include "ImiMrAgent.h"
%template(IInt32Property) imimr::IBaseProperty<int>;
%template(IInt16Property) imimr::IBaseProperty<short>;
%template(IInt64Property) imimr::IBaseProperty<long>;
%template(IFloatProperty) imimr::IBaseProperty<float>;
%template(IInt8Property) imimr::IBaseProperty<unsigned char>;
%template(IImiCameraIntrinsicProperty) imimr::IBaseProperty<imimr::ImiCameraIntrinsic>;
