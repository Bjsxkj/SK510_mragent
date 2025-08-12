
target("test_part_jni")
    set_kind('shared')
    set_group('test')
    add_rules("swig.cpp", {moduletype = "java"})
    add_files("part_api_java.i", {swigflags = {'-doxygen', '-package', 'com.sximi.test', '-DUSE_ANDROID_API=1'} })
   add_includedirs('D:\\Program\\openjdk-11'..'/include', 'D:\\Program\\openjdk-11'..'/include/win32')

    add_files("./*.cpp")
    add_includedirs("../../cpp/include", "./")
