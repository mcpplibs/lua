---
name: mcpplibs-capi-lua
description: 使用 mcpplibs.capi.lua 模块（Lua C API 的 C++23 模块化绑定）。适用于编写使用 Lua 的 C++23 代码、嵌入 Lua 脚本引擎、注册 C++ 函数到 Lua、操作 Lua 栈和表，或用户提及 Lua binding、capi.lua、嵌入 Lua 时。
---

# mcpplibs-capi-lua

Lua 5.4 C API 的 C++23 模块化绑定。`import mcpplibs.capi.lua;` 即可使用完整 Lua C API，无需 `#include`。

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

## 命名映射

命名空间 `mcpplibs::capi::lua` 内去掉 C API 前缀：

| C API | 模块内 | 说明 |
|---|---|---|
| `lua_State` | `State` | 去 `lua_` |
| `lua_gettop(L)` | `gettop(L)` | 去 `lua_` |
| `luaL_newstate()` | `L_newstate()` | 去 `lua` 保留 `L_` |
| `LUA_OK` | `OK` | 去 `LUA_` |
| `luaopen_base(L)` | `open_base(L)` | 去 `luaopen_` 改 `open_` |
| `lua_typename(L,t)` | `type_name(L,t)` | 避免与 `type()` 冲突 |
| `lua_register(L,n,f)` | `register_func(L,n,f)` | 避免关键字冲突 |
| `lua_upvalueindex(i)` | `upvalueindex(i)` | 去 `lua_` |

## 类型

```cpp
lua::State        // lua_State
lua::Number       // lua_Number (double)
lua::Integer      // lua_Integer (long long)
lua::Unsigned     // lua_Unsigned
lua::CFunction    // int (*)(lua_State*)
lua::KFunction    // lua_KFunction
lua::KContext     // lua_KContext
lua::Alloc        // lua_Alloc
lua::Reader       // lua_Reader
lua::Writer       // lua_Writer
lua::Hook         // lua_Hook
lua::Debug        // lua_Debug
lua::L_Reg        // luaL_Reg — { name, func } 注册表
lua::L_Buffer     // luaL_Buffer
```

## 常量

```cpp
// 状态码
lua::OK, lua::YIELD, lua::ERRRUN, lua::ERRSYNTAX, lua::ERRMEM, lua::ERRERR, lua::ERRFILE

// 类型标签
lua::TNONE, lua::TNIL, lua::TBOOLEAN, lua::TLIGHTUSERDATA, lua::TNUMBER,
lua::TSTRING, lua::TTABLE, lua::TFUNCTION, lua::TUSERDATA, lua::TTHREAD

// 算术运算
lua::OPADD, lua::OPSUB, lua::OPMUL, lua::OPDIV, lua::OPIDIV, lua::OPMOD, lua::OPPOW,
lua::OPBAND, lua::OPBOR, lua::OPBXOR, lua::OPSHL, lua::OPSHR, lua::OPUNM, lua::OPBNOT

// 比较运算
lua::OPEQ, lua::OPLT, lua::OPLE

// 其他
lua::MULTRET, lua::REGISTRYINDEX, lua::MINSTACK
lua::RIDX_MAINTHREAD, lua::RIDX_GLOBALS, lua::NOREF, lua::REFNIL

// GC
lua::GCSTOP, lua::GCRESTART, lua::GCCOLLECT, lua::GCCOUNT, lua::GCCOUNTB,
lua::GCSTEP, lua::GCSETPAUSE, lua::GCSETSTEPMUL, lua::GCISRUNNING, lua::GCGEN, lua::GCINC

// Hook
lua::MASKCALL, lua::MASKRET, lua::MASKLINE, lua::MASKCOUNT
lua::HOOKCALL, lua::HOOKRET, lua::HOOKLINE, lua::HOOKCOUNT, lua::HOOKTAILCALL
```

## 核心 API 速查

### 状态管理

```cpp
auto* L = lua::L_newstate();       // 创建状态
lua::L_openlibs(L);                // 打开标准库
lua::close(L);                     // 关闭状态
lua::version(L);                   // Lua 版本号 (504)
```

### 栈操作

```cpp
lua::gettop(L)                     // 栈顶索引（元素数量）
lua::settop(L, idx)                // 设置栈顶
lua::pop(L, n)                     // 弹出 n 个元素
lua::pushvalue(L, idx)             // 复制到栈顶
lua::rotate(L, idx, n)             // 旋转栈元素
lua::copy(L, from, to)             // 复制值
lua::insert(L, idx)                // 插入（栈顶移到 idx）
lua::remove(L, idx)                // 移除 idx 处元素
lua::replace(L, idx)               // 替换（栈顶覆盖 idx）
lua::checkstack(L, n)              // 确保栈有 n 个空位
lua::absindex(L, idx)              // 转绝对索引
lua::xmove(from, to, n)            // 线程间移动值
```

### 入栈

```cpp
lua::pushnil(L)
lua::pushnumber(L, 3.14)
lua::pushinteger(L, 42)
lua::pushstring(L, "hello")
lua::pushlstring(L, ptr, len)
lua::pushboolean(L, 1)
lua::pushcfunction(L, fn)          // fn: lua::CFunction
lua::pushcclosure(L, fn, nup)      // 带 nup 个 upvalue
lua::pushlightuserdata(L, ptr)
lua::pushglobaltable(L)
lua::pushfstring(L, fmt, ...)      // 格式化字符串
```

### 取值

```cpp
lua::tonumber(L, idx)              // → Number
lua::tointeger(L, idx)             // → Integer
lua::toboolean(L, idx)             // → int (0/1)
lua::tostring(L, idx)              // → const char*
lua::tolstring(L, idx, &len)       // → const char* + 长度
lua::tocfunction(L, idx)           // → CFunction
lua::touserdata(L, idx)            // → void*
lua::tothread(L, idx)              // → State*
lua::topointer(L, idx)             // → const void*
lua::rawlen(L, idx)                // 原始长度
lua::tonumberx(L, idx, &isnum)     // 带成功标志
lua::tointegerx(L, idx, &isnum)    // 带成功标志
```

### 类型检查

```cpp
lua::type(L, idx)                  // → int (TNIL, TNUMBER, ...)
lua::type_name(L, tp)              // → "nil", "number", ...
lua::L_typename(L, idx)            // type_name(L, type(L, idx))
lua::isnumber(L, idx)
lua::isstring(L, idx)
lua::isinteger(L, idx)
lua::isfunction(L, idx)
lua::istable(L, idx)
lua::isnil(L, idx)
lua::isboolean(L, idx)
lua::iscfunction(L, idx)
lua::isuserdata(L, idx)
lua::islightuserdata(L, idx)
lua::isthread(L, idx)
lua::isnone(L, idx)
lua::isnoneornil(L, idx)
```

### 表操作

```cpp
lua::newtable(L)                   // 创建空表
lua::createtable(L, narr, nrec)    // 预分配

lua::setfield(L, idx, "key")       // t[key] = 栈顶，弹出值
lua::getfield(L, idx, "key")       // 压入 t[key]

lua::settable(L, idx)              // t[栈顶-1] = 栈顶，弹出 k+v
lua::gettable(L, idx)              // 栈顶为 key，替换为 t[key]

lua::seti(L, idx, n)               // t[n] = 栈顶
lua::geti(L, idx, n)               // 压入 t[n]

lua::rawset(L, idx)                // 不触发元方法
lua::rawget(L, idx)
lua::rawseti(L, idx, n)
lua::rawgeti(L, idx, n)

lua::setmetatable(L, idx)          // 设置元表
lua::getmetatable(L, idx)          // 获取元表
lua::next(L, idx)                  // 遍历（需先 pushnil）
```

### 全局变量

```cpp
lua::setglobal(L, "name")          // _G[name] = 栈顶
lua::getglobal(L, "name")          // 压入 _G[name]
```

### 脚本执行

```cpp
lua::L_dostring(L, "code")         // 加载并执行字符串
lua::L_dofile(L, "path.lua")       // 加载并执行文件
lua::L_loadstring(L, "code")       // 加载为函数（不执行）
lua::L_loadfile(L, "path.lua")     // 加载文件为函数
lua::pcall(L, nargs, nresults, 0)  // 安全调用（有错误处理）
lua::call(L, nargs, nresults)      // 调用（无错误处理）
```

### 注册 C++ 函数

```cpp
// 简单注册
lua::register_func(L, "add", my_add);

// 带 upvalue 的闭包
lua::pushinteger(L, 100);
lua::pushcclosure(L, my_closure, 1);  // 1 个 upvalue
lua::setglobal(L, "add_100");

// 访问 upvalue
auto val = lua::tointeger(L, lua::upvalueindex(1));

// 注册模块（多个函数）
static const lua::L_Reg mylib[] = {
    {"add", my_add},
    {"sub", my_sub},
    {nullptr, nullptr}
};
lua::newtable(L);
lua::L_setfuncs(L, mylib, 0);
lua::setglobal(L, "mylib");
```

### 辅助库

```cpp
lua::L_ref(L, lua::REGISTRYINDEX)         // 创建引用
lua::L_unref(L, lua::REGISTRYINDEX, ref)  // 释放引用
lua::L_newmetatable(L, "TypeName")        // 创建/获取命名元表
lua::L_setmetatable(L, "TypeName")        // 设置栈顶对象的元表
lua::L_checkudata(L, arg, "TypeName")     // 检查参数为 userdata
lua::L_checkinteger(L, arg)               // 检查参数为整数
lua::L_checknumber(L, arg)                // 检查参数为数字
lua::L_checkstring(L, arg)                // 检查参数为字符串
lua::L_error(L, "msg")                    // 抛出错误
lua::L_len(L, idx)                        // #t
lua::L_gsub(L, s, pat, rep)               // 字符串替换
lua::L_traceback(L, L1, msg, level)       // 调用栈回溯
lua::L_requiref(L, name, openf, glb)      // 注册模块
```

### Buffer

```cpp
lua::L_Buffer buf;
lua::L_buffinit(L, &buf);
lua::L_addstring(&buf, "hello");
lua::L_addlstring(&buf, ptr, len);
lua::L_pushresult(&buf);            // 将结果压入栈
```

### 标准库

```cpp
lua::open_base(L)       lua::open_math(L)
lua::open_string(L)     lua::open_table(L)
lua::open_io(L)         lua::open_os(L)
lua::open_debug(L)      lua::open_package(L)
lua::open_coroutine(L)  lua::open_utf8(L)
```

### GC / 协程 / 调试

```cpp
lua::gc(L, lua::GCCOLLECT)                // 完整 GC
lua::gc(L, lua::GCCOUNT)                  // 内存 KB

lua::status(L)                            // 线程状态
lua::resume(L, from, narg, &nres)         // 恢复协程

lua::sethook(L, hook, lua::MASKCALL, 0)   // 设置调试钩子
lua::gethook(L)                           // 获取钩子
```

## 常见模式

### 执行 Lua 并获取结果

```cpp
auto* L = lua::L_newstate();
lua::L_openlibs(L);
lua::L_dostring(L, "x = 6 * 7");
lua::getglobal(L, "x");
auto result = lua::tointeger(L, -1);  // 42
lua::pop(L, 1);
lua::close(L);
```

### 调用 Lua 函数

```cpp
lua::L_dostring(L, "function add(a,b) return a+b end");
lua::getglobal(L, "add");
lua::pushinteger(L, 10);
lua::pushinteger(L, 20);
if (lua::pcall(L, 2, 1, 0) == lua::OK) {
    auto sum = lua::tointeger(L, -1);  // 30
    lua::pop(L, 1);
}
```

### C++ 回调函数

```cpp
// 回调签名必须用 lua::State* （不是 lua_State*）
static int my_add(lua::State* L) {
    auto a = lua::tonumber(L, 1);
    auto b = lua::tonumber(L, 2);
    lua::pushnumber(L, a + b);
    return 1;  // 返回值个数
}
lua::register_func(L, "add", my_add);
```

### 创建和遍历表

```cpp
// 创建
lua::newtable(L);
lua::pushinteger(L, 42);
lua::setfield(L, -2, "x");
lua::setglobal(L, "point");

// 遍历
lua::getglobal(L, "t");
lua::pushnil(L);
while (lua::next(L, -2) != 0) {
    // key 在 -2，value 在 -1
    lua::pop(L, 1);  // 弹出 value，保留 key 供 next 使用
}
```

## 构建配置

### xmake

```lua
add_requires("lua")

target("myapp")
    set_kind("binary")
    set_languages("c++23")
    add_files("main.cpp")
    add_deps("mcpplibs-capi-lua")
    add_packages("lua")
    set_policy("build.c++.modules", true)
```

### 构建与运行

```bash
xmake build
xmake run capi_lua_test     # 测试（97 tests）
xmake run basic             # 基本示例
xmake run table             # 表操作示例
xmake run function          # 函数注册示例
xmake run eval              # 脚本求值示例
```

## 注意事项

1. **回调函数签名**：必须使用 `lua::State*` 而非 `lua_State*`，因为 `lua_State` 在模块外不可见
2. **`tostring` 陷阱**：对非字符串值调用 `lua::tostring()` 返回 `nullptr`，需先用 `lua::isstring()` 检查
3. **接口+实现分离**：模块使用 `.cppm`（声明）+ `.cpp`（定义），不能用 `inline`（GCC C++ 模块限制）
4. **extern "C" 包装**：C 头文件通过 `src/capi/lua_headers.h` 包装 `extern "C"`，解决 GCC 模块链接问题
5. **Lua 包依赖传递**：使用此库的 target 需要同时 `add_deps("mcpplibs-capi-lua")` 和 `add_packages("lua")`

## 更多资源

- 完整 API 参考：[reference.md](reference.md)
- 设计文档：[docs/pr/design.md](../../../docs/pr/design.md)
- Lua 5.4 官方手册：[lua.org/manual/5.4](https://www.lua.org/manual/5.4/)
- mcpp-style-ref：[mcpp-style-ref](../mcpp-style-ref/SKILL.md)
