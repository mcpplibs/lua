module;

#include "lua_headers.h"

module mcpplibs.capi.lua;

namespace mcpplibs::capi::lua {

// ============================================================================
// State Management
// ============================================================================

State* newstate(Alloc f, void* ud) {
#if LUA_VERSION_NUM >= 505
    return ::lua_newstate(f, ud, 0);
#else
    return ::lua_newstate(f, ud);
#endif
}
void close(State* L) { ::lua_close(L); }
State* newthread(State* L) { return ::lua_newthread(L); }
CFunction atpanic(State* L, CFunction panicf) { return ::lua_atpanic(L, panicf); }
Number version(State* L) { return ::lua_version(L); }

// ============================================================================
// Stack Operations
// ============================================================================

int absindex(State* L, int idx) { return ::lua_absindex(L, idx); }
int gettop(State* L) { return ::lua_gettop(L); }
void settop(State* L, int idx) { ::lua_settop(L, idx); }
void pop(State* L, int n) { ::lua_settop(L, -(n) - 1); }
void pushvalue(State* L, int idx) { ::lua_pushvalue(L, idx); }
void rotate(State* L, int idx, int n) { ::lua_rotate(L, idx, n); }
void copy(State* L, int fromidx, int toidx) { ::lua_copy(L, fromidx, toidx); }
int checkstack(State* L, int n) { return ::lua_checkstack(L, n); }
void xmove(State* from, State* to, int n) { ::lua_xmove(from, to, n); }

void insert(State* L, int idx) {
    ::lua_rotate(L, (idx), 1);
}

void remove(State* L, int idx) {
    ::lua_rotate(L, (idx), -1);
    ::lua_settop(L, -2);
}

void replace(State* L, int idx) {
    ::lua_copy(L, -1, (idx));
    ::lua_settop(L, -2);
}

// ============================================================================
// Push Functions (C -> Stack)
// ============================================================================

void pushnil(State* L) { ::lua_pushnil(L); }
void pushnumber(State* L, Number n) { ::lua_pushnumber(L, n); }
void pushinteger(State* L, Integer n) { ::lua_pushinteger(L, n); }

const char* pushlstring(State* L, const char* s, unsigned long long len) {
    return ::lua_pushlstring(L, s, static_cast<size_t>(len));
}

const char* pushstring(State* L, const char* s) { return ::lua_pushstring(L, s); }
void pushcclosure(State* L, CFunction fn, int n) { ::lua_pushcclosure(L, fn, n); }
void pushcfunction(State* L, CFunction f) { ::lua_pushcclosure(L, (f), 0); }
void pushboolean(State* L, int b) { ::lua_pushboolean(L, b); }
void pushlightuserdata(State* L, void* p) { ::lua_pushlightuserdata(L, p); }
int pushthread(State* L) { return ::lua_pushthread(L); }

void pushglobaltable(State* L) {
    ::lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
}

const char* pushfstring(State* L, const char* fmt) {
    return ::lua_pushfstring(L, "%s", fmt);
}

const char* pushfstring(State* L, const char* fmt, const char* arg1) {
    return ::lua_pushfstring(L, fmt, arg1);
}

const char* pushfstring(State* L, const char* fmt, const char* arg1, int arg2) {
    return ::lua_pushfstring(L, fmt, arg1, arg2);
}

const char* pushfstring(State* L, const char* fmt, int arg1) {
    return ::lua_pushfstring(L, fmt, arg1);
}

// ============================================================================
// Type Check Functions
// ============================================================================

int type(State* L, int idx) { return ::lua_type(L, idx); }
const char* type_name(State* L, int tp) { return ::lua_typename(L, tp); }
int isnumber(State* L, int idx) { return ::lua_isnumber(L, idx); }
int isstring(State* L, int idx) { return ::lua_isstring(L, idx); }
int iscfunction(State* L, int idx) { return ::lua_iscfunction(L, idx); }
int isinteger(State* L, int idx) { return ::lua_isinteger(L, idx); }
int isuserdata(State* L, int idx) { return ::lua_isuserdata(L, idx); }
int isfunction(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TFUNCTION; }
int istable(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TTABLE; }
int islightuserdata(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TLIGHTUSERDATA; }
int isnil(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TNIL; }
int isboolean(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TBOOLEAN; }
int isthread(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TTHREAD; }
int isnone(State* L, int idx) { return ::lua_type(L, (idx)) == LUA_TNONE; }
int isnoneornil(State* L, int idx) { return ::lua_type(L, (idx)) <= 0; }

// ============================================================================
// Access Functions (Stack -> C)
// ============================================================================

Number tonumberx(State* L, int idx, int* isnum) { return ::lua_tonumberx(L, idx, isnum); }
Integer tointegerx(State* L, int idx, int* isnum) { return ::lua_tointegerx(L, idx, isnum); }
Number tonumber(State* L, int idx) { return ::lua_tonumberx(L, (idx), nullptr); }
Integer tointeger(State* L, int idx) { return ::lua_tointegerx(L, (idx), nullptr); }
int toboolean(State* L, int idx) { return ::lua_toboolean(L, idx); }

const char* tolstring(State* L, int idx, unsigned long long* len) {
    return ::lua_tolstring(L, idx, reinterpret_cast<size_t*>(len));
}

const char* tostring(State* L, int idx) { return ::lua_tolstring(L, (idx), nullptr); }
CFunction tocfunction(State* L, int idx) { return ::lua_tocfunction(L, idx); }
void* touserdata(State* L, int idx) { return ::lua_touserdata(L, idx); }
State* tothread(State* L, int idx) { return ::lua_tothread(L, idx); }
const void* topointer(State* L, int idx) { return ::lua_topointer(L, idx); }
Unsigned rawlen(State* L, int idx) { return ::lua_rawlen(L, idx); }

// ============================================================================
// Arithmetic and Comparison
// ============================================================================

void arith(State* L, int op) { ::lua_arith(L, op); }
int rawequal(State* L, int idx1, int idx2) { return ::lua_rawequal(L, idx1, idx2); }
int compare(State* L, int idx1, int idx2, int op) { return ::lua_compare(L, idx1, idx2, op); }

// ============================================================================
// Table Access (Get)
// ============================================================================

int getglobal(State* L, const char* name) { return ::lua_getglobal(L, name); }
int gettable(State* L, int idx) { return ::lua_gettable(L, idx); }
int getfield(State* L, int idx, const char* k) { return ::lua_getfield(L, idx, k); }
int geti(State* L, int idx, Integer n) { return ::lua_geti(L, idx, n); }
int rawget(State* L, int idx) { return ::lua_rawget(L, idx); }
int rawgeti(State* L, int idx, Integer n) { return ::lua_rawgeti(L, idx, n); }
int rawgetp(State* L, int idx, const void* p) { return ::lua_rawgetp(L, idx, p); }
void createtable(State* L, int narr, int nrec) { ::lua_createtable(L, narr, nrec); }
void newtable(State* L) { ::lua_createtable(L, 0, 0); }

void* newuserdatauv(State* L, unsigned long long sz, int nuvalue) {
    return ::lua_newuserdatauv(L, static_cast<size_t>(sz), nuvalue);
}

void* newuserdata(State* L, unsigned long long sz) {
    return ::lua_newuserdatauv(L, static_cast<size_t>(sz), 1);
}

int getmetatable(State* L, int objindex) { return ::lua_getmetatable(L, objindex); }
int getiuservalue(State* L, int idx, int n) { return ::lua_getiuservalue(L, idx, n); }

// ============================================================================
// Table Access (Set)
// ============================================================================

void setglobal(State* L, const char* name) { ::lua_setglobal(L, name); }
void settable(State* L, int idx) { ::lua_settable(L, idx); }
void setfield(State* L, int idx, const char* k) { ::lua_setfield(L, idx, k); }
void seti(State* L, int idx, Integer n) { ::lua_seti(L, idx, n); }
void rawset(State* L, int idx) { ::lua_rawset(L, idx); }
void rawseti(State* L, int idx, Integer n) { ::lua_rawseti(L, idx, n); }
void rawsetp(State* L, int idx, const void* p) { ::lua_rawsetp(L, idx, p); }
int setmetatable(State* L, int objindex) { return ::lua_setmetatable(L, objindex); }
int setiuservalue(State* L, int idx, int n) { return ::lua_setiuservalue(L, idx, n); }

// ============================================================================
// Call / Load / Execute
// ============================================================================

void callk(State* L, int nargs, int nresults, KContext ctx, KFunction k) {
    ::lua_callk(L, nargs, nresults, ctx, k);
}

void call(State* L, int nargs, int nresults) {
    ::lua_callk(L, (nargs), (nresults), 0, nullptr);
}

int pcallk(State* L, int nargs, int nresults, int errfunc, KContext ctx, KFunction k) {
    return ::lua_pcallk(L, nargs, nresults, errfunc, ctx, k);
}

int pcall(State* L, int nargs, int nresults, int errfunc) {
    return ::lua_pcallk(L, (nargs), (nresults), (errfunc), 0, nullptr);
}

int load(State* L, Reader reader, void* dt, const char* chunkname, const char* mode) {
    return ::lua_load(L, reader, dt, chunkname, mode);
}

int dump(State* L, Writer writer, void* data, int strip) {
    return ::lua_dump(L, writer, data, strip);
}

// ============================================================================
// Coroutine
// ============================================================================

int yieldk(State* L, int nresults, KContext ctx, KFunction k) {
    return ::lua_yieldk(L, nresults, ctx, k);
}

int resume(State* L, State* from, int narg, int* nres) {
    return ::lua_resume(L, from, narg, nres);
}

int status(State* L) { return ::lua_status(L); }
int isyieldable(State* L) { return ::lua_isyieldable(L); }

// ============================================================================
// GC
// ============================================================================

int gc(State* L, int what) { return ::lua_gc(L, what, 0); }
int gc(State* L, int what, int data) { return ::lua_gc(L, what, data); }
int gc(State* L, int what, int data, int data2) { return ::lua_gc(L, what, data, data2); }

// ============================================================================
// Misc
// ============================================================================

int error(State* L) { return ::lua_error(L); }
int next(State* L, int idx) { return ::lua_next(L, idx); }
void concat(State* L, int n) { ::lua_concat(L, n); }
void len(State* L, int idx) { ::lua_len(L, idx); }

unsigned long long stringtonumber(State* L, const char* s) {
    return static_cast<unsigned long long>(::lua_stringtonumber(L, s));
}

Alloc getallocf(State* L, void** ud) { return ::lua_getallocf(L, ud); }
void setallocf(State* L, Alloc f, void* ud) { ::lua_setallocf(L, f, ud); }
void toclose(State* L, int idx) { ::lua_toclose(L, idx); }
void closeslot(State* L, int idx) { ::lua_closeslot(L, idx); }
void* getextraspace(State* L) { return lua_getextraspace(L); }
void setwarnf(State* L, WarnFunction f, void* ud) { ::lua_setwarnf(L, f, ud); }
void warning(State* L, const char* msg, int tocont) { ::lua_warning(L, msg, tocont); }

void register_func(State* L, const char* name, CFunction f) {
    ::lua_pushcclosure(L, (f), 0);
    ::lua_setglobal(L, (name));
}

int upvalueindex(int i) { return lua_upvalueindex(i); }

// ============================================================================
// Debug Interface
// ============================================================================

int getstack(State* L, int level, Debug* ar) { return ::lua_getstack(L, level, ar); }
int getinfo(State* L, const char* what, Debug* ar) { return ::lua_getinfo(L, what, ar); }
const char* getlocal(State* L, const Debug* ar, int n) { return ::lua_getlocal(L, ar, n); }
const char* setlocal(State* L, const Debug* ar, int n) { return ::lua_setlocal(L, ar, n); }
const char* getupvalue(State* L, int funcindex, int n) { return ::lua_getupvalue(L, funcindex, n); }
const char* setupvalue(State* L, int funcindex, int n) { return ::lua_setupvalue(L, funcindex, n); }
void* upvalueid(State* L, int fidx, int n) { return ::lua_upvalueid(L, fidx, n); }

void upvaluejoin(State* L, int fidx1, int n1, int fidx2, int n2) {
    ::lua_upvaluejoin(L, fidx1, n1, fidx2, n2);
}

void sethook(State* L, Hook func, int mask, int count) { ::lua_sethook(L, func, mask, count); }
Hook gethook(State* L) { return ::lua_gethook(L); }
int gethookmask(State* L) { return ::lua_gethookmask(L); }
int gethookcount(State* L) { return ::lua_gethookcount(L); }

// ============================================================================
// Auxiliary Library (luaL_*)
// ============================================================================

State* L_newstate() { return ::luaL_newstate(); }
void L_openlibs(State* L) { ::luaL_openlibs(L); }
int L_loadstring(State* L, const char* s) { return ::luaL_loadstring(L, s); }

int L_loadfilex(State* L, const char* filename, const char* mode) {
    return ::luaL_loadfilex(L, filename, mode);
}

int L_loadfile(State* L, const char* filename) {
    return ::luaL_loadfilex(L, (filename), nullptr);
}

int L_loadbufferx(State* L, const char* buff, unsigned long long sz,
                  const char* name, const char* mode) {
    return ::luaL_loadbufferx(L, buff, static_cast<size_t>(sz), name, mode);
}

int L_dostring(State* L, const char* s) {
    return ::luaL_loadstring(L, s) || ::lua_pcallk(L, 0, LUA_MULTRET, 0, 0, nullptr);
}

int L_dofile(State* L, const char* filename) {
    return ::luaL_loadfilex(L, filename, nullptr) || ::lua_pcallk(L, 0, LUA_MULTRET, 0, 0, nullptr);
}

int L_getmetafield(State* L, int obj, const char* e) { return ::luaL_getmetafield(L, obj, e); }
int L_callmeta(State* L, int obj, const char* e) { return ::luaL_callmeta(L, obj, e); }

const char* L_tolstring(State* L, int idx, unsigned long long* len) {
    return ::luaL_tolstring(L, idx, reinterpret_cast<size_t*>(len));
}

int L_argerror(State* L, int arg, const char* extramsg) { return ::luaL_argerror(L, arg, extramsg); }
int L_typeerror(State* L, int arg, const char* tname) { return ::luaL_typeerror(L, arg, tname); }

const char* L_checklstring(State* L, int arg, unsigned long long* l) {
    return ::luaL_checklstring(L, arg, reinterpret_cast<size_t*>(l));
}

const char* L_optlstring(State* L, int arg, const char* def, unsigned long long* l) {
    return ::luaL_optlstring(L, arg, def, reinterpret_cast<size_t*>(l));
}

Number L_checknumber(State* L, int arg) { return ::luaL_checknumber(L, arg); }
Number L_optnumber(State* L, int arg, Number def) { return ::luaL_optnumber(L, arg, def); }
Integer L_checkinteger(State* L, int arg) { return ::luaL_checkinteger(L, arg); }
Integer L_optinteger(State* L, int arg, Integer def) { return ::luaL_optinteger(L, arg, def); }
void L_checkstack(State* L, int sz, const char* msg) { ::luaL_checkstack(L, sz, msg); }
void L_checktype(State* L, int arg, int t) { ::luaL_checktype(L, arg, t); }
void L_checkany(State* L, int arg) { ::luaL_checkany(L, arg); }
int L_newmetatable(State* L, const char* tname) { return ::luaL_newmetatable(L, tname); }
void L_setmetatable(State* L, const char* tname) { ::luaL_setmetatable(L, tname); }
void* L_testudata(State* L, int ud, const char* tname) { return ::luaL_testudata(L, ud, tname); }
void* L_checkudata(State* L, int ud, const char* tname) { return ::luaL_checkudata(L, ud, tname); }
void L_where(State* L, int lvl) { ::luaL_where(L, lvl); }

int L_checkoption(State* L, int arg, const char* def, const char* const lst[]) {
    return ::luaL_checkoption(L, arg, def, lst);
}

int L_ref(State* L, int t) { return ::luaL_ref(L, t); }
void L_unref(State* L, int t, int ref) { ::luaL_unref(L, t, ref); }
Integer L_len(State* L, int idx) { return ::luaL_len(L, idx); }

const char* L_gsub(State* L, const char* s, const char* p, const char* r) {
    return ::luaL_gsub(L, s, p, r);
}

void L_setfuncs(State* L, const L_Reg* l, int nup) { ::luaL_setfuncs(L, l, nup); }

int L_getsubtable(State* L, int idx, const char* fname) {
    return ::luaL_getsubtable(L, idx, fname);
}

void L_traceback(State* L, State* L1, const char* msg, int level) {
    ::luaL_traceback(L, L1, msg, level);
}

void L_requiref(State* L, const char* modname, CFunction openf, int glb) {
    ::luaL_requiref(L, modname, openf, glb);
}

const char* L_typename(State* L, int idx) {
    return ::lua_typename(L, ::lua_type(L, (idx)));
}

const char* L_checkstring(State* L, int arg) {
    return ::luaL_checklstring(L, (arg), nullptr);
}

const char* L_optstring(State* L, int arg, const char* def) {
    return ::luaL_optlstring(L, (arg), (def), nullptr);
}

int L_getmetatable(State* L, const char* n) {
    return ::lua_getfield(L, LUA_REGISTRYINDEX, (n));
}

void L_checkversion(State* L) { ::luaL_checkversion(L); }

int L_error(State* L, const char* fmt) { return ::luaL_error(L, "%s", fmt); }
int L_error(State* L, const char* fmt, const char* arg1) { return ::luaL_error(L, fmt, arg1); }

// ============================================================================
// Buffer Operations
// ============================================================================

void L_buffinit(State* L, L_Buffer* B) { ::luaL_buffinit(L, B); }

char* L_prepbuffsize(L_Buffer* B, unsigned long long sz) {
    return ::luaL_prepbuffsize(B, static_cast<size_t>(sz));
}

void L_addlstring(L_Buffer* B, const char* s, unsigned long long l) {
    ::luaL_addlstring(B, s, static_cast<size_t>(l));
}

void L_addstring(L_Buffer* B, const char* s) { ::luaL_addstring(B, s); }
void L_addvalue(L_Buffer* B) { ::luaL_addvalue(B); }
void L_pushresult(L_Buffer* B) { ::luaL_pushresult(B); }

void L_pushresultsize(L_Buffer* B, unsigned long long sz) {
    ::luaL_pushresultsize(B, static_cast<size_t>(sz));
}

char* L_buffinitsize(State* L, L_Buffer* B, unsigned long long sz) {
    return ::luaL_buffinitsize(L, B, static_cast<size_t>(sz));
}

// ============================================================================
// Standard Libraries (luaopen_*)
// ============================================================================

int open_base(State* L) { return ::luaopen_base(L); }
int open_coroutine(State* L) { return ::luaopen_coroutine(L); }
int open_table(State* L) { return ::luaopen_table(L); }
int open_io(State* L) { return ::luaopen_io(L); }
int open_os(State* L) { return ::luaopen_os(L); }
int open_string(State* L) { return ::luaopen_string(L); }
int open_utf8(State* L) { return ::luaopen_utf8(L); }
int open_math(State* L) { return ::luaopen_math(L); }
int open_debug(State* L) { return ::luaopen_debug(L); }
int open_package(State* L) { return ::luaopen_package(L); }

} // namespace mcpplibs::capi::lua
