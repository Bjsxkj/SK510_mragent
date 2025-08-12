--[[
MrAgent 打包工具
--]]

task("mragent_export")
    set_category("plugin")
    -- the main entry of the plugin
    on_run(function ()
        import("main", {rootdir = path.translate(vformat("$(scriptdir)"))})
        main()
    end)

    local options = {
            {nil, 'import', 'kv', './.xmake/mycache', 'Import cache config path'}
            , {'c', "clean", "k",  false,         "Clean cache"     }
            , {nil, "menu", "k",  false,         "Config by Menu UI" }
            , {nil, "outputdir"   ,  "v" , nil , 'Output dir', category = 'project'} 
            , {'k', 'kind', 'kv', nil, 'Set Target kind', values={'cpp', 'android', 'u3d'}}
            }

    -- local examples = { ColorView = true, DepthView = true, ColorDepthView = true, FloodIRView = true,
    --     PointCloudView = true}
    
    -- for one, a in pairs(examples) do 
    --     table.join2(options, {{nil, 'output_'..one, 'k', a, 'Output "'..one..'" Examples', category = 'Examples'}})
    -- end

    set_menu { usage = "xmake mragent_export [options] outputdir"
        ,   description = "生成MrAgent SDK!"
            -- options
        , options = options
    }
    

task_end()