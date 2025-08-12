
%module(directors="1") mragent_java

%typemap("javapackage") SWIGTYPE, SWIGTYPE *, SWIGTYPE & "com.sximi.mragent";

%{

#include <string>

#include <stdint.h>

#include "ImiMrTypes.h"
#include "ImiMrAgent.h"

%}

%init %{  
    // JNI C++ init code

    // JNIEXPORT jlong JNICALL Java_com_sximi_mr_AndroidAppContext_getSwigCPtr(JNIEnv *env, jobject obj) {
    //     jclass clazz = env->FindClass("com/sximi/mr/AppContext");
    //     jfieldID fieldID = env->GetFieldID(clazz, "swigCPtr", "J");
    //     return env->GetLongField(obj, fieldID);
    // }
%}  

// 额外的代码
%pragma(java) jniclasscode=%{
    //java code
    static {
        System.loadLibrary("mragent_jni");
    }

%}

// 引进公共数据类的定义
%import "mragent_data_java.i"
//增加import 解决不同模块包名不同问题
%typemap(javaimports) SWIGTYPE %{
    import com.sximi.mragent.*;
    import com.sximi.mragent.ImiBodySkeleton;
%}

%include <std_shared_ptr.i>
%include <std_string.i>
%include <std_map.i>
%include <std_vector.i>
%include <cpointer.i>
%include <carrays.i>
%include <arrays_java.i>
// %include <enum.i>
%include <typemaps.i> 


%include "mragent_data_java.i"

//%apply void* VOID_INT_PTR { void* }

// 定义 void* 到 Java long 的类型映射
%typemap(jstype) void* "long"
%typemap(jtype) void* "long"
%typemap(jni) void* "jlong"
// 输入参数类型映射
%typemap(in) void* (void* temp) {
    temp = (void*)$input;
    $1 = temp;
}
// 返回值类型映射
%typemap(out) void* {
    $result = (jlong)$1;
}
%typemap(javaout) void* {
    return $jnicall;
}



// %apply (unsigned char OUTPUT[])  { (unsigned char* array_ref)};
// %apply (int OUTPUT[])  { (int* array_out)};
// %apply (double OUTPUT[])  { (double* array_out)};

map_buffer_ref(array_ref, array_len)

// %std::shared_ptr(imimr::IImiDevice);

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
// %shared_ptr(imimr::IImiDeviceProperty);


// %template(ImiFaceDetectDataList) std::vector<imimr::ImiFaceDetectData>;

// ARGOUT_ARRAY1
map_java_bytebuffer(array_ptr, array_len)
// %apply ( uint8_t* IN_ARRAY1, int DIM1){ (uint8_t* array_ptr, int array_len) };

// %feature("director") IMyCallback;
%define list_2_array(cname, javaname)
%extend std::vector<cname> {
%proxycode %{
    public javaname[] toArray(){
        int n = size();
        javaname[] ret = new javaname[n];
        for(int i=0; i<n; i++){
            ret[i] = get(i);
        }
        return ret;
    }
%}
}
%enddef

list_2_array(imimr::ImiBaseDevice, IImiBaseDevice)
// list_2_array(imimr::ImiDeviceAttribute, ImiDeviceAttribute)
// list_2_array(imimr::ImiFaceDetectData, ImiFaceDetectData)


// %ignore imimr::ImiForegroundData::foregroundData;
// %ignore imimr::ImiForegroundData::size;
// %ignore imimr::ImiMattingData::mattingData;
// %ignore imimr::ImiMattingData::size;


// %extend imimr::ImiBodySkeleton {

//     /// @brief 访问第index个骨架，无拷贝
//     const ImiBodySkeletonData* at(int index) {
//         if(index<0) index = 0;
//         if(index>5) index = 5;
//         return &self->skeletonDatas[index];

//     }

//     /// @brief 访问第index个骨架，无拷贝
//     const ImiBodySkeletonData* atBodySkeleton(int index){
//         return & self->skeletonDatas[index];
//     }
// }


%extend imimr::IImiBaseSensor {
// %rename("readFrame0") readFrame; 
// %proxycode %{
//     public readFrame(self, timeout):
//     //     frame = self.readFrame0(timeout)
//     //     return FrameCast(frame)
// %}
}

%extend imimr::IImiBaseDevice {
// %rename("getSensor0") getSensor; 
// %proxycode %{
//     public IImiBaseSensor getSensor(int type):
//         sensor = getSensor0(type);
//         return (sensor);
// %}
}

typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

// %apply ( long ) { void* }

// %typemap(jni)       (void*) "jlong" 
// %typemap(jtype)     (void*) "long" 
// %typemap(jstype)    (void*) "long" 
// %typemap(javaout)   (void*) { 
//     return $jnicall; 
// }
// %typemap(out)   (void*) { 
//     $result = (jlong) $1;
// }

// %apply (float[ANY])  { (float* array_out)};


// member_fix_base_array(imimr::ImiBodySkeletonData::ImiBodySkeletonPositionTrackState, jintArray, int, GetIntArrayElements, SetIntArrayRegion, NewIntArray)
// member_fix_base_array(imimr::ImiBodySkeletonData::ImiSkeletonOrientationState, jintArray, int, GetIntArrayElements, SetIntArrayRegion, NewIntArray)
// member_fix_base_array(imimr::ImiMoveTrackData::ButtonSate, jintArray, int, GetIntArrayElements, SetIntArrayRegion, NewIntArray)

// member_fix_array(imimr::ImiVector4, ImiVector4, "com/sximi/mragent/ImiVector4")
// member_fix_array(imimr::ImiPoint2D, ImiPoint2D, "com/sximi/mragent/ImiPoint2D")
// member_fix_array(imimr::ImiSkeletonOrientationData, ImiSkeletonOrientationData, "com/sximi/mragent/ImiSkeletonOrientationData")
// member_fix_array(imimr::ImiBodySkeletonData, ImiBodySkeletonData, "com/sximi/mragent/ImiBodySkeletonData")
// member_fix_array(imimr::ImiHandSkeletonData, ImiHandSkeletonData, "com/sximi/mragent/ImiHandSkeletonData")
// member_fix_array(imimr::ImiMoveTrackData, ImiMoveTrackData, "com/sximi/mragent/ImiMoveTrackData")


%include "enumtypeunsafe.swg"
%javaconst(1);


%feature("director") imimr::AppContext;
%feature("director") imimr::IDeviceStateListener;
%feature("director") imimr::MrAgentExceptionHandler;



// %include "ImiMrTypes.h"

%include "ImiMrAgent.h"

%template(IInt32Property) imimr::IBaseProperty<int>;
%template(IInt16Property) imimr::IBaseProperty<short>;
%template(IInt64Property) imimr::IBaseProperty<long>;
%template(IFloatProperty) imimr::IBaseProperty<float>;
%template(IInt8Property) imimr::IBaseProperty<unsigned char>;
%template(IImiCameraIntrinsicProperty) imimr::IBaseProperty<imimr::ImiCameraIntrinsic>;

%extend imimr::IImiColorFrame {
    %proxycode %{
        private static native java.nio.ByteBuffer toByteBuffer(long address, int length);
        public java.nio.ByteBuffer getData() {
            return toByteBuffer(this.getDataPointer(), this.getDataSize());
        }
    %}
}
