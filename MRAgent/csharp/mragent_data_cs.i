
%module(directors="1") mragent_data_cs

// , csbegin="using System.Runtime.CompilerServices;\n[assembly: InternalsVisibleTo(\"mragent_cs_csharp\")]  \n \n"

%{

#include <string>

// #define USE_ANDROID_API 1

#include "ImiMrTypes.h"

%}

// cpp 初始化
%init %{  
    // cpp init int abc=123;

    //public delegate int PerformCalculation(int x, int y);
%}  


%proxycode %{
    // my code
%}

// %insert("headers") {
//     // This is a custom added line
// };

// %insert("proxycode") %{
//     [assembly: InternalsVisibleTo("mragent_cs_csharp")]
// %}

// %typemap(cscode) SWIGTYPE "// import [assembly: InternalsVisibleTo(\"mragent_cs_csharp\")] \n"

// 使用 %typemap 在 namespace 之前插入自定义代码

// %csmethodmodifiers $modulePINVOKE::SetPendingApplicationException %{
//     // abc
// %}

//将SWIG构造函数设为公有
// SWIG_CSBODY_PROXY(public, public, SWIGTYPE)
// SWIG_CSBODY_TYPEWRAPPER(public, public, public, SWIGTYPE)

// 将SWIG构造函数设为公有,以便多模块使用
#define PUBLIC_BUT_HIDDEN [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)] public 
SWIG_CSBODY_PROXY(PUBLIC_BUT_HIDDEN, PUBLIC_BUT_HIDDEN, SWIGTYPE)

// %typemap(csmethodmodifiers) SWIGTYPE %{ public static %}


%csattributes  SWIG_CSharpSetPendingException(const char *msg)  "[MonoPInvokeCallback(typeof(ExceptionDelegate))]"

// 添加自定义的using语句
%pragma(csharp) imclassimports=%{
    // [MonoPInvokeCallback(typeof(ExceptionDelegate))]
    using System;
    // using AOT;
//     using MyCustomNamespace; // 自定义的using语句

%}

%typemap(csharpwrapper) SWIGTYPE %{
    // 自定义的 C# 代码
    // 继续生成原始的 C# 包装代码
    namespace %csharpnamespace {
        %csharpmodule {
            %typemap(csharpclass) *;
        }
    }
%}


// %typemap(csclassmodifiers) SWIGTYPE "//允许访问 internal 方法\n [assembly: InternalsVisibleTo(\"mragent_cs_csharp\")] \n public class"

// 自定义 C# 构造函数的访问属性
// %typemap(csharpnewcomposite) SWIGTYPE "%public% %typemap(csharpclassmodifiers) MyClass% %cshapename%(%parameters%) {\n"
// "    %typemap(csharpnewcode) MyClass%;\n"
// "}";

// %typemap(csbody) SWIGTYPE %{
//     private global::System.Runtime.InteropServices.HandleRef swigCPtr;
//     protected bool swigCMemOwn;

//   public $csclassname (global::System.IntPtr cPtr, bool cMemoryOwn) : base( $modulePINVOKE.$csclassname_SWIGUpcast(cPtr), cMemoryOwn) {
//     swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
//   }

//   public static global::System.Runtime.InteropServices.HandleRef getCPtr($csclassname obj) {
//     return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
//   }
// %}

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



// // %shared_ptr(imimr::ImiBodySkeleton);
// %shared_ptr(imimr::ImiHandSkeleton);
// %shared_ptr(imimr::ImiFaceFeatureData);

// %shared_ptr(imimr::IImiActionSensor);
// %shared_ptr(imimr::IImiActionFrame);

// %template(ImiDeviceList) std::vector<std::shared_ptr<imimr::IImiBaseDevice>>;
// %template(ImiDeviceAttributeList) std::vector<imimr::ImiDeviceAttribute>;
%template(ImiFaceDetectDataList) std::vector<imimr::ImiFaceDetectData>;

// %shared_ptr(ABC);
// [global::System.Runtime.InteropServices.In, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)]


%apply (unsigned char OUTPUT[])  { (unsigned char* array_ref)};
%apply (int OUTPUT[])  { (int* array_out)};
%apply (double OUTPUT[])  { (double* array_out)};

%include "smp_vector_cs.i"

// %csmethodmodifiers SWIGTYPE::ToString() "public override ";

// %extend imimr::IImiBaseDevice {
// %proxycode %{
//     public bool isNull(){
//         return swigCPtr.Handle == global::System.IntPtr.Zero;
//     }
// %}
// }



%ignore imimr::ImiForegroundData::foregroundData;
%ignore imimr::ImiForegroundData::size;
%ignore imimr::ImiMattingData::mattingData;
%ignore imimr::ImiMattingData::size;

typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

smp_base_array(int,int)
smp_base_array(imimr::ImiBodySkeletonData::ImiBodySkeletonPositionTrackState,int)
smp_base_array(imimr::ImiBodySkeletonData::ImiSkeletonOrientationState,int)
smp_base_array(imimr::ImiMoveTrackData::ButtonSate,int)
smp_base_array(float,float)


struct_array_to_list(imimr::ImiFaceDetectData, ImiFaceDetectData, landmark, 5, imimr::ImiPoint2D, ImiPoint2DList)





struct_array_to_list(imimr::ImiBodySkeletonData, ImiBodySkeletonData, skeletonPositions, 20, imimr::ImiVector4, ImiVector4List)
struct_array_to_list(imimr::ImiBodySkeletonData, ImiBodySkeletonData, skeletonOrientations, 20, imimr::ImiSkeletonOrientationData, ImiSkeletonOrientationDataList)

struct_array_to_list(imimr::ImiHandSkeletonData, ImiHandSkeletonData, skeletonPositions, 21, imimr::ImiVector4, ImiVector4List)
struct_array_to_list(imimr::ImiHandSkeletonData, ImiHandSkeletonData, skeletonOrientations, 21, imimr::ImiSkeletonOrientationData, ImiSkeletonOrientationDataList)


struct_array_to_list(imimr::ImiMoveData, ImiMoveData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)
struct_array_to_list(imimr::ImiPositionTrackData, ImiPositionTrackData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)

struct_array_to_list(imimr::ImiBodySkeleton, ImiBodySkeleton, skeletonDatas, 6, imimr::ImiBodySkeletonData, ImiBodySkeletonDataList)
// struct_array_to_list(imimr::ImiBodySkeleton, skeletonDatas, 6, imimr::ImiBodySkeletonData, ImiBodySkeletonDataList)
// struct_array_to_list(imimr::ImiHandSkeleton, handSkeletonDatas, 2, imimr::ImiHandSkeletonData, ImiHandSkeletonDataList)
// struct_array_to_list(imimr::ImiMoveData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)
// struct_array_to_list(imimr::ImiPositionTrackData, moveTrackData, 4, imimr::ImiMoveTrackData, ImiMoveTrackDataList)

// %feature("director") imimr::AppContext;
%feature("director") imimr::IDeviceStateListener;

%include "ImiMrTypes.h"

%extend imimr::ImiBodySkeleton {
    const ImiBodySkeletonData* atBodySkeleton(int index){
        return & self->skeletonDatas[index];
    }
}



