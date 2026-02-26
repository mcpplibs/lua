add_rules("mode.release", "mode.debug")

set_languages("c++23")

add_requires("lua")

target("mcpplibs-capi-lua")
    set_kind("static")
    add_files("src/capi/*.cppm", { public = true, install = true })
    add_files("src/capi/*.cpp")
    add_packages("lua", { public = true })
    set_policy("build.c++.modules", true)

if not is_host("macosx") then
    includes("examples", "tests")
end
