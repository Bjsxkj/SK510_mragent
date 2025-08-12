function main()
    import("core.project.task")
        import("core.base.option")
        -- import("core.base.io")
        if option.get('menu') then
            local cachepath = path.translate('./.mragent_export_cache')
            import("plugin_menuconf", {rootdir = path.translate(vformat("$(scriptdir)")), alias = "menuconf_show"})
            menuconf_show({cachepath=cachepath})
            io.save(cachepath, option.options())
        end
        local def_options = {}
        for _, one in ipairs(option.taskmenu().options) do
            local k = one[2]
            if one[4]~=nil and one[2]~=nil then
                def_options[one[2]] = one[4]
                --print(k, type(k), one[4])
                --option.set(k, one[4])
            end
        end
        local merge_opts = table.join(def_options, option.options())
        print('options', merge_opts)
        local okind = option.get('kind')
        local outdir = option.get('outputdir')
        local export_apk = option.get('export_apk')

        local exam_list = {}
        for k, v in pairs(merge_opts) do 
            if k:startswith('output_') and v then 
                table.join2(exam_list, {string.sub(k, 8)})
            end
        end

        if okind==nil or outdir==nil then 
            raise('must set kind or outputdir')
        end

        print('Output Examples:', exam_list)

        print('outdir:', outdir)

        os.mkdir(outdir)

        if okind=='android' then 
            
            os.mkdir(outdir..'/sdk')
            os.cp('./MRAgent/out/android/sdk-release.aar', outdir..'/sdk/mragent.aar')
            os.mkdir(outdir..'/document')
            os.cp('./doc/MRAgent/*', outdir..'/document/')
            os.mkdir(outdir..'/Examples')
            os.mkdir(outdir..'/APKs')
            
            local dir_map = { }
            -- for _, one in ipairs(exam_list) do
            --     local srcname = dir_map[one]
            --     if srcname ~= nil and os.exists('./Examples/android/'..srcname) then
            --         -- cprintf('copy: %s[%s]\n', one, srcname)
            --         -- os.mkdir(outdir..'/Examples/'..one)
            --         -- os.cp('./Examples/android/'..srcname..'/*|.gradle|.idea|.git|app/build|*.iml|app/libs', 
            --         --     outdir..'/Examples/'..one..'/')

            --         -- os.rm(outdir..'/Examples/'..one..'/**/build/')
            --         -- os.rm(outdir..'/Examples/'..one..'/**/.gradle/')
            --         -- os.rm(outdir..'/Examples/'..one..'/app/libs/')

            --         -- os.cp('./out/android/*', outdir..'/Examples/'..one..'/app/libs/')


            --         -- if export_apk then 
            --         --     os.cp('./Examples/android/'..srcname..'/app/build/outputs/**/app*.apk', outdir..'/APKs/'..one..'.apk')
            --         -- end 
            --     else
            --         cprintf('${red}Can not copy %s[%s]', one, srcname)

            --     end

            -- end

        elseif okind=='cpp' then    
            os.mkdir(outdir..'/lib')
            os.cp('./MRAgent/out/'..'windows', outdir..'/lib/')
            local dir_map = { ColorView = 'ColorViewer', DepthView = 'DepthViewer', ColorDepthView = 'ColorDepthViewer', 
                IRView = 'IrViewer', PointCloudView = 'PointCloudViewer'}
            os.mkdir(outdir..'/document')
            os.cp('./doc/MRAgent/*', outdir..'/document/')
            os.mkdir(outdir..'/Examples')
            os.cp('./MRAgent/cpp/include', outdir..'/')

        elseif okind=='u3d' then    

            os.mkdir(outdir..'/sdk')
            os.mkdir(outdir..'/sdk/Assets')
            os.mkdir(outdir..'/sdk/scripts')
            os.mkdir(outdir..'/sdk/lib')

            os.cp('./MRAgent/out/android/sdk-release.aar', outdir..'/sdk/Assets/mragent.aar')
            os.cp('./MRAgent/out/'..'windows', outdir..'/sdk/lib/')
            os.cp('./MRAgent/csharp/swig/*.cs', outdir..'/sdk/scripts/')

            os.mkdir(outdir..'/document')
            os.cp('./doc/MRAgent/*', outdir..'/document/')
            os.mkdir(outdir..'/Examples')

        else
            raise('this kind is not implemented')
        end
end