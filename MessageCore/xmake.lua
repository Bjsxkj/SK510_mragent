add_requires('libzmq-4.3')

includes('../swig_rule_plus.lua')

target('MessageCoreApi')
    set_kind('static')
    add_includedirs("../common")
    add_files('../common/*.cpp')
    add_defines('MESSAGECORE')
    add_files('cpp/src/*.cpp', 'cpp/common/*.cpp')
    add_includedirs('cpp/include', 'cpp/src', 'cpp/common', {public = true})
    add_cxflags('-fPIC')
    if is_plat('android') then
        add_syslinks("log", "android")
        add_defines('ANDROID')
        if is_mode("debug") then 
            add_cxflags('-g', '-O0')
        end
    end

    if has_config('use_mock_test') then
        add_defines('MOCK_TEST')
    end

    add_packages('libzmq-4.3', {public=true})

    after_build(function(target)
    end)

if has_config('build_java_api') then
    includes('java_lang')
end

if true then -- has_config('build_tests') then
    includes('test')
end
