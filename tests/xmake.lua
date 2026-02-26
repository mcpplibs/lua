add_rules("mode.debug", "mode.release")

set_languages("c++23")

add_requires("gtest")
add_requires("lua")

target("capi_lua_test")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("mcpplibs-capi-lua")
    add_packages("gtest", "lua")
    set_policy("build.c++.modules", true)
