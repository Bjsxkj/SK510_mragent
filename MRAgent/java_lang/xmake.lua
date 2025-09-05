-- add_requires("python 3.x") --, "numpy")

rule('my_jni_build')

if is_plat('android') then
    on_load(function(target)
        target:add('defines', 'ANDROID', 'USE_ANDROID_API')
    end)
else
    on_load(function(target)
        target:add('includedirs', '$(env JAVA_HOME)/include', '$(env JAVA_HOME)/include/win32')
        --target:add('includedirs', '$(env JAVA_HOME)')
        
        if is_mode("debug") then
            target:set('runtimes', 'MDd')  -- Multi-threaded Debug DLL
        else
            target:set('runtimes', 'MD')   -- Multi-threaded DLL
        end
    end)
end

    after_build(function(target)
        local out = '$(scriptdir)/swig/'
        local cpp_out = out..'/cpp/'
        local java_out = out..'/java/com/sximi/mragent/'
        --os.rm(out..'/*')
        os.mkdir(cpp_out)
        os.mkdir(java_out)
        os.cp(target:autogendir()..'/rules/swig/*.cpp', cpp_out)
        os.cp(target:autogendir()..'/rules/swig/*.h', cpp_out)
        os.cp(target:autogendir()..'/rules/swig/*.java', java_out)
        -- copy jar 到 android目录
        os.cp(target:autogendir()..'/rules/swig/'..target:name()..'.jar', out)
        if target:plat()=='android' then
            os.cp(target:autogendir()..'/rules/swig/'..target:name()..'.jar', '$(scriptdir)/android/app/libs')
            os.cp(target:autogendir()..'/rules/swig/'..target:name()..'.jar', '$(scriptdir)/../android/app/libs')
        end
    end)
rule_end()

-- 提供公用数据类型的定义
target("mragent_data_jni")
    set_kind('shared')
    set_group('mragent')
    add_rules("swig1.cpp", {moduletype = "java" , buildjar=true })
    add_files("mragent_data_java.i", {swigflags = { '-package', 'com.sximi.mragent', '-DUSE_ANDROID_API=1'} ,jarflags = { '-encoding', 'UTF-8'} }) --'-doxygen',
    add_includedirs("../../common")
    add_defines('MRAGENT')
    add_includedirs('../cpp/include', {public = true})
    if is_plat('windows') then
        add_includedirs('$(env JAVA_HOME)/include',  '$(env JAVA_HOME)/include/win32')
    end
    add_rules('my_jni_build')


target("mragent_jni")
    set_kind('shared')
     set_group('mragent')
    -- 
    local ref_jar = ("$(scriptdir)/MRAgent/java_lang/swig/mragent_data_jni.jar")
    add_rules("swig1.cpp", {moduletype = "java" , buildjar=true })
    add_files("api_java.i", {swigflags = { '-package', 'com.sximi.mragent', '-DUSE_ANDROID_API=1'} , jarflags = { '-encoding', 'UTF-8', '-classpath', ref_jar},
     }) --'-doxygen',

    add_deps('mragent_data_jni')
    add_rules('my_jni_build')
    
    add_includedirs("../../common")
    add_defines('MRAGENT')
    add_includedirs("../cpp/common")
    add_packages('libzmq-4.3', {public=true})

    add_deps('mragent_cpp', {public=true})

if (not is_plat('android')) then
    
else
    
    add_defines('NOTAGME') 
    add_defines('ANDROID', 'USE_ANDROID_API=1')
    

    -- add_files("swig0/cpp/*.cpp")
end
    -- add_files("../cpp/src/*.cpp")
    add_includedirs("../cpp/include", "./")


--add_cxflags('-fsanitize=address', '-fno-omit-frame-pointer','-fsanitize=leak','-g', '-O0')
   
    -- if is_plat('android') or is_plat('linux') then
    --     if is_mode("debug") then
    --         add_cxflags('-g', '-O0')
    --     end
    -- end


    



    

    

    -- on_load(function (target) 
    --    -- target:set('autogendir', 'wrap/python/py')
    -- end)

    -- add_linkdirs('../../lib')
    -- -- add_links('npymath', 'python3', 'python38')

-- if not is_plat('android') and has_config('update_mragent_wrapped_code') then
--     after_build(function (target)
--         --print('dir=', target:autogendir())
--         --os.cp("wrap/python/test/*.py", target:targetdir()..'/')
--         local out = '$(scriptdir)/swig/'
--         local cpp_out = out..'/cpp/'
--         local java_out = out..'/java/com/sximi/mragent/'
--         --os.rm(out..'/*')
--         os.mkdir(cpp_out)
--         os.mkdir(java_out)
--         os.cp(target:autogendir()..'/rules/swig/*.cpp', cpp_out)
--         os.cp(target:autogendir()..'/rules/swig/*.h', cpp_out)
--         os.cp(target:autogendir()..'/rules/swig/*.java', java_out)

--        -- os.cp(target:targetfile(), "out/$(plat)/$(arch)/")
        
--     end)

--     on_clean(function (target) 
--         local dir = target:autogendir()

--         os.rm(dir..'/rules/swig/*')
--         os.rm('$(scriptdir)/swig/java/*')
--     end)
-- end
    -- on_run(function (target) 
    --     os.cd(target:targetdir())
    --     os.execv('python3', {'./test_1.py'})
    -- end)

if not is_plat('android') then
    includes('./java_rule.lua')
    includes('test_data', 'test')
end



