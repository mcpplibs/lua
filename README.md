# mcpplibs capi-lua

> Lua C API 的 C++23 模块化绑定 - `import mcpplibs.capi.lua;`

将 Lua C API（`lua.h` / `lauxlib.h` / `lualib.h`）封装为 C++23 模块，让其他 mcpplibs 模块可以通过 `import` 直接使用 Lua，无需手动 `#include` C 头文件。

## 特性

- **C++23 模块** — `import mcpplibs.capi.lua;`
- **1:1 映射** — 接口与 Lua C API 完全对应，零额外抽象
- **零开销** — 所有封装均为 `inline` 函数和 `constexpr` 常量
- **双构建系统** — 同时支持 xmake 和 CMake
- **CI/CD** — GitHub Actions 多平台构建（Linux / macOS / Windows）
- **遵循 mcpp-style-ref** — 标准化命名和项目结构

## 命名映射

在 `mcpplibs::capi::lua` 命名空间内，去掉 C API 前缀避免冗余：

| C API | 模块内 | 示例 |
|---|---|---|
| `lua_State` | `State` | `lua::State*` |
| `lua_gettop()` | `gettop()` | `lua::gettop(L)` |
| `luaL_newstate()` | `L_newstate()` | `lua::L_newstate()` |
| `LUA_OK` | `OK` | `lua::OK` |
| `luaopen_base()` | `open_base()` | `lua::open_base(L)` |

## 快速开始

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

## 安装与配置

```bash
xlings install
```

## 构建与运行

**使用 xmake**

```bash
xmake build                 # 构建库
xmake run basic             # 基本用法示例
xmake run table             # 表操作示例
xmake run function          # 函数注册示例
xmake run eval              # 脚本求值示例
xmake run capi_lua_test     # 运行测试
```

**使用 CMake**

```bash
cmake -B build -G Ninja
cmake --build build
ctest --test-dir build
```

## 项目结构

```
mcpplibs-capi-lua/
├── src/                    # 模块源码
│   └── capi/
│       └── lua.cppm        # 主模块接口 (export module mcpplibs.capi.lua)
├── tests/                  # 测试
│   ├── main.cpp            # Google Test 测试套件
│   └── xmake.lua
├── examples/               # 示例
│   ├── basic.cpp           # 基本用法
│   ├── table.cpp           # 表操作
│   ├── function.cpp        # 函数注册与回调
│   ├── eval.cpp            # 脚本求值
│   └── xmake.lua
├── docs/                   # 文档
│   ├── architecture.md     # 架构文档
│   └── pr/
│       ├── design.md       # 设计文档
│       └── tasks.md        # 任务清单
├── .github/workflows/      # CI/CD
│   └── ci.yml
├── xmake.lua               # xmake 构建配置
├── CMakeLists.txt          # CMake 构建配置
└── config.xlings           # xlings 工具链配置
```

## 集成到构建工具

### xmake

```lua
add_repositories("mcpplibs-index https://github.com/mcpplibs/mcpplibs-index.git")

add_requires("capi-lua")

target("myapp")
    set_kind("binary")
    set_languages("c++23")
    add_files("main.cpp")
    add_packages("capi-lua")
    set_policy("build.c++.modules", true)
```

## 覆盖范围

封装了 Lua 5.4 C API 的核心功能：

- **类型** — `State`, `Number`, `Integer`, `CFunction`, `L_Reg`, `L_Buffer` 等
- **常量** — 状态码、类型标签、运算符、GC 选项、Hook 掩码等
- **状态管理** — `L_newstate`, `close`, `newthread`, `version` 等
- **栈操作** — `gettop`, `settop`, `pop`, `pushvalue`, `rotate`, `insert`, `remove`, `replace` 等
- **入栈/取值** — `pushnil/number/integer/string/boolean`, `tonumber/tointeger/tostring/toboolean` 等
- **表操作** — `newtable`, `getfield/setfield`, `gettable/settable`, `rawget/rawset`, `next` 等
- **函数调用** — `call`, `pcall`, `pushcfunction`, `pushcclosure` 等
- **辅助库** — `L_dostring`, `L_dofile`, `L_loadstring`, `L_ref/L_unref`, `L_setfuncs`, `L_error` 等
- **标准库** — `open_base`, `open_math`, `open_string`, `open_table`, `open_io`, `open_os` 等
- **调试** — `getstack`, `getinfo`, `sethook`, `gethook` 等

## 相关链接

- [mcpp-style-ref | 现代C++编码/项目风格参考](https://github.com/mcpp-community/mcpp-style-ref)
- [mcpplibs/templates | 项目模板](https://github.com/mcpplibs/templates)
- [mcpp社区官网](https://mcpp.d2learn.org)
- [mcpp | 现代C++爱好者论坛](https://mcpp.d2learn.org/forum)
