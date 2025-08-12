-- add_requires("python 3.x") --, "numpy")

includes('./rule_cs.lua')

rule('my_cs_build')
    on_clean(function (target) 
            local dir = target:autogendir()
            
            os.rm(dir..'/rules/swig/*')
            os.rm('$(scriptdir)/swig/cs/*')
        end)
    
    after_build(function(target)
        local out = '$(scriptdir)/swig/'
        local cpp_out = out..'/'
        local java_out = out..'/'
        --os.rm(out..'/*')
        os.mkdir(cpp_out)
        os.mkdir(java_out)
        os.cp(target:autogendir()..'/rules/swig/*.cpp', cpp_out)
        os.cp(target:autogendir()..'/rules/swig/*.h', cpp_out)
        os.cp(target:autogendir()..'/rules/swig/*.cs', java_out)
    end)

rule_end()

local cs_ref = 'C:\\Program Files (x86)\\Reference Assemblies\\Microsoft\\Framework\\.NETFramework\\v4.6.1'

target("mragent_data_cs")
    set_kind('shared')
    set_group('mragent')
    add_rules("swig1.cpp", {moduletype = "csharp" , buildcsharp= is_plat('windows') and true or false })
    add_files("mragent_data_cs.i",  {swigflags = { '-namespace', 'imi.mr.agent', '-DUSE_ANDROID_API=1'} , cscflags={'/lib:'..cs_ref, '/reference:System.Core.dll;mscorlib.dll'} })  --
    add_includedirs("../../common")
    add_defines('MRAGENT')
    add_includedirs('../../include', '../cpp/include', {public = true})
    add_rules('my_cs_build')

target("mragent_cs")
    set_group('mragent')
    set_kind('shared')
    add_rules("swig1.cpp", {moduletype = "csharp" , buildcsharp= is_plat('windows') and true or false })
    add_includedirs("../../common")
    add_defines('MRAGENT')

    --local data_cs = os.files('MrAgent/csharp/swig/*.cs')
    local a = path.absolute('../../build/windows/x64/release/') -- path.absolute('')
    local r = 'mragent_data_cs_csharp.dll'
    add_files("api_cs.i",  {swigflags = {'-namespace', 'imi.mr.agent', '-DUSE_ANDROID_API=1'},  cscflags={'/lib:'..a,  '/reference:'..r} }) --, {swigflags = {'-DUSE_ANDROID_API=1'}}) --"-doxygen"



    add_deps('mragent_data_cs')
    add_rules('my_cs_build')

-- else 
--     --android不需要重新生产
--     set_kind('shared')
--     add_files('swig0/*.cpp')

-- end

    add_defines('USE_ANDROID_API')

    -- add_files("../cpp/src/*.cpp")

    add_includedirs("./", "../cpp/include")

    add_includedirs('../../include')
    add_deps('mragent_cpp', {public=true})

    -- after_build(function(target)
    --     -- todo
    --    -- os.cp(target:targetfile(), 'out/$(plat)/$(arch)/')
    -- end)


-- target('make_u3d_pkg')
--     set_kind('phony')
--     on_build(function(target)
--         local out = 'build/u3d/'
--         os.mkdir(out)
--         os.mkdir(out..'/Assets/Android/')
--         os.mkdir(out..'/script/')
--         -- os.cp('wrappers/java_lang/android/app/build/outputs/aar/app-debug.aar', out..'/Assets/Android/imimrapi_android.aar')
--         os.cp('$(buildir)/$(plat)/$(arch)/*.dll', out)
--         os.cp('$(scriptdir)/swig/*.cs', out..'/script/')
--         -- os.cp('wrappers/csharp/cs_4_u3d_android/*.cs', out..'/script/')

--     end)

if not is_plat('android') then

    target('mragent_csharp')
        set_kind('shared')
        add_rules('csharp')
        -- add_files('testcs_1.cs')
        add_files('swig/*.cs')

    includes('test')
end