--
-- 修改至xmake内置的swig rule， 增强其功能
-- @author      Ling Zheng
--

-- imports
import("lib.detect.find_tool")
import("utils.progress")
import("core.project.depend")
import("core.tool.compiler")

function find_user_outdir(fileconfig)
    -- user specified output path
    if fileconfig and fileconfig.swigflags then
        -- find -outdir path
        for i, par in ipairs(fileconfig.swigflags) do
            if par == "-outdir" then
                local dirpath = fileconfig.swigflags[i + 1]
                if os.isdir(dirpath) then
                    return dirpath
                end
            end
        end
    end
end


function map_list(ls, proc)
    -- local n = len(ls)
    for i = 1, #ls do
        ls[i] = proc(ls[i])
    end
end

function jar_build(target, fileconfig, opt)
    cprint("${red}build jar")
    cprint("${blue}build ")
    print(fileconfig)
    print(opt)
    cprint("${blue}build jar")

    -- local javac = assert(find_tool("javac"), "javac not found!")
    -- local jar = assert(find_tool("jar"), "jar not found!")

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

    -- print('javac', javac)
    
    -- local javac = "C:\\Program Files\\Java\\jdk-17\\bin\\javac.exe" 
    -- local jar = "C:\\Program Files\\Java\\jdk-17\\bin\\jar.exe"

    local java_src_dir = path.join(target:autogendir(), "rules", "swig")
    local java_class_dir = java_src_dir..'/class'

    local user_outdir = find_user_outdir(fileconfig)
    if user_outdir then
        java_src_dir = user_outdir
    end

    os.mkdir(java_class_dir)

    -- get java files
    local autogenfiles = os.files(path.join(java_src_dir, "*.java"))

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

function swig_par(target, sourcefile, opt)
    -- get swig
    opt = opt or {}
    local swig = assert(find_tool("swig"), "swig not found!")
    local sourcefile_cx = path.join(target:autogendir(), "rules", "swig",
        path.basename(sourcefile) .. (opt.sourcekind == "cxx" and ".cpp" or ".c"))

    -- add objectfile
    local objectfile = target:objectfile(sourcefile_cx)
    table.insert(target:objectfiles(), objectfile)

    -- add commands
    local moduletype = assert(target:data("swig.moduletype"), "swig.moduletype not found!")
    local argv = {"-" .. moduletype, "-o", sourcefile_cx}
    if opt.sourcekind == "cxx" then
        table.insert(argv, "-c++")
    end
    local fileconfig = target:fileconfig(sourcefile)
    if fileconfig and fileconfig.swigflags then
        table.join2(argv, fileconfig.swigflags)
    end

    -- add includedirs
    local function _get_values_from_target(target, name)
        local values = {}
        for _, value in ipairs((target:get_from(name, "*"))) do
            table.join2(values, value)
        end
        return table.unique(values)
    end
    local pathmaps = {
        { "includedirs",    "includedir" },
        { "sysincludedirs", "includedir" },
        { "frameworkdirs",  "frameworkdir" }
    }
    for _, pathmap in ipairs(pathmaps) do
        for _, item in ipairs(_get_values_from_target(target, pathmap[1])) do
            table.join2(argv, "-I" .. item)
        end
    end



    table.insert(argv, sourcefile)
    return {
        argv = argv,
        objectfile = objectfile,
        swig = swig,
        sourcefile_cx = sourcefile_cx,
        moduletype = moduletype,
        fileconfig = fileconfig
    }
end

function swig_build_cmd(target, batchcmds, sourcefile, opt, pars)
    local par = swig_par(target, sourcefile, opt)

    local objectfile = par.objectfile
    local argv = par.argv
    local swig = par.swig
    local sourcefile_cx = par.sourcefile_cx
    local moduletype = par.moduletype
    local fileconfig = par.fileconfig

    batchcmds:show_progress(opt.progress, "${color.build.object}compiling.swig.%s %s", moduletype, sourcefile)
    batchcmds:mkdir(path.directory(sourcefile_cx))
    local rc = batchcmds:vrunv(swig.program, argv)
    if rc~=0 then
         raise("run swig ERROR")
    end
    batchcmds:compile(sourcefile_cx, objectfile)

    -- add deps
    batchcmds:add_depfiles(sourcefile)
    batchcmds:set_depmtime(os.mtime(objectfile))
    batchcmds:set_depcache(target:dependfile(objectfile))
end

function swig_build_file(target, sourcefile, opt, par)
    local par = swig_par(target, sourcefile, opt)

    local objectfile = par.objectfile
    local argv = par.argv
    local swig = par.swig
    local sourcefile_cx = par.sourcefile_cx
    local moduletype = par.moduletype
    local fileconfig = par.fileconfig

    local dependfile = target:dependfile(objectfile)
    local dependinfo = target:is_rebuilt() and {} or (depend.load(dependfile) or {})
    if not depend.is_changed(dependinfo, {lastmtime = os.mtime(objectfile),
                                          values = argv
                                          }) then
        return
    end

    progress.show(opt.progress, "${color.build.object}compiling.swig.%s %s", moduletype, sourcefile)
    os.mkdir(path.directory(sourcefile_cx))

    -- gen swig depend file , same with gcc .d
    local swigdep = os.tmpfile()
    local argv2 = {"-MMD", "-MF", swigdep}
    table.join2(argv2, argv)

    -- swig generate file and depend file
    os.vrunv(swig.program, argv2)
    compiler.compile(sourcefile_cx, objectfile, {target = target})

    -- update depend file
    local deps = io.readfile(swigdep, {continuation = "\\"})
    os.tryrm(swigdep)
    dependinfo.files = {sourcefile}
    dependinfo.depfiles_gcc = deps
    dependinfo.values = argv
    depend.save(dependinfo, target:dependfile(objectfile))

    -- jar build
    local buildjar = target:extraconf("rules", "swig1.c", "buildjar") or target:extraconf("rules", "swig1.cpp", "buildjar")
    if moduletype == "java" and buildjar then
        jar_build(target, fileconfig, opt)
        local java_src_dir = path.join(target:autogendir(), "rules", "swig")
        local jar_file = java_src_dir..'/'..target:name()..'.jar'
        local tar_file = path.directory(target:targetfile())..'/'
        cprint("${red}build SWIG jar："..jar_file..' => '..tar_file)
        os.cp(jar_file, tar_file)
    end

    local buildcsharp = target:extraconf("rules", "swig1.c", "buildcsharp") or target:extraconf("rules", "swig1.cpp", "buildcsharp")
    if moduletype == "csharp" and buildcsharp then
        csharp_build(target, fileconfig, opt)
    end

end


function csharp_build(target, fileconfig, opt)
    cprint("${red}build C#")
    cprint("${blue}build ")
    print(fileconfig)
    print(opt)
    cprint("${blue}build C#")
    local csc = nil
    if os.exists('C:\\Program Files (x86)\\MSBuild') then
        for _, one in ipairs({'14.0', '12.0'}) do
            local path1 = 'C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\csc.exe'
            --('C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\Roslyn\\csc.exe')
            if os.exists(path1) then
                csc = path1
                break
            end
            --print('f =', path, f)
        end
    end
    if csc==nil then
        raise("C# compiler (csc.exe) not found!")
    end
    --local javac = assert(find_tool("csc.exe"), "C# compiler (csc.exe) not found!")
    -- local jar = assert(find_tool("jar"), "jar not found!")

    local tpath = path.directory(target:targetfile())..'/'..target:name()..'_csharp.dll'
    local args = {}
    
    if fileconfig and fileconfig.cscflags then
        table.join2(args, fileconfig.cscflags)
    end

    table.join2(args, {'-target:'.. (is_exe and 'exe' or 'library'), '-platform:'..'anycpu', '-out:'..tpath})

    local is_exe = false
    local java_src_dir = path.join(target:autogendir(), "rules", "swig")
    local files = os.files(java_src_dir..'/*.cs')

    if fileconfig and fileconfig.cs_src then
        table.join2(files, fileconfig.cs_src)
    end

    table.join2(args, files);
    os.vrunv(csc, args)
    -- for _, one in ipairs(os.files(java_src_dir..'/*.cs')) do

    -- end
    -- local java_class_dir = java_src_dir..'/class' 

    -- local user_outdir = find_user_outdir(fileconfig)
    -- if user_outdir then
    --     java_src_dir = user_outdir
    -- end

    -- os.mkdir(java_class_dir)

    -- -- get java files
    -- local autogenfiles = os.files(path.join(java_src_dir, "*.java"))

    -- -- write file list
    -- local filelistname = path.join(java_src_dir, "buildlist.txt")
    -- local file = io.open(filelistname, "w")
    -- if file then
    --     for _, sourcebatch in ipairs(autogenfiles) do
    --         file:print(sourcebatch)
    --     end
    --     file:close()
        
    -- end

    -- -- compile to class file
    -- progress.show(opt.progress, "${color.build.object}compiling.javac %s class file", target:name())
    -- os.vrunv(javac.program, { "-d", java_class_dir, "@" .. filelistname}) --"--release", "17",  -"-Dfile.encoding=UTF-8",

    -- -- generate jar file
    -- progress.show(opt.progress, "${color.build.object}compiling.jar %s", target:name() .. ".jar")
    -- --os.vrunv(jar.program, {"-cvf", path.join(java_src_dir, target:name() .. ".jar"),  java_class_dir})
    -- os.vrunv(jar.program, {"-cvf", path.join(java_src_dir, target:name() .. ".jar"), '-C', java_class_dir, '.'})

    -- os.tryrm(filelistname)

    
end
