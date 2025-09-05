%module(directors="1") test_part_api_jni

%{

#include <string>

// #define USE_ANDROID_API 1

#include "ImiMrAgent.h"
#include "part_api.h"

%}

// cpp 初始化
%init %{  
    // cpp init int abc=123;

    //public delegate int PerformCalculation(int x, int y);
%}  

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

imi_data_in(imimr::ImiBodySkeleton, imi.mr.agent.ImiBodySkeleton)

// %typemap(out) void* {
//     $result = $1; //(global::System.IntPtr)
// }
// %typemap(csout) void* {
//     return $imcall;
// }


%include "part_api.h"