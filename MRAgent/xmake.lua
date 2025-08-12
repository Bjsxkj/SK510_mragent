-- 操作类型的实现
target('mragent_cpp')
    set_kind('static')
    set_group('mragent')
    add_includedirs("../common")
    add_files('../common/*.cpp')
    add_defines('MRAGENT')
    add_files('cpp/src/*.cpp|ImiMrTypes.cpp', 'cpp/common/*.cpp')
    add_includedirs('../common')
    add_includedirs('cpp/include', 'cpp/src', 'cpp/common', 'cpp/common/mpp/inc', 'cpp/common/mpp/inc/osal/inc', 'cpp/common/mpp/inc/utils/inc', 'cpp/common/libjpegturbo/include', {public=true})
    add_cxxflags('-fPIC')
    if is_plat('android') or is_plat('linux') then
        if is_mode("debug") then
            add_cxflags('-g', '-O0')
        end
    end
    if is_plat('android') then
        add_defines('ANDROID')
        add_syslinks("log", "android")
        add_linkdirs("cpp/common/mpp/lib")
        add_linkdirs("cpp/common/libjpegturbo/lib")
        add_links("mpp")
        add_links("osal")
        add_links("codec_avsd")
        add_links("codec_avs2d")
        add_links("codec_h263d")
        add_links("codec_h264d")
        add_links("codec_h265d")
        add_links("dec_common")
        add_links("codec_mpeg2d")
        add_links("codec_mpeg4d")
        add_links("codec_vp8d")
        add_links("codec_vp9d")
        add_links("codec_jpegd")
        add_links("codec_av1d")
        add_links("codec_jpege")
        add_links("codec_h265e")
        add_links("codec_vp8e")
        add_links("codec_dummy_enc")
        add_links("codec_dummy_dec")
        add_links("mpp_hal")
        add_links("hal_h264d")
        add_links("mpp_hal")
        add_links("hal_h264d")
        add_links("hal_avsd")
        add_links("hal_avs2d")
        add_links("hal_h263d")
        add_links("hal_h265d")
        add_links("hal_mpeg2d")
        add_links("hal_mpeg4d")
        add_links("hal_vp8d")
        add_links("hal_vp9d")
        add_links("hal_jpegd")
        add_links("hal_av1d")
        add_links("hal_av1d_rkv")
        add_links("hal_av1d_vpu")
        add_links("hal_av1d_com")
        add_links("hal_h264e")
        add_links("hal_h264e_rkv")
        add_links("hal_h264e_vpu")
        add_links("codec_h264e")
        add_links("enc_rc")
        add_links("hal_h264e_com")
        add_links("hal_jpege")
        add_links("hal_jpege_rkv")
        add_links("hal_jpege_com")
        add_links("hal_jpege_vpu")
        add_links("hal_h265e")
        add_links("hal_h265e_vepu541")
        add_links("hal_h265e_com")
        add_links("hal_h265e_vepu541")
        add_links("hal_h265e_com")
        add_links("hal_vepu541_common")
        add_links("hal_common")
        add_links("hal_vp8e")
        add_links("hal_vepu_common")
        add_links("hal_dummy")
        add_links("vdpu383_com")
        add_links("vdpu34x_com")
        add_links("vdpu384a_com")
        add_links("mpp_vproc")
        add_links("vproc_rga")
        add_links("vproc_iep")
        add_links("vproc_iep2")
        add_links("utils")
        add_links("ion")
        add_links("jpegturbo")
    end

    if is_plat('windows') then
        add_includedirs('c:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.10240.0\\ucrt', {public=true})

        --add_defines('NOTGAME')
    end
    
    add_packages('libzmq-4.3', {public=true})

    after_build(function(target)
        print('==================================================================================copying mragent.toml to target dir')
        os.cp(path.join(os.scriptdir(), 'mragent.toml'), path.join(target:targetdir(), 'mragent.toml'))
    end)
target_end()


-- includes('cpp/test')


includes('csharp')
includes('java_lang')

--编译所有的MrAgent的库
target('mragent_all')
    set_kind('shared')
    set_group('mragent')
    add_deps('mragent_jni', 'mragent_cs')
    add_files('cpp/x.cpp')
    after_build(function (target) 
        import('core.project.project')
        local outdir = '$(scriptdir)/out/$(plat)/$(arch)/'
		for _, one in ipairs(target:get('deps')) do
            local t = project.target(one)
            print('cp ', t:targetfile(), '==>', outdir)
			os.cp(t:targetfile(), outdir)
            -- copy C# files dll to out 
            local cs_dll_file = t:targetdir()..'/'..t:name()..'_files.dll'
            if os.isfile(cs_dll_file) then
               os.cp(cs_dll_file, '$(scriptdir)/out/cs_dlls/')
            end
            if is_plat('windows') then 
                os.cp(t:targetdir()..'/'..t:name()..'.lib', outdir)
            end
		end
    end)
