module;

#include "lua_headers.h"

export module mcpplibs.capi.lua;

export namespace mcpplibs::capi::lua {

// ============================================================================
// Types
// ============================================================================

using State        = ::lua_State;
using Number       = ::lua_Number;
using Integer      = ::lua_Integer;
using Unsigned     = ::lua_Unsigned;
using CFunction    = ::lua_CFunction;
using KFunction    = ::lua_KFunction;
using KContext     = ::lua_KContext;
using Alloc        = ::lua_Alloc;
using Reader       = ::lua_Reader;
using Writer       = ::lua_Writer;
using WarnFunction = ::lua_WarnFunction;
using Hook         = ::lua_Hook;
using Debug        = ::lua_Debug;
using L_Reg        = ::luaL_Reg;
using L_Buffer     = ::luaL_Buffer;

// ============================================================================
// Constants — Status Codes
// ============================================================================

inline constexpr int OK        = LUA_OK;
inline constexpr int YIELD     = LUA_YIELD;
inline constexpr int ERRRUN    = LUA_ERRRUN;
inline constexpr int ERRSYNTAX = LUA_ERRSYNTAX;
inline constexpr int ERRMEM    = LUA_ERRMEM;
inline constexpr int ERRERR    = LUA_ERRERR;
inline constexpr int ERRFILE   = LUA_ERRFILE;

// ============================================================================
// Constants — Type Tags
// ============================================================================

inline constexpr int TNONE          = LUA_TNONE;
inline constexpr int TNIL           = LUA_TNIL;
inline constexpr int TBOOLEAN       = LUA_TBOOLEAN;
inline constexpr int TLIGHTUSERDATA = LUA_TLIGHTUSERDATA;
inline constexpr int TNUMBER        = LUA_TNUMBER;
inline constexpr int TSTRING        = LUA_TSTRING;
inline constexpr int TTABLE         = LUA_TTABLE;
inline constexpr int TFUNCTION      = LUA_TFUNCTION;
inline constexpr int TUSERDATA      = LUA_TUSERDATA;
inline constexpr int TTHREAD        = LUA_TTHREAD;
inline constexpr int NUMTYPES       = LUA_NUMTYPES;

// ============================================================================
// Constants — Arithmetic / Comparison Operators
// ============================================================================

inline constexpr int OPADD  = LUA_OPADD;
inline constexpr int OPSUB  = LUA_OPSUB;
inline constexpr int OPMUL  = LUA_OPMUL;
inline constexpr int OPMOD  = LUA_OPMOD;
inline constexpr int OPPOW  = LUA_OPPOW;
inline constexpr int OPDIV  = LUA_OPDIV;
inline constexpr int OPIDIV = LUA_OPIDIV;
inline constexpr int OPBAND = LUA_OPBAND;
inline constexpr int OPBOR  = LUA_OPBOR;
inline constexpr int OPBXOR = LUA_OPBXOR;
inline constexpr int OPSHL  = LUA_OPSHL;
inline constexpr int OPSHR  = LUA_OPSHR;
inline constexpr int OPUNM  = LUA_OPUNM;
inline constexpr int OPBNOT = LUA_OPBNOT;

inline constexpr int OPEQ = LUA_OPEQ;
inline constexpr int OPLT = LUA_OPLT;
inline constexpr int OPLE = LUA_OPLE;

// ============================================================================
// Constants — Misc
// ============================================================================

inline constexpr int MULTRET         = LUA_MULTRET;
inline constexpr int REGISTRYINDEX   = LUA_REGISTRYINDEX;
inline constexpr int MINSTACK        = LUA_MINSTACK;
inline constexpr int RIDX_MAINTHREAD = LUA_RIDX_MAINTHREAD;
inline constexpr int RIDX_GLOBALS    = LUA_RIDX_GLOBALS;
inline constexpr int NOREF           = LUA_NOREF;
inline constexpr int REFNIL          = LUA_REFNIL;

// ============================================================================
// Constants — GC Options
// ============================================================================

inline constexpr int GCSTOP       = LUA_GCSTOP;
inline constexpr int GCRESTART    = LUA_GCRESTART;
inline constexpr int GCCOLLECT    = LUA_GCCOLLECT;
inline constexpr int GCCOUNT      = LUA_GCCOUNT;
inline constexpr int GCCOUNTB     = LUA_GCCOUNTB;
inline constexpr int GCSTEP       = LUA_GCSTEP;
inline constexpr int GCSETPAUSE   = LUA_GCSETPAUSE;
inline constexpr int GCSETSTEPMUL = LUA_GCSETSTEPMUL;
inline constexpr int GCISRUNNING  = LUA_GCISRUNNING;
inline constexpr int GCGEN        = LUA_GCGEN;
inline constexpr int GCINC        = LUA_GCINC;

// ============================================================================
// Constants — Hook Masks
// ============================================================================

inline constexpr int MASKCALL  = LUA_MASKCALL;
inline constexpr int MASKRET   = LUA_MASKRET;
inline constexpr int MASKLINE  = LUA_MASKLINE;
inline constexpr int MASKCOUNT = LUA_MASKCOUNT;

inline constexpr int HOOKCALL     = LUA_HOOKCALL;
inline constexpr int HOOKRET      = LUA_HOOKRET;
inline constexpr int HOOKLINE     = LUA_HOOKLINE;
inline constexpr int HOOKCOUNT    = LUA_HOOKCOUNT;
inline constexpr int HOOKTAILCALL = LUA_HOOKTAILCALL;

// ============================================================================
// Function Declarations
// ============================================================================

// State Management
State* newstate(Alloc f, void* ud);
void close(State* L);
State* newthread(State* L);
CFunction atpanic(State* L, CFunction panicf);
Number version(State* L);

// Stack Operations
int absindex(State* L, int idx);
int gettop(State* L);
void settop(State* L, int idx);
void pop(State* L, int n);
void pushvalue(State* L, int idx);
void rotate(State* L, int idx, int n);
void copy(State* L, int fromidx, int toidx);
int checkstack(State* L, int n);
void xmove(State* from, State* to, int n);
void insert(State* L, int idx);
void remove(State* L, int idx);
void replace(State* L, int idx);

// Push Functions (C -> Stack)
void pushnil(State* L);
void pushnumber(State* L, Number n);
void pushinteger(State* L, Integer n);
const char* pushlstring(State* L, const char* s, unsigned long long len);
const char* pushstring(State* L, const char* s);
void pushcclosure(State* L, CFunction fn, int n);
void pushcfunction(State* L, CFunction f);
void pushboolean(State* L, int b);
void pushlightuserdata(State* L, void* p);
int pushthread(State* L);
void pushglobaltable(State* L);
const char* pushfstring(State* L, const char* fmt);
const char* pushfstring(State* L, const char* fmt, const char* arg1);
const char* pushfstring(State* L, const char* fmt, const char* arg1, int arg2);
const char* pushfstring(State* L, const char* fmt, int arg1);

// Type Check Functions
int type(State* L, int idx);
const char* type_name(State* L, int tp);
int isnumber(State* L, int idx);
int isstring(State* L, int idx);
int iscfunction(State* L, int idx);
int isinteger(State* L, int idx);
int isuserdata(State* L, int idx);
int isfunction(State* L, int idx);
int istable(State* L, int idx);
int islightuserdata(State* L, int idx);
int isnil(State* L, int idx);
int isboolean(State* L, int idx);
int isthread(State* L, int idx);
int isnone(State* L, int idx);
int isnoneornil(State* L, int idx);

// Access Functions (Stack -> C)
Number tonumberx(State* L, int idx, int* isnum);
Integer tointegerx(State* L, int idx, int* isnum);
Number tonumber(State* L, int idx);
Integer tointeger(State* L, int idx);
int toboolean(State* L, int idx);
const char* tolstring(State* L, int idx, unsigned long long* len);
const char* tostring(State* L, int idx);
CFunction tocfunction(State* L, int idx);
void* touserdata(State* L, int idx);
State* tothread(State* L, int idx);
const void* topointer(State* L, int idx);
Unsigned rawlen(State* L, int idx);

// Arithmetic and Comparison
void arith(State* L, int op);
int rawequal(State* L, int idx1, int idx2);
int compare(State* L, int idx1, int idx2, int op);

// Table Access (Get)
int getglobal(State* L, const char* name);
int gettable(State* L, int idx);
int getfield(State* L, int idx, const char* k);
int geti(State* L, int idx, Integer n);
int rawget(State* L, int idx);
int rawgeti(State* L, int idx, Integer n);
int rawgetp(State* L, int idx, const void* p);
void createtable(State* L, int narr, int nrec);
void newtable(State* L);
void* newuserdatauv(State* L, unsigned long long sz, int nuvalue);
void* newuserdata(State* L, unsigned long long sz);
int getmetatable(State* L, int objindex);
int getiuservalue(State* L, int idx, int n);

// Table Access (Set)
void setglobal(State* L, const char* name);
void settable(State* L, int idx);
void setfield(State* L, int idx, const char* k);
void seti(State* L, int idx, Integer n);
void rawset(State* L, int idx);
void rawseti(State* L, int idx, Integer n);
void rawsetp(State* L, int idx, const void* p);
int setmetatable(State* L, int objindex);
int setiuservalue(State* L, int idx, int n);

// Call / Load / Execute
void callk(State* L, int nargs, int nresults, KContext ctx, KFunction k);
void call(State* L, int nargs, int nresults);
int pcallk(State* L, int nargs, int nresults, int errfunc, KContext ctx, KFunction k);
int pcall(State* L, int nargs, int nresults, int errfunc);
int load(State* L, Reader reader, void* dt, const char* chunkname, const char* mode);
int dump(State* L, Writer writer, void* data, int strip);

// Coroutine
int yieldk(State* L, int nresults, KContext ctx, KFunction k);
int resume(State* L, State* from, int narg, int* nres);
int status(State* L);
int isyieldable(State* L);

// GC
int gc(State* L, int what);
int gc(State* L, int what, int data);
int gc(State* L, int what, int data, int data2);

// Misc
int error(State* L);
int next(State* L, int idx);
void concat(State* L, int n);
void len(State* L, int idx);
unsigned long long stringtonumber(State* L, const char* s);
Alloc getallocf(State* L, void** ud);
void setallocf(State* L, Alloc f, void* ud);
void toclose(State* L, int idx);
void closeslot(State* L, int idx);
void* getextraspace(State* L);
void setwarnf(State* L, WarnFunction f, void* ud);
void warning(State* L, const char* msg, int tocont);
void register_func(State* L, const char* name, CFunction f);
int upvalueindex(int i);

// Debug Interface
int getstack(State* L, int level, Debug* ar);
int getinfo(State* L, const char* what, Debug* ar);
const char* getlocal(State* L, const Debug* ar, int n);
const char* setlocal(State* L, const Debug* ar, int n);
const char* getupvalue(State* L, int funcindex, int n);
const char* setupvalue(State* L, int funcindex, int n);
void* upvalueid(State* L, int fidx, int n);
void upvaluejoin(State* L, int fidx1, int n1, int fidx2, int n2);
void sethook(State* L, Hook func, int mask, int count);
Hook gethook(State* L);
int gethookmask(State* L);
int gethookcount(State* L);

// Auxiliary Library (luaL_*)
State* L_newstate();
void L_openlibs(State* L);
int L_loadstring(State* L, const char* s);
int L_loadfilex(State* L, const char* filename, const char* mode);
int L_loadfile(State* L, const char* filename);
int L_loadbufferx(State* L, const char* buff, unsigned long long sz, const char* name, const char* mode);
int L_dostring(State* L, const char* s);
int L_dofile(State* L, const char* filename);
int L_getmetafield(State* L, int obj, const char* e);
int L_callmeta(State* L, int obj, const char* e);
const char* L_tolstring(State* L, int idx, unsigned long long* len);
int L_argerror(State* L, int arg, const char* extramsg);
int L_typeerror(State* L, int arg, const char* tname);
const char* L_checklstring(State* L, int arg, unsigned long long* l);
const char* L_optlstring(State* L, int arg, const char* def, unsigned long long* l);
Number L_checknumber(State* L, int arg);
Number L_optnumber(State* L, int arg, Number def);
Integer L_checkinteger(State* L, int arg);
Integer L_optinteger(State* L, int arg, Integer def);
void L_checkstack(State* L, int sz, const char* msg);
void L_checktype(State* L, int arg, int t);
void L_checkany(State* L, int arg);
int L_newmetatable(State* L, const char* tname);
void L_setmetatable(State* L, const char* tname);
void* L_testudata(State* L, int ud, const char* tname);
void* L_checkudata(State* L, int ud, const char* tname);
void L_where(State* L, int lvl);
int L_checkoption(State* L, int arg, const char* def, const char* const lst[]);
int L_ref(State* L, int t);
void L_unref(State* L, int t, int ref);
Integer L_len(State* L, int idx);
const char* L_gsub(State* L, const char* s, const char* p, const char* r);
void L_setfuncs(State* L, const L_Reg* l, int nup);
int L_getsubtable(State* L, int idx, const char* fname);
void L_traceback(State* L, State* L1, const char* msg, int level);
void L_requiref(State* L, const char* modname, CFunction openf, int glb);
const char* L_typename(State* L, int idx);
const char* L_checkstring(State* L, int arg);
const char* L_optstring(State* L, int arg, const char* def);
int L_getmetatable(State* L, const char* n);
void L_checkversion(State* L);
int L_error(State* L, const char* fmt);
int L_error(State* L, const char* fmt, const char* arg1);

// Buffer Operations
void L_buffinit(State* L, L_Buffer* B);
char* L_prepbuffsize(L_Buffer* B, unsigned long long sz);
void L_addlstring(L_Buffer* B, const char* s, unsigned long long l);
void L_addstring(L_Buffer* B, const char* s);
void L_addvalue(L_Buffer* B);
void L_pushresult(L_Buffer* B);
void L_pushresultsize(L_Buffer* B, unsigned long long sz);
char* L_buffinitsize(State* L, L_Buffer* B, unsigned long long sz);

// Standard Libraries (luaopen_*)
int open_base(State* L);
int open_coroutine(State* L);
int open_table(State* L);
int open_io(State* L);
int open_os(State* L);
int open_string(State* L);
int open_utf8(State* L);
int open_math(State* L);
int open_debug(State* L);
int open_package(State* L);

} // namespace mcpplibs::capi::lua
