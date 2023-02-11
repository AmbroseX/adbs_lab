add_rules("mode.release", "mode.debug")
target("adbs_lab")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("test/*.cpp")
    set_rundir("$(projectdir)/build")
    add_includedirs("include")
    add_includedirs("include/argparse")
    add_defines("DEBUG")
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    end
