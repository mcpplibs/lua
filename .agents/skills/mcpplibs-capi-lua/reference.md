# mcpplibs.capi.lua 完整 API 参考

`import mcpplibs.capi.lua;` — 命名空间 `mcpplibs::capi::lua`

## 一、类型别名

| 模块内名称 | C API 原名 | 说明 |
|---|---|---|
| `State` | `lua_State` | Lua 虚拟机状态 |
| `Number` | `lua_Number` | 浮点数（通常 `double`） |
| `Integer` | `lua_Integer` | 整数（通常 `long long`） |
| `Unsigned` | `lua_Unsigned` | 无符号整数 |
| `CFunction` | `lua_CFunction` | C 函数指针 `int (*)(lua_State*)` |
| `KFunction` | `lua_KFunction` | 延续函数指针 |
| `KContext` | `lua_KContext` | 延续上下文 |
| `Alloc` | `lua_Alloc` | 分配器函数指针 |
| `Reader` | `lua_Reader` | 读取器函数指针 |
| `Writer` | `lua_Writer` | 写入器函数指针 |
| `WarnFunction` | `lua_WarnFunction` | 警告处理函数 |
| `Hook` | `lua_Hook` | 调试钩子函数 |
| `Debug` | `lua_Debug` | 调试信息结构体 |
| `L_Reg` | `luaL_Reg` | 函数注册表 `{ name, func }` |
| `L_Buffer` | `luaL_Buffer` | 字符串缓冲区 |

---

## 二、常量

### 状态码

| 模块内 | C API 原名 | 值 | 说明 |
|---|---|---|---|
| `OK` | `LUA_OK` | 0 | 无错误 |
| `YIELD` | `LUA_YIELD` | 1 | 线程挂起 |
| `ERRRUN` | `LUA_ERRRUN` | 2 | 运行时错误 |
| `ERRSYNTAX` | `LUA_ERRSYNTAX` | 3 | 语法错误 |
| `ERRMEM` | `LUA_ERRMEM` | 4 | 内存分配错误 |
| `ERRERR` | `LUA_ERRERR` | 5 | 错误处理函数出错 |
| `ERRFILE` | `LUA_ERRFILE` | 6 | 文件相关错误 |

### 类型标签

| 模块内 | C API 原名 | 值 | 说明 |
|---|---|---|---|
| `TNONE` | `LUA_TNONE` | -1 | 无效索引 |
| `TNIL` | `LUA_TNIL` | 0 | nil |
| `TBOOLEAN` | `LUA_TBOOLEAN` | 1 | 布尔值 |
| `TLIGHTUSERDATA` | `LUA_TLIGHTUSERDATA` | 2 | 轻量用户数据 |
| `TNUMBER` | `LUA_TNUMBER` | 3 | 数字 |
| `TSTRING` | `LUA_TSTRING` | 4 | 字符串 |
| `TTABLE` | `LUA_TTABLE` | 5 | 表 |
| `TFUNCTION` | `LUA_TFUNCTION` | 6 | 函数 |
| `TUSERDATA` | `LUA_TUSERDATA` | 7 | 完整用户数据 |
| `TTHREAD` | `LUA_TTHREAD` | 8 | 线程 |

### 算术运算符

`OPADD`, `OPSUB`, `OPMUL`, `OPMOD`, `OPPOW`, `OPDIV`, `OPIDIV`, `OPBAND`, `OPBOR`, `OPBXOR`, `OPSHL`, `OPSHR`, `OPUNM`, `OPBNOT`

### 比较运算符

`OPEQ`, `OPLT`, `OPLE`

### 其他

| 模块内 | 说明 |
|---|---|
| `MULTRET` | 多返回值标记 (-1) |
| `REGISTRYINDEX` | 注册表伪索引 |
| `MINSTACK` | 最小栈保证大小 |
| `RIDX_MAINTHREAD` | 注册表中主线程的索引 |
| `RIDX_GLOBALS` | 注册表中全局表的索引 |
| `NOREF`, `REFNIL` | luaL_ref 特殊值 |

### GC 选项

`GCSTOP`, `GCRESTART`, `GCCOLLECT`, `GCCOUNT`, `GCCOUNTB`, `GCSTEP`, `GCSETPAUSE`, `GCSETSTEPMUL`, `GCISRUNNING`, `GCGEN`, `GCINC`

### Hook 掩码与事件

掩码：`MASKCALL`, `MASKRET`, `MASKLINE`, `MASKCOUNT`

事件：`HOOKCALL`, `HOOKRET`, `HOOKLINE`, `HOOKCOUNT`, `HOOKTAILCALL`

---

## 三、函数

### 3.1 状态管理

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `newstate` | `lua_newstate` | `State* (Alloc f, void* ud)` |
| `close` | `lua_close` | `void (State* L)` |
| `newthread` | `lua_newthread` | `State* (State* L)` |
| `atpanic` | `lua_atpanic` | `CFunction (State* L, CFunction panicf)` |
| `version` | `lua_version` | `Number (State* L)` |

### 3.2 栈操作

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `absindex` | `lua_absindex` | `int (State* L, int idx)` |
| `gettop` | `lua_gettop` | `int (State* L)` |
| `settop` | `lua_settop` | `void (State* L, int idx)` |
| `pop` | `lua_pop` (宏) | `void (State* L, int n)` |
| `pushvalue` | `lua_pushvalue` | `void (State* L, int idx)` |
| `rotate` | `lua_rotate` | `void (State* L, int idx, int n)` |
| `copy` | `lua_copy` | `void (State* L, int fromidx, int toidx)` |
| `checkstack` | `lua_checkstack` | `int (State* L, int n)` |
| `xmove` | `lua_xmove` | `void (State* from, State* to, int n)` |
| `insert` | `lua_insert` (宏) | `void (State* L, int idx)` |
| `remove` | `lua_remove` (宏) | `void (State* L, int idx)` |
| `replace` | `lua_replace` (宏) | `void (State* L, int idx)` |

### 3.3 入栈函数

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `pushnil` | `lua_pushnil` | `void (State* L)` |
| `pushnumber` | `lua_pushnumber` | `void (State* L, Number n)` |
| `pushinteger` | `lua_pushinteger` | `void (State* L, Integer n)` |
| `pushlstring` | `lua_pushlstring` | `const char* (State* L, const char* s, ull len)` |
| `pushstring` | `lua_pushstring` | `const char* (State* L, const char* s)` |
| `pushcclosure` | `lua_pushcclosure` | `void (State* L, CFunction fn, int n)` |
| `pushcfunction` | `lua_pushcfunction` (宏) | `void (State* L, CFunction f)` |
| `pushboolean` | `lua_pushboolean` | `void (State* L, int b)` |
| `pushlightuserdata` | `lua_pushlightuserdata` | `void (State* L, void* p)` |
| `pushthread` | `lua_pushthread` | `int (State* L)` |
| `pushglobaltable` | `lua_pushglobaltable` (宏) | `void (State* L)` |
| `pushfstring` | `lua_pushfstring` | 多个重载 |

### 3.4 类型检查

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `type` | `lua_type` | `int (State* L, int idx)` |
| `type_name` | `lua_typename` | `const char* (State* L, int tp)` |
| `isnumber` | `lua_isnumber` | `int (State* L, int idx)` |
| `isstring` | `lua_isstring` | `int (State* L, int idx)` |
| `iscfunction` | `lua_iscfunction` | `int (State* L, int idx)` |
| `isinteger` | `lua_isinteger` | `int (State* L, int idx)` |
| `isuserdata` | `lua_isuserdata` | `int (State* L, int idx)` |
| `isfunction` | `lua_isfunction` (宏) | `int (State* L, int idx)` |
| `istable` | `lua_istable` (宏) | `int (State* L, int idx)` |
| `islightuserdata` | `lua_islightuserdata` (宏) | `int (State* L, int idx)` |
| `isnil` | `lua_isnil` (宏) | `int (State* L, int idx)` |
| `isboolean` | `lua_isboolean` (宏) | `int (State* L, int idx)` |
| `isthread` | `lua_isthread` (宏) | `int (State* L, int idx)` |
| `isnone` | `lua_isnone` (宏) | `int (State* L, int idx)` |
| `isnoneornil` | `lua_isnoneornil` (宏) | `int (State* L, int idx)` |

### 3.5 取值函数

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `tonumberx` | `lua_tonumberx` | `Number (State* L, int idx, int* isnum)` |
| `tointegerx` | `lua_tointegerx` | `Integer (State* L, int idx, int* isnum)` |
| `tonumber` | `lua_tonumber` (宏) | `Number (State* L, int idx)` |
| `tointeger` | `lua_tointeger` (宏) | `Integer (State* L, int idx)` |
| `toboolean` | `lua_toboolean` | `int (State* L, int idx)` |
| `tolstring` | `lua_tolstring` | `const char* (State* L, int idx, ull* len)` |
| `tostring` | `lua_tostring` (宏) | `const char* (State* L, int idx)` |
| `tocfunction` | `lua_tocfunction` | `CFunction (State* L, int idx)` |
| `touserdata` | `lua_touserdata` | `void* (State* L, int idx)` |
| `tothread` | `lua_tothread` | `State* (State* L, int idx)` |
| `topointer` | `lua_topointer` | `const void* (State* L, int idx)` |
| `rawlen` | `lua_rawlen` | `Unsigned (State* L, int idx)` |

### 3.6 算术与比较

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `arith` | `lua_arith` | `void (State* L, int op)` |
| `rawequal` | `lua_rawequal` | `int (State* L, int idx1, int idx2)` |
| `compare` | `lua_compare` | `int (State* L, int idx1, int idx2, int op)` |

### 3.7 表操作（读取）

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `getglobal` | `lua_getglobal` | `int (State* L, const char* name)` |
| `gettable` | `lua_gettable` | `int (State* L, int idx)` |
| `getfield` | `lua_getfield` | `int (State* L, int idx, const char* k)` |
| `geti` | `lua_geti` | `int (State* L, int idx, Integer n)` |
| `rawget` | `lua_rawget` | `int (State* L, int idx)` |
| `rawgeti` | `lua_rawgeti` | `int (State* L, int idx, Integer n)` |
| `rawgetp` | `lua_rawgetp` | `int (State* L, int idx, const void* p)` |
| `createtable` | `lua_createtable` | `void (State* L, int narr, int nrec)` |
| `newtable` | `lua_newtable` (宏) | `void (State* L)` |
| `newuserdatauv` | `lua_newuserdatauv` | `void* (State* L, ull sz, int nuvalue)` |
| `newuserdata` | `lua_newuserdata` (宏) | `void* (State* L, ull sz)` |
| `getmetatable` | `lua_getmetatable` | `int (State* L, int objindex)` |
| `getiuservalue` | `lua_getiuservalue` | `int (State* L, int idx, int n)` |

### 3.8 表操作（写入）

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `setglobal` | `lua_setglobal` | `void (State* L, const char* name)` |
| `settable` | `lua_settable` | `void (State* L, int idx)` |
| `setfield` | `lua_setfield` | `void (State* L, int idx, const char* k)` |
| `seti` | `lua_seti` | `void (State* L, int idx, Integer n)` |
| `rawset` | `lua_rawset` | `void (State* L, int idx)` |
| `rawseti` | `lua_rawseti` | `void (State* L, int idx, Integer n)` |
| `rawsetp` | `lua_rawsetp` | `void (State* L, int idx, const void* p)` |
| `setmetatable` | `lua_setmetatable` | `int (State* L, int objindex)` |
| `setiuservalue` | `lua_setiuservalue` | `int (State* L, int idx, int n)` |

### 3.9 调用

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `callk` | `lua_callk` | `void (State* L, int nargs, int nresults, KContext ctx, KFunction k)` |
| `call` | `lua_call` (宏) | `void (State* L, int nargs, int nresults)` |
| `pcallk` | `lua_pcallk` | `int (State* L, int nargs, int nresults, int errfunc, KContext ctx, KFunction k)` |
| `pcall` | `lua_pcall` (宏) | `int (State* L, int nargs, int nresults, int errfunc)` |
| `load` | `lua_load` | `int (State* L, Reader reader, void* dt, const char* chunkname, const char* mode)` |
| `dump` | `lua_dump` | `int (State* L, Writer writer, void* data, int strip)` |

### 3.10 协程

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `yieldk` | `lua_yieldk` | `int (State* L, int nresults, KContext ctx, KFunction k)` |
| `resume` | `lua_resume` | `int (State* L, State* from, int narg, int* nres)` |
| `status` | `lua_status` | `int (State* L)` |
| `isyieldable` | `lua_isyieldable` | `int (State* L)` |

### 3.11 GC

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `gc` | `lua_gc` | `int (State* L, int what)` |
| `gc` | `lua_gc` | `int (State* L, int what, int data)` |
| `gc` | `lua_gc` | `int (State* L, int what, int data, int data2)` |

### 3.12 其他

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `error` | `lua_error` | `int (State* L)` |
| `next` | `lua_next` | `int (State* L, int idx)` |
| `concat` | `lua_concat` | `void (State* L, int n)` |
| `len` | `lua_len` | `void (State* L, int idx)` |
| `stringtonumber` | `lua_stringtonumber` | `ull (State* L, const char* s)` |
| `getallocf` | `lua_getallocf` | `Alloc (State* L, void** ud)` |
| `setallocf` | `lua_setallocf` | `void (State* L, Alloc f, void* ud)` |
| `toclose` | `lua_toclose` | `void (State* L, int idx)` |
| `closeslot` | `lua_closeslot` | `void (State* L, int idx)` |
| `getextraspace` | `lua_getextraspace` (宏) | `void* (State* L)` |
| `setwarnf` | `lua_setwarnf` | `void (State* L, WarnFunction f, void* ud)` |
| `warning` | `lua_warning` | `void (State* L, const char* msg, int tocont)` |
| `register_func` | `lua_register` (宏) | `void (State* L, const char* name, CFunction f)` |
| `upvalueindex` | `lua_upvalueindex` (宏) | `int (int i)` |

### 3.13 调试接口

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `getstack` | `lua_getstack` | `int (State* L, int level, Debug* ar)` |
| `getinfo` | `lua_getinfo` | `int (State* L, const char* what, Debug* ar)` |
| `getlocal` | `lua_getlocal` | `const char* (State* L, const Debug* ar, int n)` |
| `setlocal` | `lua_setlocal` | `const char* (State* L, const Debug* ar, int n)` |
| `getupvalue` | `lua_getupvalue` | `const char* (State* L, int funcindex, int n)` |
| `setupvalue` | `lua_setupvalue` | `const char* (State* L, int funcindex, int n)` |
| `upvalueid` | `lua_upvalueid` | `void* (State* L, int fidx, int n)` |
| `upvaluejoin` | `lua_upvaluejoin` | `void (State* L, int fidx1, int n1, int fidx2, int n2)` |
| `sethook` | `lua_sethook` | `void (State* L, Hook func, int mask, int count)` |
| `gethook` | `lua_gethook` | `Hook (State* L)` |
| `gethookmask` | `lua_gethookmask` | `int (State* L)` |
| `gethookcount` | `lua_gethookcount` | `int (State* L)` |

### 3.14 辅助库 (luaL_*)

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `L_newstate` | `luaL_newstate` | `State* ()` |
| `L_openlibs` | `luaL_openlibs` | `void (State* L)` |
| `L_loadstring` | `luaL_loadstring` | `int (State* L, const char* s)` |
| `L_loadfilex` | `luaL_loadfilex` | `int (State* L, const char* filename, const char* mode)` |
| `L_loadfile` | `luaL_loadfile` (宏) | `int (State* L, const char* filename)` |
| `L_loadbufferx` | `luaL_loadbufferx` | `int (State* L, const char* buff, ull sz, const char* name, const char* mode)` |
| `L_dostring` | `luaL_dostring` (宏) | `int (State* L, const char* s)` |
| `L_dofile` | `luaL_dofile` (宏) | `int (State* L, const char* filename)` |
| `L_getmetafield` | `luaL_getmetafield` | `int (State* L, int obj, const char* e)` |
| `L_callmeta` | `luaL_callmeta` | `int (State* L, int obj, const char* e)` |
| `L_tolstring` | `luaL_tolstring` | `const char* (State* L, int idx, ull* len)` |
| `L_argerror` | `luaL_argerror` | `int (State* L, int arg, const char* extramsg)` |
| `L_typeerror` | `luaL_typeerror` | `int (State* L, int arg, const char* tname)` |
| `L_checklstring` | `luaL_checklstring` | `const char* (State* L, int arg, ull* l)` |
| `L_optlstring` | `luaL_optlstring` | `const char* (State* L, int arg, const char* def, ull* l)` |
| `L_checknumber` | `luaL_checknumber` | `Number (State* L, int arg)` |
| `L_optnumber` | `luaL_optnumber` | `Number (State* L, int arg, Number def)` |
| `L_checkinteger` | `luaL_checkinteger` | `Integer (State* L, int arg)` |
| `L_optinteger` | `luaL_optinteger` | `Integer (State* L, int arg, Integer def)` |
| `L_checkstack` | `luaL_checkstack` | `void (State* L, int sz, const char* msg)` |
| `L_checktype` | `luaL_checktype` | `void (State* L, int arg, int t)` |
| `L_checkany` | `luaL_checkany` | `void (State* L, int arg)` |
| `L_newmetatable` | `luaL_newmetatable` | `int (State* L, const char* tname)` |
| `L_setmetatable` | `luaL_setmetatable` | `void (State* L, const char* tname)` |
| `L_testudata` | `luaL_testudata` | `void* (State* L, int ud, const char* tname)` |
| `L_checkudata` | `luaL_checkudata` | `void* (State* L, int ud, const char* tname)` |
| `L_where` | `luaL_where` | `void (State* L, int lvl)` |
| `L_checkoption` | `luaL_checkoption` | `int (State* L, int arg, const char* def, const char* const lst[])` |
| `L_ref` | `luaL_ref` | `int (State* L, int t)` |
| `L_unref` | `luaL_unref` | `void (State* L, int t, int ref)` |
| `L_len` | `luaL_len` | `Integer (State* L, int idx)` |
| `L_gsub` | `luaL_gsub` | `const char* (State* L, const char* s, const char* p, const char* r)` |
| `L_setfuncs` | `luaL_setfuncs` | `void (State* L, const L_Reg* l, int nup)` |
| `L_getsubtable` | `luaL_getsubtable` | `int (State* L, int idx, const char* fname)` |
| `L_traceback` | `luaL_traceback` | `void (State* L, State* L1, const char* msg, int level)` |
| `L_requiref` | `luaL_requiref` | `void (State* L, const char* modname, CFunction openf, int glb)` |
| `L_typename` | `luaL_typename` (宏) | `const char* (State* L, int idx)` |
| `L_checkstring` | `luaL_checkstring` (宏) | `const char* (State* L, int arg)` |
| `L_optstring` | `luaL_optstring` (宏) | `const char* (State* L, int arg, const char* def)` |
| `L_getmetatable` | `luaL_getmetatable` (宏) | `int (State* L, const char* n)` |
| `L_checkversion` | `luaL_checkversion` (宏) | `void (State* L)` |
| `L_error` | `luaL_error` | `int (State* L, const char* fmt)` |
| `L_error` | `luaL_error` | `int (State* L, const char* fmt, const char* arg1)` |

### 3.15 Buffer 操作

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `L_buffinit` | `luaL_buffinit` | `void (State* L, L_Buffer* B)` |
| `L_prepbuffsize` | `luaL_prepbuffsize` | `char* (L_Buffer* B, ull sz)` |
| `L_addlstring` | `luaL_addlstring` | `void (L_Buffer* B, const char* s, ull l)` |
| `L_addstring` | `luaL_addstring` | `void (L_Buffer* B, const char* s)` |
| `L_addvalue` | `luaL_addvalue` | `void (L_Buffer* B)` |
| `L_pushresult` | `luaL_pushresult` | `void (L_Buffer* B)` |
| `L_pushresultsize` | `luaL_pushresultsize` | `void (L_Buffer* B, ull sz)` |
| `L_buffinitsize` | `luaL_buffinitsize` | `char* (State* L, L_Buffer* B, ull sz)` |

### 3.16 标准库

| 函数 | C API 原名 | 签名 |
|---|---|---|
| `open_base` | `luaopen_base` | `int (State* L)` |
| `open_coroutine` | `luaopen_coroutine` | `int (State* L)` |
| `open_table` | `luaopen_table` | `int (State* L)` |
| `open_io` | `luaopen_io` | `int (State* L)` |
| `open_os` | `luaopen_os` | `int (State* L)` |
| `open_string` | `luaopen_string` | `int (State* L)` |
| `open_utf8` | `luaopen_utf8` | `int (State* L)` |
| `open_math` | `luaopen_math` | `int (State* L)` |
| `open_debug` | `luaopen_debug` | `int (State* L)` |
| `open_package` | `luaopen_package` | `int (State* L)` |

---

## 四、命名映射速查表

| C API 前缀 | 模块内规则 | 示例 |
|---|---|---|
| `lua_` → 去掉 | `lua_gettop` → `gettop` |  |
| `luaL_` → `L_` | `luaL_newstate` → `L_newstate` |  |
| `LUA_` → 去掉 | `LUA_OK` → `OK` |  |
| `luaopen_` → `open_` | `luaopen_base` → `open_base` |  |
| `lua_typename` → `type_name` | 避免与 `type()` 冲突 |  |
| `lua_register` → `register_func` | 避免关键字冲突 |  |
| `lua_upvalueindex` → `upvalueindex` | 去 `lua_` |  |

> `ull` = `unsigned long long`
