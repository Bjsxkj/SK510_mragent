
rule('csharp')
    set_extensions('.cs')
    on_load(function(target)
        local csc_path = 'C:\\Program Files (x86)\\MSBuild\\14.0\\Bin\\amd64\\csc.exe'
        target:set('values', 'csc_path', csc_path)
    end)

    on_build(function(target)
        local csc_path = target:values('csc_path')
        local files = target:sourcebatches().csharp.sourcefiles
        local is_exe = target:kind()=='binary'
        print('target:sourcebatches', files)

        --..(is_exe and '.exe' or '.dll') 
        local args = {'-target:'.. (is_exe and 'exe' or 'library'), '-platform:'..'anycpu', '-out:'..target:targetfile()}

        table.join2(args, files);

        print(args)

        os.execv(csc_path, args)

    end)


rule_end()