--[[
用javac编译成jar
```
target('imimrapi_android')
    -- set_kind('phony')
    add_rules('java')
    -- add_values("java_srcdir", 'java', 'swig/java')
    add_files('../java_4_android/java/**/*.java')
    --引用但不包含的jar
    add_values('ref_jar', '$(buildir)/$(plat)/$(arch)/$(mode)/imimrapi_java.jar')
```
--]]

rule('java')
set_extensions(".java",'.mf','.jar')
add_imports("core.project.project")
on_load(function (target)
    target:set("extension", ".jar")
    local outdata, errdata = os.iorun("where javac")
    if outdata==nil or outdata=='' then
        raise('Please install JDK or put javac\'s dir into system `PATH`')
    end
    import("core.base.global")
end)

on_clean(function(target)
    local clazzdir = vformat('$(buildir)/class/%s',target:name())
    os.tryrm(clazzdir..'/*')
end)

before_build(function(target)
    -- local clazzdir = vformat('$(buildir)/class/%s',target:name())
    -- os.tryrm(clazzdir..'/*')

    for _, one in ipairs(target:get('deps')) do
        print('deps=', one)
        local tt = project.target(one)
        local pp = tt:targetfile()
        if pp:endswith('.jar') then
            target:add('files', pp)
        end
    end

end)


function jar_build(target, fileconfig, opt)
    cprint("${red}build jar")
    cprint("${blue}build ")
    print(fileconfig)
    print(opt)
    cprint("${blue}build jar")

    javac = find_tool("javac")
    local exeend = (os.is_host('windows') and '.exe' or '')
    local javac_path = '$(env JAVA_HOME)/bin/javac'..exeend
    if javac == nil and os.exists(javac_path) then
        javac =  { program = javac_path }
    end

    jar = find_tool("jar")
    local jar_path = '$(env JAVA_HOME)/bin/jar'..exeend
    if jar == nil and os.exists(jar_path) then
        jar = { program = jar_path }
    end

    assert(javac, "javac not found! please Install JDK or set $JAVA_HOME")
    assert(jar, "jar not found! please Install JDK or set $JAVA_HOME")


    local java_src_dir = path.directory(target:targetfile())..'/'..target:name()  --fileconfig.java_files
    -- path.join(target:autogendir(), "rules", "swig")
    local java_class_dir = java_src_dir..'/class'

    local user_outdir = find_user_outdir(fileconfig)
    if user_outdir then
        java_src_dir = user_outdir
    end

    os.mkdir(java_class_dir)

    -- get java files
    local autogenfiles = fileconfig.java_files -- os.files(path.join(java_src_dir, "*.java"))

    if fileconfig and fileconfig.java_src then
        table.join2(autogenfiles, fileconfig.java_src)
    end 

    -- write file list
    local filelistname = path.join(java_src_dir, "buildlist.txt")
    local file = io.open(filelistname, "w")
    if file then
        for _, sourcebatch in ipairs(autogenfiles) do
            file:print(sourcebatch)
        end
        file:close()
        
    end

    print('build Java files:', filelistname)

    local javac_args = {}

    -- print(opt)
    if fileconfig.jarflags then
        map_list(fileconfig.jarflags, vformat)
        table.join2(javac_args, fileconfig.jarflags)
    end


    table.join2(javac_args, { "-d", java_class_dir, "@" .. filelistname})

    -- cprint("1111111111111111111111111111111111111111111111")
    -- compile to class file
    progress.show(opt.progress, "${color.build.object}compiling.javac %s class file", target:name())
    -- print("5555555555555555555555555555555555555555555555555555555", javac, javac_args)
    os.vrunv(javac.program, javac_args) --"--release", "17",  -"-Dfile.encoding=UTF-8",

    -- cprint("222222222222222222222222222222222222222222222222222222222")
    -- generate jar file
    progress.show(opt.progress, "${color.build.object}compiling.jar %s", target:name() .. ".jar")
    --os.vrunv(jar.program, {"-cvf", path.join(java_src_dir, target:name() .. ".jar"),  java_class_dir})
    os.vrunv(jar.program, {"-cvf", path.join(java_src_dir, target:name() .. ".jar"), '-C', java_class_dir, '.'})

    -- cprint("333333333333333333333333333333333333333333333333333333333333333333333")
    os.tryrm(filelistname)

    print("Build jar OK")
    
end

on_build(function(target)
    import("core.base.global")

    local srcdir = target:values('java_srcdir')
    print('java_srcdir = ', srcdir)
    print('java files = ', target:get('files'))

    
    local src_dirs = ''
    local mod_paths = ''
    local k = 1
    for _, one in ipairs(target:get('files')) do
        if one:endswith('.java') then
            local x,_ = string.find(one,'(%*%*)')
            local ss = (src_dirs=='' and '' or ';')..string.sub(one,1,x-1)
            --print(x,ss)
            src_dirs = src_dirs..ss
        end
        if one:endswith('.jar') then
            mod_paths = mod_paths..(k > 1 and ';' or '')..one
            k = k + 1
        end
    end

    cprintf('${blue}build:%s', src_dirs)

    local clazzdir = vformat('$(buildir)/class/%s',target:name()..'/')
    os.mkdir(clazzdir)
    os.rm(clazzdir..'/*')
    local deps = target:get('deps')
    if deps then
        for _, one in ipairs(deps) do
            os.trycp('$(buildir)/class/'.. one..'/*', clazzdir..'/' )
        end
    end
    --cprint('${red}deps=%s: %s', deps, type(deps))

    local sourcebatch = target:sourcebatches().java
--    print(target:sourcebatches())

   print('java sourcefiles = ', sourcebatch.sourcefiles)
-- end)
-- on_build_files(function (target, sourcebatch, opt)
    --print('java  compile: ', sourcebatch.sourcefiles)
    -- os.cd('build/java')

    local jarsstr='.'
    local jars={}
    local k = 1
    for _, filepath in ipairs(sourcebatch.sourcefiles) do
        if filepath:endswith('.jar') then
            table.join2(jars, {filepath})
            jarsstr = jarsstr .. (k > 1 and ';' or '') .. filepath
            k = k + 1

            --os.execv('jar',{'xMf', '-C',clazzdir, filepath})

        end
    end

    if is_plat('android') then
        local sdkdir = global.get('android_sdk')
        -- jarsstr = jarsstr .. ';' .. path.translate(sdkdir..'/platforms/android-30/android.jar')
        mod_paths = mod_paths .. ';' .. path.translate(sdkdir..'/platforms/android-30/android.jar')
    end

    local x = target:values("ref_jar")
    if type(x)=='string' then
        mod_paths = mod_paths .. ';' .. path.translate(x)
    elseif type(x)=='table' then
        for _, one in ipairs(x)  do
            mod_paths = mod_paths .. ';' .. path.translate(one)
        end
    end

    print('All jars:', jars, jarsstr)
    
    local manifest = nil
    for _, filepath in ipairs(sourcebatch.sourcefiles) do
            if filepath:endswith('.java') then
               
                local curdir = path.directory(filepath)
                --print(curdir, path.directory(curdir))

                cprint('${red}'..filepath..' ==> '..clazzdir)
                --os.cd(clazzdir)
               local argv = {'-encoding','UTF-8',
                '-d', clazzdir,
                -- '-s', curdir,
                    
                '--source-path', src_dirs, --\\com\\imi\\authorization', -- curdir,  --'build/java',  
                --    '-cp', clazzdir,   -- vformat('$(scriptdir)/'),
                    -- '-sourcepath', 'src/swig/java', --curdir,  -- 'build/java', 

                    '-classpath', jarsstr, 
                }

                if mod_paths~='' then
                table.join2(argv, {'-cp', mod_paths})
                end

                table.join2(argv, {filepath})

                print('javac', unpack(argv))

                os.runv('javac',argv)
            elseif filepath:endswith('.mf') then
                manifest = io.readfile(filepath)
            end

    end

    -- os.cd('build/class/'.. target:name())
    
    local jarfile = target:targetfile() --vformat('%s%s.jar', out, mname)
    --合并成fat-jar
    if #jars > 0 then
        cprint('${yellow}merge jar:')
        -- local mergedir = os.tmpdir()..'/'..path.basename(jarfile)..'/'
        -- os.mkdir(mergedir)
        -- os.cp(clazzdir..'/*', mergedir)
        local mergedir = clazzdir
        local curdir = os.curdir()

        for _,jar in ipairs(jars) do
            -- local tmpdir = os.tmpdir()..'/'..path.basename(jar)
            local jar_src = curdir..'/'..jar
            -- os.mkdir(tmpdir)
            tmpdir = mergedir
            os.cd(tmpdir)
            cprint('${yellow}tmpdir:'..tmpdir..' <= '..jar_src)
            os.execv('jar',{'xf', jar_src})  -- curdir..'/'..jar})
            os.cd(curdir)
            -- os.cp(tmpdir..'/*', mergedir)

            -- for _,jj in ipairs(os.files(tmpdir..'/**.class')) do
            --     print('add:',jj)
            --     local rdir = path.relative(jj, tmpdir)
            --     os.execv('jar',{'uf',jarfile, '-C', tmpdir, rdir})
            -- end
            
        end
        cprint('${yellow}merge: '..mergedir)
        --os.execv('jar',{'cvfM',  jarfile,'-C', mergedir, '.' })
    end

    -- os.cd(curdir)

    cprint('${blue}make jar : '..jarfile..' + '..(manifest~=nil and manifest or 'nil'))
    -- if target:get('kind') == 'binary' then 
    if manifest ~= nil then 
            --raise('maiclass is no defined')
       -- end
       
        local tmp = os.tmpdir() .. '/manifest.mf'
        io.writefile(tmp, manifest)  --'Main-Class: '..mainclazz..'\n')
        cprintf('${green}manifest: %s\n', manifest)
        print(tmp)
        os.execv('jar',{'cvfm',  jarfile , tmp, '-C', clazzdir, '.' })
        
    else
        os.execv('jar',{'cvfM',  jarfile,'-C', clazzdir, '.' })
    end

    

    if is_plat('android') then
        local sdkdir = global.get('android_sdk')
        --dx --dex --output=output.jar source.jar
        if target:kind()=='binary' then
            local x = global.get('build_toolver')
            if x==nil then
                raise('Please set global config build_toolver, e.g.: xmake global --build_toolver=30.0.2')
            end
            os.execv(sdkdir..'/build-tools/'..x..'/dx.bat', {'--dex', '--output='..jarfile..'.dex', jarfile } , {envs = {PATH = vformat("$(env PATH)")}})
        end
    end

end)

rule_end()
