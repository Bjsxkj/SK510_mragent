
%define vector2java_array(cname, jniname, java_name, jnigetter, jnisetter, jnialocer)
%ignore std::vector<cname>;
%typemap(jni) (std::vector<cname>)  "jniname"
%typemap(jtype) (std::vector<cname>) "java_name"
%typemap(jstype) (std::vector<cname>) "java_name"
%typemap(javain) (std::vector<cname>) "$javainput"
%typemap(javaout) (std::vector<cname>) {
    return $jnicall;
}
%typemap(in) (std::vector<cname>) {
    cname* str = (cname *) JCALL2(jnigetter, jenv, $input, 0);
    cname* end = str + JCALL1(GetArrayLength, jenv, $input);
    $1 = std::vector<cname>(str, end);
}
%typemap(out) (std::vector<cname>) {
    int _len = $1.size();
    jniname res = JCALL1(jnialocer, jenv, _len);
    JCALL4(jnisetter, jenv, res, 0, _len, ( j ## cname *) $1.data());
    $result = res;
}
%enddef

%define vector2java_objarray(cname, java_name, java_clazz)
%ignore std::vector<cname>;
%typemap(jni) (std::vector<cname>)  "jobjectArray"
%typemap(jtype) (std::vector<cname>) "java_name[]"
%typemap(jstype) (std::vector<cname>) "java_name[]"
%typemap(javain) (std::vector<cname>) "$javainput"
%typemap(javaout) (std::vector<cname>) {
    return $jnicall;
}
%typemap(in) (std::vector<cname>) {
    size_t n = JCALL1(GetArrayLength, jenv, $input);
    std::vector<cname> a;
    a.resize(n);
    jclass clazz = JCALL1(FindClass, jenv, java_clazz);
    // if(clazz==0){
    //     return 0;
    // }
    // 获取名字为name的静态成员方法的ID，其中sig为类型签名
    jmethodID meth = JCALL3(GetStaticMethodID, jenv, clazz, "getCPtr", "(L"  java_clazz  ";)J");
    for(int i=0; i<n; i++){
        jobject oo = JCALL2(GetObjectArrayElement, jenv, $input, i);
        cname* ptr = (cname*) JCALL3(CallStaticLongMethod, jenv, clazz, meth, oo);
        a[i] = *(ptr);
    }
    $1 = a;
}
%typemap(out) (std::vector<cname>) {
    int _len = $1.size();
    jclass clazz = JCALL1(FindClass, jenv, java_clazz);
    if(clazz==0){
        return 0;
    }
    jmethodID meth = JCALL3(GetMethodID,jenv, clazz, "<init>", "(JZ)V");
    if(meth==0){
        return 0;
    }
    jobjectArray res = JCALL3(NewObjectArray, jenv, _len, clazz, NULL);
    for(int i=0; i<_len; i++){
        cname* ptr = new cname($1.operator[](i)); //new cname();
        jobject oo = JCALL4(NewObject, jenv, clazz, meth, (jlong)ptr , (jboolean)true); //$1.operator[](i)
        //printf("ptr=%x, oo=%x\n", ptr, oo);
        JCALL3(SetObjectArrayElement, jenv, res, i, oo);
    }
    $result = res;
}
%enddef

vector2java_array(int,jintArray, int[], GetIntArrayElements, SetIntArrayRegion, NewIntArray)
vector2java_array(long,jlongArray, long[], GetLongArrayElements, SetLongArrayRegion, NewLongArray)
vector2java_array(float,jfloatArray, float[], GetFloatArrayElements, SetFloatArrayRegion, NewFloatArray)
vector2java_array(double,jdoubleArray, double[], GetDoubleArrayElements, SetDoubleArrayRegion, NewDoubleArray)
vector2java_array(bool,jbooleanArray, boolean[], GetBooleanArrayElements, SetBooleanArrayRegion, NewBooleanArray)


%define member_fix_array(cname, java_name ,java_clazz)
%typemap(jni)  (cname [ANY])  "jobjectArray"
%typemap(jtype)  (cname [ANY]) "java_name[]"
%typemap(jstype) (cname [ANY]) "java_name[]"
%typemap(javain)  (cname [ANY]) "$javainput"
%typemap(javaout)  (cname [ANY]) {
    return $jnicall;
}
%typemap(memberin) (cname [ANY]) {
    // typemap(memberin)
    int n = $1_dim0;
    size_t n1 = JCALL1(GetArrayLength, jenv, jarg2);
    jclass clazz = JCALL1(FindClass, jenv, java_clazz);
    // 获取名字为name的静态成员方法的ID，其中sig为类型签名
    jmethodID meth = JCALL3(GetStaticMethodID, jenv, clazz, "getCPtr", "(L"  java_clazz  ";)J");
    for(int i=0; i<n && i<n1; i++){
        jobject oo = JCALL2(GetObjectArrayElement, jenv, jarg2, i);
        cname* ptr = (cname*) JCALL3(CallStaticLongMethod, jenv, clazz, meth, oo);
        $1[i] = *(ptr);
    }
}
%typemap(in) (cname [ANY]) {
    // in;
}
%typemap(out) (cname [ANY]) {
    int n = $1_dim0;
   jclass clazz = JCALL1(FindClass, jenv, java_clazz);
    if(clazz==0){
        return 0;
    }
    jmethodID meth = JCALL3(GetMethodID,jenv, clazz, "<init>", "(JZ)V");
    if(meth==0){
        return 0;
    }
    jobjectArray res = JCALL3(NewObjectArray, jenv, n, clazz, NULL);
    for(int i=0; i<n; i++){
        cname* ptr = new cname($1[i]); 
        jobject oo = JCALL4(NewObject, jenv, clazz, meth, (jlong)ptr , (jboolean)true); 
        //printf("ptr=%x, oo=%x\n", ptr, oo);
        JCALL3(SetObjectArrayElement, jenv, res, i, oo);
    }
    $result = res;
}
%enddef


%define member_fix_base_array(cname, jniname, java_name, jnigetter, jnisetter, jnialocer)
%typemap(jni)  (cname [ANY])  "jniname"
%typemap(jtype)  (cname [ANY]) "java_name[]"
%typemap(jstype) (cname [ANY]) "java_name[]"
%typemap(javain)  (cname [ANY]) "$javainput"
%typemap(javaout)  (cname [ANY]) {
    return $jnicall;
}
%typemap(memberin) (cname [ANY]) {
    // typemap(memberin)
    int len = JCALL1(GetArrayLength, jenv, jarg2);
    int n = ($1_dim0<len) ? $1_dim0 : len;
    cname* src = (cname *) JCALL2(jnigetter, jenv, jarg2, 0);
    cname* dst = $1;
    for(int i=0;i<n;i++, dst++, src++){
        *dst = *src;
    }
}
%typemap(in) (cname [ANY]) {
    // in;
}
%typemap(out) (cname [ANY]) {
    int n = $1_dim0;
    jniname res = JCALL1(jnialocer, jenv, n);
    JCALL4(jnisetter, jenv, res, 0, n, ( j ## java_name *) $1);
    $result = res;
}
%enddef


//member_base_fix_array(int, GetIntArrayElement, PyInt_FromLong)
