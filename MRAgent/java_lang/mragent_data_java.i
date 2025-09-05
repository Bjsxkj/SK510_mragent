
%module(directors="1") mragent_data_java

%typemap("javapackage") SWIGTYPE, SWIGTYPE *, SWIGTYPE & "com.sximi.mragent";

%{

#include <string>

#include <stdint.h>

#include "ImiMrTypes.h"

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
        System.loadLibrary("mragent_data_jni");
    }

%}

// 将SWIG构造函数设为公有,以便多模块使用
SWIG_JAVABODY_PROXY(public, public, SWIGTYPE)

%include <std_shared_ptr.i>
%include <std_string.i>
%include <std_map.i>
%include <std_vector.i>
%include <cpointer.i>
%include <carrays.i>
%include <arrays_java.i>
// %include <enum.i>
%include <typemaps.i> 

%include "smp_vector_java.i"
%include "bytebuffer_java.i"

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


%template(ImiFaceDetectDataList) std::vector<imimr::ImiFaceDetectData>;

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

list_2_array(imimr::ImiDeviceAttribute, ImiDeviceAttribute)
list_2_array(imimr::ImiFaceDetectData, ImiFaceDetectData)


%ignore imimr::ImiForegroundData::foregroundData;
%ignore imimr::ImiForegroundData::size;
%ignore imimr::ImiMattingData::mattingData;
%ignore imimr::ImiMattingData::size;


%extend imimr::ImiBodySkeleton {

    /// @brief 访问第index个骨架，无拷贝
    const ImiBodySkeletonData* at(int index) {
        if(index<0) index = 0;
        if(index>5) index = 5;
        return &self->skeletonDatas[index];

    }

    /// @brief 访问第index个骨架，无拷贝
    const ImiBodySkeletonData* atBodySkeleton(int index){
        return & self->skeletonDatas[index];
    }
}

typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

member_fix_base_array(imimr::ImiBodySkeletonData::ImiBodySkeletonPositionTrackState, jintArray, int, GetIntArrayElements, SetIntArrayRegion, NewIntArray)
member_fix_base_array(imimr::ImiBodySkeletonData::ImiSkeletonOrientationState, jintArray, int, GetIntArrayElements, SetIntArrayRegion, NewIntArray)
member_fix_base_array(imimr::ImiMoveTrackData::ButtonSate, jintArray, int, GetIntArrayElements, SetIntArrayRegion, NewIntArray)

member_fix_array(imimr::ImiVector4, ImiVector4, "com/sximi/mragent/ImiVector4")
member_fix_array(imimr::ImiPoint2D, ImiPoint2D, "com/sximi/mragent/ImiPoint2D")
member_fix_array(imimr::ImiSkeletonOrientationData, ImiSkeletonOrientationData, "com/sximi/mragent/ImiSkeletonOrientationData")
member_fix_array(imimr::ImiBodySkeletonData, ImiBodySkeletonData, "com/sximi/mragent/ImiBodySkeletonData")
member_fix_array(imimr::ImiHandSkeletonData, ImiHandSkeletonData, "com/sximi/mragent/ImiHandSkeletonData")
member_fix_array(imimr::ImiMoveTrackData, ImiMoveTrackData, "com/sximi/mragent/ImiMoveTrackData")


%include "enumtypeunsafe.swg"
%javaconst(1);

%include "ImiMrTypes.h"






