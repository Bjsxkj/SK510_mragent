%define map_java_bytebuffer(pchInput,inputSize)

%typemap(in)        (char* pchInput, int inputSize) {
  $1 = (char*) JCALL1(GetDirectBufferAddress, jenv, $input); 
  $2 = (int)JCALL1(GetDirectBufferCapacity, jenv, $input); 
} 
/* These 3 typemaps tell SWIG what JNI and Java types to use */ 
%typemap(jni)       (char* pchInput, int inputSize), (char* pchOutput, int* outputSize) "jobject" 
%typemap(jtype)     (char* pchInput, int inputSize), (char* pchOutput, int* outputSize) "java.nio.ByteBuffer" 
%typemap(jstype)    (char* pchInput, int inputSize), (char* pchOutput, int* outputSize) "java.nio.ByteBuffer" 
%typemap(javain, pre="assert ($javainput && $javainput.isDirect()")    (char* pchInput, int inputSize), (char* pchOutput, int* outputSize) "$javainput" 
%typemap(javaout)   (char* pchInput, int inputSize), (char* pchOutput, int* outputSize) { 
    return $jnicall; 
}

%enddef

%define map_buffer_ref(arr, len)
%typemap(jni) (unsigned char* arr, int len)  "jboolean _$1, jobject"
%typemap(jtype) (unsigned char* arr, int len) "boolean _$1, java.lang.Object"
%typemap(jstype) (unsigned char* arr, int len) "java.nio.ByteBuffer"
%typemap(javain, pre="  assert ($javainput.isDirect() || $javainput.array()!=null) : \"Buffer must not be null.\";") (unsigned char* arr, int len)
"$javainput.isDirect(), $javainput.isDirect() ? $javainput : $javainput.array()"
%typemap(in) (unsigned char* arr, int len) {
    if(_$1){
        void *data =  JCALL1(GetDirectBufferAddress, jenv, $input);
        //LOGIS("ptr=",data)
        jobject obj = $input;
        size_t _len = JCALL1(GetDirectBufferCapacity, jenv, $input);
        $1 = (unsigned char *)data;
        $2 = _len;
    }else{
        jobject obj = $input;
        jbyteArray aa = (jbyteArray) $input;
        void *data = JCALL2(GetByteArrayElements, jenv, aa, 0);
        size_t _len = JCALL1(GetArrayLength, jenv, aa);
        $1 = (unsigned char *)data;
        $2 = _len;
    }
}
%enddef