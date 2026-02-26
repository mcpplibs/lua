# mcpplibs.capi.lua 设计文档

> Lua C API 的 C++23 模块化绑定方案

## 1. 目标

为 Lua C API（`lua.h` / `lauxlib.h` / `lualib.h`）提供 C++23 模块化封装，使其他 mcpplibs 模块可以通过 `import mcpplibs.capi.lua;` 直接使用 Lua，无需手动 `#include` C 头文件。

## 2. 设计原则

1. **最简单的方式** — 不引入额外抽象层，1:1 映射 C API
2. **接口与 C 保持一致** — 函数签名、参数类型完全对应原始 C API
3. **命名空间去前缀** — 在 `mcpplibs::capi::lua` 命名空间内去掉 `lua_` / `luaL_` / `LUA_` / `luaopen_` 前缀，避免冗余
4. **遵循 mcpp-style-ref** — 类型 PascalCase、常量 UPPER_SNAKE、命名空间全小写

## 3. 命名映射规则

| C API 前缀 | 模块内规则 | 示例 |
|---|---|---|
| `lua_State` | 类型保留去 `lua_` → `State` | `mcpplibs::capi::lua::State` |
| `lua_Number` | 类型去 `lua_` → `Number` | `mcpplibs::capi::lua::Number` |
| `lua_gettop()` | 函数去 `lua_` → `gettop()` | `mcpplibs::capi::lua::gettop(L)` |
| `luaL_newstate()` | 函数去 `luaL_` → `L_newstate()` | `mcpplibs::capi::lua::L_newstate()` |
| `LUA_OK` | 常量去 `LUA_` → `OK` | `mcpplibs::capi::lua::OK` |
| `LUA_TNIL` | 常量去 `LUA_` → `TNIL` | `mcpplibs::capi::lua::TNIL` |
| `luaopen_base()` | 函数去 `luaopen_` → `open_base()` | `mcpplibs::capi::lua::open_base(L)` |

## 4. 模块结构

```
src/
└── capi/
    └── lua.cppm        # export module mcpplibs.capi.lua
```

### 模块文件布局

```cpp
module;

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

export module mcpplibs.capi.lua;

export namespace mcpplibs::capi::lua {

    // ===== 类型别名 =====
    using State = ::lua_State;
    using Number = ::lua_Number;
    // ...

    // ===== 常量 =====
    inline constexpr int OK = LUA_OK;
    inline constexpr int TNIL = LUA_TNIL;
    // ...

    // ===== 函数（inline 转发） =====
    inline State* L_newstate() { return ::luaL_newstate(); }
    inline void close(State* L) { ::lua_close(L); }
    // ...
}
```

### 技术要点

1. **全局模块片段** — `#include` C 头文件放在 `module;` 之后、`export module` 之前
2. **inline 转发函数** — 所有 C 函数和宏都封装为 `inline` 函数，零开销
3. **constexpr 常量** — C 宏常量转为 `inline constexpr`，类型安全
4. **变参函数** — `luaL_error` 等变参函数通过 C++ 模板参数包转发

## 5. 封装分类

### 5.1 类型（using 别名）

| 模块内名称 | C API 原名 |
|---|---|
| `State` | `lua_State` |
| `Number` | `lua_Number` |
| `Integer` | `lua_Integer` |
| `Unsigned` | `lua_Unsigned` |
| `CFunction` | `lua_CFunction` |
| `KFunction` | `lua_KFunction` |
| `KContext` | `lua_KContext` |
| `Alloc` | `lua_Alloc` |
| `Reader` | `lua_Reader` |
| `Writer` | `lua_Writer` |
| `WarnFunction` | `lua_WarnFunction` |
| `Hook` | `lua_Hook` |
| `Debug` | `lua_Debug` |
| `L_Reg` | `luaL_Reg` |
| `L_Buffer` | `luaL_Buffer` |

### 5.2 常量（inline constexpr）

- 类型标签：`TNONE`, `TNIL`, `TBOOLEAN`, `TNUMBER`, `TSTRING`, `TTABLE`, `TFUNCTION`, `TUSERDATA`, `TTHREAD`, `TLIGHTUSERDATA`
- 状态码：`OK`, `YIELD`, `ERRRUN`, `ERRSYNTAX`, `ERRMEM`, `ERRERR`, `ERRFILE`
- 比较运算：`OPEQ`, `OPLT`, `OPLE`
- 算术运算：`OPADD`, `OPSUB`, `OPMUL`, `OPMOD`, `OPPOW`, `OPDIV`, `OPIDIV`, `OPBAND`, `OPBOR`, `OPBXOR`, `OPSHL`, `OPSHR`, `OPUNM`, `OPBNOT`
- 其他：`MULTRET`, `REGISTRYINDEX`, `MINSTACK`, `RIDX_MAINTHREAD`, `RIDX_GLOBALS`, `NOREF`, `REFNIL`

### 5.3 函数（inline 转发）

覆盖以下类别：

- **状态管理** — `close`, `newthread`, `version`, `atpanic`
- **栈操作** — `gettop`, `settop`, `pop`, `pushvalue`, `rotate`, `copy`, `checkstack`, `absindex`, `insert`, `remove`, `replace`
- **入栈** — `pushnil`, `pushnumber`, `pushinteger`, `pushstring`, `pushlstring`, `pushboolean`, `pushcclosure`, `pushcfunction`, `pushlightuserdata`, `pushthread`, `pushglobaltable`
- **类型检查** — `type`, `type_name`, `isnumber`, `isstring`, `isinteger`, `iscfunction`, `isuserdata`, `isnil`, `isboolean`, `istable`, `isfunction`, `islightuserdata`, `isthread`, `isnone`, `isnoneornil`
- **取值** — `tonumberx`, `tointegerx`, `tonumber`, `tointeger`, `toboolean`, `tolstring`, `tostring`, `tocfunction`, `touserdata`, `tothread`, `topointer`, `rawlen`
- **表操作** — `createtable`, `newtable`, `gettable`, `getfield`, `geti`, `settable`, `setfield`, `seti`, `rawget`, `rawgeti`, `rawgetp`, `rawset`, `rawseti`, `rawsetp`, `getmetatable`, `setmetatable`, `next`
- **全局变量** — `getglobal`, `setglobal`
- **调用** — `callk`, `call`, `pcallk`, `pcall`
- **加载** — `load`, `dump`
- **算术/比较** — `arith`, `rawequal`, `compare`
- **其他** — `concat`, `len`, `error`, `status`, `gc`, `newuserdatauv`, `getiuservalue`, `setiuservalue`, `toclose`, `closeslot`
- **辅助库 (luaL)** — `L_newstate`, `L_openlibs`, `L_loadstring`, `L_loadfile`, `L_loadfilex`, `L_loadbufferx`, `L_dostring`, `L_dofile`, `L_ref`, `L_unref`, `L_checkinteger`, `L_checknumber`, `L_checkstring`, `L_optinteger`, `L_optnumber`, `L_checktype`, `L_checkany`, `L_newmetatable`, `L_setmetatable`, `L_getmetatable`, `L_len`, `L_gsub`, `L_setfuncs`, `L_requiref`, `L_traceback`, `L_typename`, `L_error`, `L_argerror`, `L_typeerror`, `L_checkudata`, `L_testudata`, `L_callmeta`, `L_getmetafield`, `L_getsubtable`, `L_where`
- **标准库** — `open_base`, `open_coroutine`, `open_table`, `open_io`, `open_os`, `open_string`, `open_utf8`, `open_math`, `open_debug`, `open_package`

## 6. 构建配置

### xmake（主构建系统）

```lua
add_requires("lua")

target("mcpplibs-capi-lua")
    set_kind("static")
    add_files("src/capi/*.cppm", { public = true, install = true })
    add_packages("lua", { public = true })
    set_policy("build.c++.modules", true)
```

### 依赖

- **Lua 5.4** — 通过 xmake `add_requires("lua")` 自动下载构建
- **Google Test** — 测试使用 `add_requires("gtest")`

## 7. 使用示例

```cpp
import std;
import mcpplibs.capi.lua;

namespace lua = mcpplibs::capi::lua;

int main() {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    lua::L_dostring(L, "print('hello from Lua!')");

    lua::close(L);
    return 0;
}
```
