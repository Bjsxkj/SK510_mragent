--增加打包工具脚本
add_plugindirs('common/scripts/xmaketools')

if is_plat("android") then
    set_toolchains("@ndk", {sdkver = "26"})
end

option('use_qt_demo')
    set_showmenu(true)
    set_description('Use QT framework Demo')
    set_default(false)
    set_configvar('USE_QT_UI', 1)
    -- set_configvar('USE_QT_UI', 1)
    -- set_configvar('USE_QT_UI', 1)
option_end()

option('build_java_api')
    set_showmenu(true)
    set_description('Build Java API')
    set_default(true)
option_end()

option('build_tests')
    set_showmenu(true)
    set_description('Build Test')
    set_default(false)
option_end()


--更新SWIG生成的代码.
option('update_mragent_wrapped_code')
    set_showmenu(true)
    set_description('Update wrapped Code for C# or Java')
    set_default(false)
option_end()

option('update_mrcore_wrapped_code')
    set_showmenu(true)
    set_description('Update wrapped Code for Java')
    set_default(false)
option_end()

option('use_mock_test')
    set_showmenu(true)
    set_description('Mock Test')
    set_default(false)
option_end()

includes('common/3rdparty')

-- Set runtime library based on build mode
if is_plat('windows') then
    if is_mode("debug") then
        set_runtimes("MDd")  -- Multi-threaded Debug DLL
    else
        set_runtimes("MD")   -- Multi-threaded DLL
    end
end

-- Add rules for debug and release modes
add_rules("mode.debug", "mode.release")

-- includes('swig_rule_plus.lua')

includes('MessageCore')

--更新mrcore
target('update_mrcore')
    set_kind('phony')
    on_build(function(target)
        os.cp('MessageCore/java_lang/swig/java' , 'prebuilt/android/message_core/')
        os.cp('$(buildir)/$(plat)/$(arch)/$(mode)/libmessagecore_jni.so', 'prebuilt/android/message_core/libs/$(arch)/')
    end)


-- if is_plat('windows') then

--     includes('examples/MRCore-as-server')
--     includes('examples/MRAgent-as-client')
--     includes('examples/camera-demo')
--     includes('examples/test-upgrade')

-- end

-- 
includes('MRAgent')

