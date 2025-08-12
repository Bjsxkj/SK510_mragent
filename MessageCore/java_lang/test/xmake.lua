
target('messagecore_java')
    set_kind('binary')
    add_rules('java')
    -- add_values("java_srcdir", 'java', 'swig/java')
    add_files('../swig/java/**/*.java')

    add_values('ref_jar', '$(buildir)/$(plat)/$(arch)/$(mode)/imimrapi_java.jar')
    -- add_deps('api_java')
    -- add_files('$(buildir)/$(plat)/$(arch)/$(mode)/api_java.jar')

    on_run(function (target) 
        os.cd(target:targetdir())
        os.execv('java', {'-jar', './mrcore_java_test.jar'})
    end
    )
target_end()

target('messagecore_java_test')
    set_kind('binary')
    add_rules('java')
    -- add_values("java_srcdir", 'java', 'swig/java')
    add_files('java/**/*.java', '../swig/java/**/*.java')
    add_files('manifest.mf')

    add_values('ref_jar', '$(buildir)/$(plat)/$(arch)/$(mode)/imimrapi_java.jar')
    -- add_deps('api_java')
    -- add_files('$(buildir)/$(plat)/$(arch)/$(mode)/api_java.jar')

    on_run(function (target) 
        os.cd(target:targetdir())
        os.execv('java', {'-jar', './mrcore_java_test.jar'})
    end
    )
target_end()

if false then --is_plat('android') then 

    target('api_android_test')
        set_kind('phony')
        add_rules('android_dex')
        add_files('$(buildir)/$(plat)/$(arch)/$(mode)/api_javatest.jar')

        after_build(function (target)
            local out = target:targetfile()..'/../'
            local tt = '/data/local/tmp/'
            os.cd(out)
            os.execv('adb', {'push','api_android_test.jar', tt})
        
        end)

        on_run(function (target)
        --..target:name()..'.jar
            os.execv('adb', {'shell', 'cd /data/local/tmp && LD_LIBRARY_PATH=. app_process -Djava.class.path=./imimrapi_android_test.jar' .. ' ./ test.Test1'})
        end)

end
    