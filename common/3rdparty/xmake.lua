
package('libzmq-4.3')
    set_description("libzmp 4.3.4")
    set_sourcedir(path.join(os.scriptdir(), "libzmq-4.3.4"))

    on_fetch(function(package) 
        local result = {}
        local dir = package:sourcedir()

        if is_plat('windows') then
            result.links = {is_mode("release") and 'libzmq' or 'libzmqd'}

            local mode = is_mode("release") and 'release' or 'debug'
            result.linkdirs = {path.join(dir, 'lib', package:arch(), mode)}
            print(result)
            os.cp(path.join(dir, 'lib', package:arch(), mode, is_mode("release") and 'libzmq.dll' or 'libzmqd.dll'), '$(buildir)/$(plat)/$(arch)/$(mode)')
        elseif is_plat('android') then
            result.links =  {'zmq' }
            result.linkdirs = path.join(dir, 'lib', 'android', package:arch())
        end
        
        result.includedirs = {path.join(dir, 'include')}
        
        return result
    end)
package_end()
