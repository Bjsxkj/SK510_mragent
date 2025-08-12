
for _, one in ipairs(os.files('./test_*.cpp')) do 
    target('test_agent_'..path.basename(one))
        set_kind('binary')
        set_group('test')
        add_files(one)
        add_deps('mragent_cpp', {public=true})
        --add_packages('libprotobuf-lite-3.6')

        add_includedirs('../include')
        
        if is_plat('android') then 
            add_defines('ANDROID')

            after_build(function(target)
                os.execv('adb', {'push', target:targetfile(), '/data/local/tmp/'})
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