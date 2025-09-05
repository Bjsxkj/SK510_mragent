-- add_requires("python 3.x") --, "numpy")

set_languages('c++11')
target("messagecore_jni")
    set_kind('shared')
    if true then --  not is_plat('android')  then

        local ref_mragent_jar = path.absolute('android/app/libs/imimrapi_java.jar')
        add_rules("swig1.cpp", {moduletype = "java", buildjar = true})
        add_files("api_java.i", {swigflags = {'-doxygen', '-package', 'com.sximi.mrcore', '-DUSE_ANDROID_API=1'},
            jarflags = { '-encoding', 'UTF-8', '-classpath', ref_mragent_jar } } )
    -- add_includedirs('D:\\Program\\openjdk-11'..'/include', 'D:\\Program\\openjdk-11'..'/include/win32')
        -- add_defines('USE_ANDROID_API')
        --add_cxflags('-fsanitize=address', '-fno-omit-frame-pointer','-fsanitize=leak','-g', '-O0')

        -- add_files("swig/cpp/*.cpp")
    else
        add_files("swig0/cpp/*.cpp")
    end

    after_build(function (t) 
        import('core.project.project')
        local outdir = '$(scriptdir)/out/$(plat)/$(arch)/'
        -- os.cp('$(buildir)/$(plat)/$(arch)/$(mode)/libmessagecore_jni.so', outdir)
        if is_plat('windows') then 
            os.cp(t:targetdir()..'/'..t:name()..'.lib', outdir)
        elseif is_plat('android') then 
            os.cp(t:autogendir()..'/rules/swig/'..t:name()..'.jar',  '$(scriptdir)/android/app/libs/')
            os.cp(t:autogendir()..'/rules/swig/'..t:name()..'.jar',  '$(scriptdir)/../android/app/libs/')
        end
    end)

    on_clean(function (target) 
        local dir = target:autogendir()
        os.rm(dir..'/rules/swig/*')
        os.rm('$(scriptdir)/swig/java/*')
    end)

    -- add_files("../cpp/src/*.cpp")
    add_includedirs("../../common")
    add_defines('MESSAGECORE')
    add_includedirs("../cpp/include", "./")

    --add_deps('MessageCoreApi', {public=true})

    if is_plat('android') then
        add_defines('ANDROID', 'USE_ANDROID_API')
        -- add_includedirs('../../include', '../../src/iminect/include', '../../src/imiCommon/include' )
        -- add_linkdirs('../../out/$(plat)/$(arch)')
        -- add_links('iminect')
        on_load(function(target)
            --target:add('includedirs', '$(env JAVA_HOME)')
            target:add('deps', 'MessageCoreApi', {public=true})
        end)
    else
        add_includedirs('$(env JAVA_HOME)/include', '$(env JAVA_HOME)/include/win32')
        on_load(function(target)
            --target:add('includedirs', '$(env JAVA_HOME)')
            target:add('deps', 'MessageCoreApi', {public=true})
            target:set('runtimes', 'MD') --解决windows MT库问题
        end)

        -- on_clean(function (target) 
        --     local dir = target:autogendir()
            
        --     os.rm(dir..'/rules/swig/*')
        --     os.rm('$(scriptdir)/swig/java/*')
        -- end)

        add_deps('MessageCoreApi', {public=true})
    end

if has_config('build_tests') then
    includes('test')
end
