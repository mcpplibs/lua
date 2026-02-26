cmake_minimum_required(VERSION 4.0.2)

# https://github.com/Kitware/CMake/blob/master/Help/dev/experimental.rst
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_MODULE_STD 1)

project(mcpplibs-capi-lua VERSION 1.0.0 LANGUAGES CXX)

# Find Lua
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(LUA QUIET lua5.4 lua-5.4 lua54 lua)
endif()
if(NOT LUA_FOUND)
    find_package(Lua QUIET)
    if(LUA_FOUND)
        set(LUA_INCLUDE_DIRS ${LUA_INCLUDE_DIR})
        set(LUA_LIBRARIES ${LUA_LIBRARIES})
    endif()
endif()

# Library
add_library(mcpplibs-capi-lua STATIC)

file(GLOB_RECURSE MODULE_SOURCES "src/capi/*.cppm")

target_sources(mcpplibs-capi-lua
    PUBLIC
        FILE_SET CXX_MODULES FILES
        ${MODULE_SOURCES}
)

if(LUA_FOUND)
    target_include_directories(mcpplibs-capi-lua PUBLIC ${LUA_INCLUDE_DIRS})
    target_link_libraries(mcpplibs-capi-lua PUBLIC ${LUA_LIBRARIES})
endif()

# Test
add_executable(capi_lua_test tests/main.cpp)
target_link_libraries(capi_lua_test PRIVATE mcpplibs-capi-lua)

enable_testing()
add_test(NAME mcpplibs-capi-lua-test COMMAND capi_lua_test)

# Examples
foreach(example basic table function eval)
    add_executable(${example} examples/${example}.cpp)
    target_link_libraries(${example} PRIVATE mcpplibs-capi-lua)
endforeach()
