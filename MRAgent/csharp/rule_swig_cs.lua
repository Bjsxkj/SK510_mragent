--实现 swig 转 c#

rule("swig_cs")
    set_extensions(".i")
    -- on_clean(function (target)
    --     local x = target:values("swig_dir")
    --     local outdir = vformat("$(scriptdir)/swig/cpp") -- (target:get('targetdir'))
    --     if type(x)=='string' then
    --         outdir = vformat("$(scriptdir)/%s", x)
    --     elseif type(x)=='table' then
    --         for one in x  do
    --             outdir = vformat("$(scriptdir)/%s", one)
    --         end
    --     end
    --     cprint('${red}clean '..outdir)
    --     os.rm(outdir..'/*')
    -- end)
    before_build(function (target)
        print('build swig convert')
        local sourcebatches = (target:sourcebatches().swig_cs)

        print(sourcebatches, target:get('includedirs'))
        for _, one in ipairs(target:get('includedirs')) do
            cprint('Find swig search include dir: '..one)
        end
        --local
        local x = target:values("swig_dir")
        local outdir = vformat("$(scriptdir)/swig/") -- (target:get('targetdir'))
        if type(x)=='string' then
            outdir = vformat("$(scriptdir)/%s", x)
        elseif type(x)=='table' then
            for one in x  do
                outdir = vformat("$(scriptdir)/%s", one)
            end
        end
        cprint('swig outdir: '.. outdir)
        os.mkdir(outdir)

        os.rm(outdir..'/*')

        local out_cpp_dir = outdir..'/' --'/cpp/'
        os.mkdir(out_cpp_dir)
        local out_cs_dir = outdir..'/' --'/csharp/'
        os.mkdir(out_cs_dir)

        local cs_namespace = 'cs_api'
        local np = target:values("cs_namespace")
        if np then
            cs_namespace = np
        else
            raise('cs_namespace must be a string')
        end

        local swigflags = target:values("swigflags")

        cprint("${red},np=%s", cs_namespace)
        print("${red}swig", swigflags,',np=',np)

        for _,one in ipairs(sourcebatches.sourcefiles) do

            -- local a = path.relative(one, vformat("$(scriptdir)"))

            -- local java_idx = string.find(a, 'java/')

            --print('one=',one, 'a=',a, java_idx , string.sub( a, 0, java_idx + 4))

            if true then  --a:startswith('*java/') then
                -- local java_root_src = string.sub( a, 0, java_idx + 4)
                -- local pkpath = path.directory(a)
                -- pkpath = path.relative(pkpath, java_root_src)

                -- cprint('Find java package path: ${green}'..pkpath)
                -- local pkname = string.gsub(pkpath,'/','.')

                -- local javadir = outdir .. '/java/' .. pkpath
                
                
                -- os.mkdir(javadir)
                -- os.rm(javadir..'/*')
                local cmdargs = {}
                
                table.join2(cmdargs, swigflags)

                for _, one1 in ipairs(target:get('includedirs')) do
                    table.join2(cmdargs, '-I' .. one1)
                end
                local mod = path.basename(one)
                local outcpp = out_cpp_dir..'/'..mod..'Swig.cpp'

                table.join2(cmdargs, {
                    "-csharp", "-c++", -- "-doxygen",
                    "-namespace", cs_namespace,
                    --"-outfile", 'a.cpp', 
                    "-o", outcpp,  --outcpp,
                     one
                })


                print('swig', cmdargs)
                

                os.execv("swig",cmdargs)
                --
                --target:add('files', outcpp)
                target:add('includedir', (out_cpp_dir))

            end
        end
    end)
    -- on_build_file(function (target, sourcefile, opt)
    --     print('ok=',opt.progress)
    --     print(sourcefile)
    -- end)
    -- on_build_files(function (target, sourcebatch, opt)
    --     -- build some source files
    --     -- print(sourcebatch.sourcefiles)
    --     for _, sourcefile in ipairs(sourcebatch.sourcefiles) do
    --         -- ...
    --          print(sourcefile)
    --     end
    -- end)
    -- on_buildcmd_file(function (target, batchcmds, sourcefile, opt)
    --     batchcmds:vrunv('echo abc') --"gcc", {"-o", objectfile, "-c", sourcefile})
    --     --batchcmds:add_depfiles("/xxxxx/dependfile.h", ...)
    --     -- batchcmds:add_depvalues(...)
    --     -- batchcmds:set_depmtime(os.mtime(...))
    --     -- batchcmds:set_depcache("xxxx.d")
    -- end)

rule_end()

