add_rules("mode.release", "mode.debug")


target("adbs_lab")
    set_kind("binary")
    add_files("test/main.cpp")
    add_includedirs("include")
    add_includedirs("include/argparse")
    add_files("src/*.cpp")
    set_rundir("$(projectdir)/build")
    add_defines("DEBUG")
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    end

target("adbs_lab_multi_thread")
    set_kind("binary")
    add_files("test/main_multi_thread.cpp")
    add_includedirs("include")
    add_includedirs("include/argparse")
    add_files("src/*.cpp")
    set_rundir("$(projectdir)/build")
    add_defines("DEBUG")
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    end