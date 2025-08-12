
%module(directors="1") messagecore_java

%{

#include <string>

#include <stdint.h>

#include "MessageCoreApi.h"

using namespace imimr;
using namespace mrcore;

%}

%feature("director") MessageCore; // 启用Director模式
%exception {
    try {
        $action
    } catch (const std::exception& e) {
        jclass clazz = jenv->FindClass("java/lang/RuntimeException");
        jenv->ThrowNew(clazz, e.what());
        return $null;
    }
}

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
        System.loadLibrary("messagecore_jni");
    }

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

%include "smp_vector_java.i"
%include "bytebuffer_java.i"

// %apply void* VOID_INT_PTR { void* }

// %apply (unsigned char OUTPUT[])  { (unsigned char* array_ref)};
// %apply (int OUTPUT[])  { (int* array_out)};
// %apply (double OUTPUT[])  { (double* array_out)};

map_buffer_ref(array_ref, array_len)

// %std::shared_ptr(imimr::IImiDevice);

%shared_ptr(imimr::IImiBaseDevice);
%ignore imimr::IImiBaseDevice;
// %shared_ptr(mrcore::IDataSever);

// ARGOUT_ARRAY1
map_java_bytebuffer(srcData, dataLen)
map_java_bytebuffer(destData, dataLen)
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




%include "enumtypeunsafe.swg"
%javaconst(1);

%feature("director") mrcore::MessageCore;
// %feature("director") mrcore::IDataSever;

namespace imimr {
    class IImiBaseDevice;
    class IImiBaseSensor;
    class IImiBaseFrame;
typedef std::shared_ptr<imimr::IImiBaseDevice> ImiBaseDevice;
typedef std::shared_ptr<imimr::IImiBaseSensor> ImiBaseSensor;
typedef std::shared_ptr<imimr::IImiBaseeFrame> ImiBaseeFrame;
}


%define swig_class(cclazz, javaclazz, javaclazzpath)
%ignore cclazz;
%typemap(jni) (cclazz)  "jobject"
%typemap(jtype) (cclazz) "javaclazz"
%typemap(jstype) (cclazz) "javaclazz"
%typemap(javain, pre="") (cclazz)
"$javainput"
%typemap(in) (cclazz) {
    jclass clazz = JCALL1(FindClass, jenv, javaclazzpath);
    jmethodID mid = JCALL3(GetStaticMethodID, jenv, clazz, "getCPtr", "(L" javaclazzpath ";)J");
    jlong cptr = JCALL3(CallStaticLongMethod, jenv, clazz, mid, $input);
    $1 = *((cclazz*) cptr);
}

//%typemap(directorin, descriptor="J") (cclazz) "$input = (jlong) $1"
%typemap(directorin, descriptor = "L" javaclazzpath ";", noblock=1) (cclazz) {
  $input = 0;
  if ($1) {
    jclass clazz = JCALL1(FindClass, jenv, javaclazzpath);
    jmethodID mid = JCALL3(GetMethodID, jenv, clazz, "<init>", "(JB)V");
    jobject o = JCALL4(NewObject, jenv , clazz, mid, ((jlong)&($1)), (jboolean)0);
    $input = o;
//     $input = JCALL1(NewStringUTF, jenv, (const char *)$1);
//     if (!$input) return $null;
  }
}

%typemap(javadirectorin) (cclazz) "$jniinput"

%enddef

swig_class(std::shared_ptr<imimr::IImiBaseDevice>, com.sximi.mr.IImiBaseDevice, "com/sximi/mr/IImiBaseDevice")
swig_class(std::shared_ptr<imimr::IImiBaseFrame>, com.sximi.mr.IImiBaseFrame, "com/sximi/mr/IImiBaseFrame")

// swig_class(std::shared_ptr<imimr::ImiBodySkeleton>, com.sximi.mr.ImiBodySkeleton, "com/sximi/mr/ImiBodySkeleton")

// 映射本模块没有的数据类
%define imi_data_in(cname, javaname)
%ignore cname;
%typemap(jni) (const cname&)  "jlong"
%typemap(jtype) (const cname&) "long"
%typemap(jstype) (const cname&) "javaname"
%typemap(in) (const cname&) {
    $1 = ((cname*)$input);
}
%typemap(javain) (const cname&) "$javainput.getHandle()"
%enddef

imi_data_in(imimr::ImiBodySkeleton, com.sximi.mr.ImiBodySkeleton)

map_data_buffer_ref(::DataBuffer, data, len)

%include "MessageCoreApi.h"




