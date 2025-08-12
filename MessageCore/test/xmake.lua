
for _, one in ipairs(os.files("test_*.cpp")) do
    set_languages('c++11')
    target('test_core_'..path.basename(one))
        set_kind('binary')
        set_group('test')
        add_files(one)
        add_deps('MessageCoreApi', {public = true})

        if is_plat('android') then 
            after_build(function(target)
                os.execv('adb', {'push', target:targetfile(), '/data/local/tmp/'})
                --os.execv('adb', {'push', 'D:\\workspace\\sk510_mragent\\MessageCore\\java_lang\\android\\app\\libs\\arm64-v8a\\libmessagecore_jni.so', '/data/local/tmp/'})
                if target:kind()=='binary' then 
                    os.execv('adb', {'shell', 'chmod 777 /data/local/tmp/'..target:name()})
                end
            end)
            --
            on_run(function(target)
                os.execv('adb', {'shell', 'cd /data/local/tmp/ && LD_LIBRARY_PATH=. ./'..target:name()})
            end)
        end
end