add_rules("mode.debug", "mode.release")

set_languages("c++23")

add_requires("lua")

for _, name in ipairs({"basic", "table", "function", "eval"}) do
    target(name)
        set_kind("binary")
        add_files(name .. ".cpp")
        add_deps("mcpplibs-capi-lua")
        add_packages("lua")
        set_policy("build.c++.modules", true)
end
