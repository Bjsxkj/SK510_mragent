// 直接映射std::vector
%define smp_base_vector(cname, csname, listname)
%template(listname) std::vector<cname>;
// %ignore std::vector<cname>;
%typemap(ctype , out="void* ") (std::vector<cname>)  "int $1_len, cname *"
%typemap(imtype, out="global::System.IntPtr") (std::vector<cname>) "int $1_len, [global::System.Runtime.InteropServices.In, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)] csname[]"
%typemap(cstype) (std::vector<cname>) "csname[]"
%typemap(csin) (std::vector<cname>) " $csinput.Length, $csinput"
%typemap(csout) (std::vector<cname>) {
    listname r = new listname($imcall, true);
    csname[] ret  = r.ToArray();
    r.Dispose();
    return ret;
}
%typemap(in) std::vector<cname> {
    // typemap in
    cname* ptr = (cname*) $input;
    int n = $1_len;
    $1 = std::vector<cname>(ptr, ptr+n);
}
%typemap(out) std::vector<cname> {
    // typemap out
    $result = ( new std::vector<cname>($1));
}

%enddef


//return new global::System.Collections.Generic.List<int>(%jsize).ToArray();
    
// %typemap(argout) std::vector<cname> {  
//     $imtype &$1_tmp;  
//     $1 = &$1_tmp[0];  
//     $result = SWIGTYPE_p_int.getCPtr($1_tmp.ToArray());  
// }  
//%typemap(csin, argout) std::vector<cname> SIZE_ARG "ref int $1_size"


// smp_base_vector(int, int, IntList)
// smp_base_vector(uint8_t, byte, ByteList)
// smp_base_vector(float, float, FloatList)
// smp_base_vector(double, double, DoubleList)
// smp_base_vector(long, long, LongList)



// 直接映射std::vector
%define smp_fix_base_array(cname, csname, listname)
// %ignore std::vector<cname>;
%typemap(ctype , out="void* ") (cname [ANY])  "int $1_len, cname *"
%typemap(imtype, out="global::System.IntPtr") (cname [ANY]) "int $1_len, [global::System.Runtime.InteropServices.In, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)] csname[]"
%typemap(cstype) (cname [ANY]) "csname[]"
%typemap(csin) (cname [ANY]) " $csinput.Length, $csinput"
%typemap(csout) (scname [ANY]) {
    listname r = new listname($imcall, true);
    csname[] ret  = r.ToArray();
    r.Dispose();
    return ret;
}
%typemap(in) (cname [ANY]) {
    // typemap in
    cname* ptr = (cname*) $input;
    //int n = $1_len;
    $1; // = std::vector<cname>(ptr, ptr+n);
}

%typemap(memberin) (cname [ANY]) {
    // typemap in
    cname* ptr = (cname*) $input;
    //int n = $1_len;
    $1; // = std::vector<cname>(ptr, ptr+n);
}
%typemap(out) (cname [ANY]) {
    // typemap out
    $1;
    $result; // = ( new std::vector<cname>($1));
}

%enddef

smp_fix_base_array(imimr::ImiMoveTrackData::ButtonSate, int, xxx)

/// 转化结构体中的数组为List
%define struct_array_to_list(clazzname, naked_clazzname, elename, elecount, eletype, listname)
%template("listname") std::vector<eletype>;
%extend clazzname {
    %ignore elename;

    std::vector<eletype>* get_ ## elename ## _ () {
        eletype* ptr = self-> elename;
        return new std::vector<eletype>(ptr, ptr + elecount);
    }

    void set_ ## elename ## 0 (const std::vector<eletype>& a) {
        int len = a.size()< elecount ? a.size(): elecount;
        for(int i=0;i<len;i++){
            self->elename[i] = a[i];
        }
    }
%proxycode %{
    public listname get_ ## elename ## 0() {
      global::System.IntPtr cPtr = $module ## PINVOKE.##naked_clazzname##_get_## elename ## _(swigCPtr);
      listname ret = (cPtr == global::System.IntPtr.Zero) ? null : new listname(cPtr, true);
      if ($module ## PINVOKE.SWIGPendingException.Pending) throw $module ## PINVOKE.SWIGPendingException.Retrieve();
      return ret;
    }

    public listname elename {
        set {
            set_ ## elename ## 0 (value);
        }
        get {
            return get_ ## elename ## 0 ();
        }
    }
%}
}
%enddef


/// 基本类型固定数组转化
%define smp_base_array(cname, csname)
// %ignore "cname *";
%typemap(ctype , out="void* ") (cname [ANY])  "cname *"
%typemap(imtype, inattributes="[global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray, SizeConst=$1_dim0)]",
out="global::System.IntPtr"
) (cname [ANY]) "csname[]"
%typemap(cstype) (cname [ANY]) "csname[]"
%typemap(csin) (cname [ANY]) "$csinput"
%typemap(csvarout, pre="get") (cname [ANY]) %{
get {
    csname[] ret = new csname[$1_dim0];
    System.Runtime.InteropServices.Marshal.Copy($imcall, ret, 0, $1_dim0);
    return ret;
}
%}
// %typemap(csout, pre="get") (cname [ANY]) {
//     return $imcall;
// }
%typemap(in) (cname [ANY]) {
    // typemap in
    $1 = $input;
}
%typemap(memberin) (cname [ANY]) {
    // typemap in
    for(int i=0; i < $1_dim0; i++){
        $1[i] = $input[i];
    }
}
%typemap(out) (cname [ANY]) {
    // typemap out
    $result = $1;
}
%enddef