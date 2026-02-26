#include <gtest/gtest.h>

import mcpplibs.capi.lua;

namespace lua = mcpplibs::capi::lua;

// ============================================================================
// State Management
// ============================================================================

TEST(StateTest, CreateAndClose) {
    auto* L = lua::L_newstate();
    ASSERT_NE(L, nullptr);
    lua::close(L);
}

TEST(StateTest, OpenLibs) {
    auto* L = lua::L_newstate();
    ASSERT_NE(L, nullptr);
    lua::L_openlibs(L);
    lua::close(L);
}

TEST(StateTest, Version) {
    auto* L = lua::L_newstate();
    auto ver = lua::version(L);
    EXPECT_GE(ver, 504.0);
    lua::close(L);
}

TEST(StateTest, CheckVersion) {
    auto* L = lua::L_newstate();
    lua::L_checkversion(L);
    lua::close(L);
}

// ============================================================================
// Constants
// ============================================================================

TEST(ConstantsTest, StatusCodes) {
    EXPECT_EQ(lua::OK, 0);
    EXPECT_NE(lua::ERRRUN, 0);
    EXPECT_NE(lua::ERRSYNTAX, 0);
    EXPECT_NE(lua::ERRMEM, 0);
    EXPECT_NE(lua::ERRERR, 0);
}

TEST(ConstantsTest, TypeTags) {
    EXPECT_EQ(lua::TNONE, -1);
    EXPECT_EQ(lua::TNIL, 0);
    EXPECT_NE(lua::TBOOLEAN, lua::TNIL);
    EXPECT_NE(lua::TNUMBER, lua::TSTRING);
    EXPECT_NE(lua::TTABLE, lua::TFUNCTION);
}

TEST(ConstantsTest, ArithOps) {
    EXPECT_NE(lua::OPADD, lua::OPSUB);
    EXPECT_NE(lua::OPMUL, lua::OPDIV);
}

TEST(ConstantsTest, CompareOps) {
    EXPECT_NE(lua::OPEQ, lua::OPLT);
    EXPECT_NE(lua::OPLT, lua::OPLE);
}

TEST(ConstantsTest, MiscConstants) {
    EXPECT_EQ(lua::MULTRET, -1);
    EXPECT_NE(lua::REGISTRYINDEX, 0);
    EXPECT_GT(lua::MINSTACK, 0);
}

TEST(ConstantsTest, GCOptions) {
    EXPECT_NE(lua::GCSTOP, lua::GCRESTART);
    EXPECT_NE(lua::GCCOLLECT, lua::GCCOUNT);
}

// ============================================================================
// Stack Operations
// ============================================================================

TEST(StackTest, GetTopEmpty) {
    auto* L = lua::L_newstate();
    EXPECT_EQ(lua::gettop(L), 0);
    lua::close(L);
}

TEST(StackTest, PushAndGetTop) {
    auto* L = lua::L_newstate();
    lua::pushnil(L);
    EXPECT_EQ(lua::gettop(L), 1);
    lua::pushnumber(L, 3.14);
    EXPECT_EQ(lua::gettop(L), 2);
    lua::pushinteger(L, 42);
    EXPECT_EQ(lua::gettop(L), 3);
    lua::close(L);
}

TEST(StackTest, SetTop) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 1);
    lua::pushinteger(L, 2);
    lua::pushinteger(L, 3);
    EXPECT_EQ(lua::gettop(L), 3);
    lua::settop(L, 1);
    EXPECT_EQ(lua::gettop(L), 1);
    lua::close(L);
}

TEST(StackTest, Pop) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 1);
    lua::pushinteger(L, 2);
    lua::pushinteger(L, 3);
    lua::pop(L, 2);
    EXPECT_EQ(lua::gettop(L), 1);
    lua::close(L);
}

TEST(StackTest, PushValue) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 42);
    lua::pushvalue(L, -1);
    EXPECT_EQ(lua::gettop(L), 2);
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    EXPECT_EQ(lua::tointeger(L, -2), 42);
    lua::close(L);
}

TEST(StackTest, AbsIndex) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 10);
    lua::pushinteger(L, 20);
    EXPECT_EQ(lua::absindex(L, -1), 2);
    EXPECT_EQ(lua::absindex(L, -2), 1);
    EXPECT_EQ(lua::absindex(L, 1), 1);
    lua::close(L);
}

TEST(StackTest, CheckStack) {
    auto* L = lua::L_newstate();
    EXPECT_NE(lua::checkstack(L, 100), 0);
    lua::close(L);
}

TEST(StackTest, Rotate) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 1);
    lua::pushinteger(L, 2);
    lua::pushinteger(L, 3);
    lua::rotate(L, 1, 1);
    EXPECT_EQ(lua::tointeger(L, 1), 3);
    EXPECT_EQ(lua::tointeger(L, 2), 1);
    EXPECT_EQ(lua::tointeger(L, 3), 2);
    lua::close(L);
}

TEST(StackTest, Copy) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 10);
    lua::pushinteger(L, 20);
    lua::copy(L, 2, 1);
    EXPECT_EQ(lua::tointeger(L, 1), 20);
    lua::close(L);
}

TEST(StackTest, Insert) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 1);
    lua::pushinteger(L, 2);
    lua::pushinteger(L, 3);
    lua::insert(L, 1);
    EXPECT_EQ(lua::tointeger(L, 1), 3);
    EXPECT_EQ(lua::tointeger(L, 2), 1);
    EXPECT_EQ(lua::tointeger(L, 3), 2);
    lua::close(L);
}

TEST(StackTest, Remove) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 1);
    lua::pushinteger(L, 2);
    lua::pushinteger(L, 3);
    lua::remove(L, 2);
    EXPECT_EQ(lua::gettop(L), 2);
    EXPECT_EQ(lua::tointeger(L, 1), 1);
    EXPECT_EQ(lua::tointeger(L, 2), 3);
    lua::close(L);
}

TEST(StackTest, Replace) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 1);
    lua::pushinteger(L, 2);
    lua::pushinteger(L, 99);
    lua::replace(L, 1);
    EXPECT_EQ(lua::gettop(L), 2);
    EXPECT_EQ(lua::tointeger(L, 1), 99);
    lua::close(L);
}

// ============================================================================
// Push and Type Check
// ============================================================================

TEST(PushTest, PushNil) {
    auto* L = lua::L_newstate();
    lua::pushnil(L);
    EXPECT_NE(lua::isnil(L, -1), 0);
    EXPECT_EQ(lua::type(L, -1), lua::TNIL);
    lua::close(L);
}

TEST(PushTest, PushNumber) {
    auto* L = lua::L_newstate();
    lua::pushnumber(L, 3.14);
    EXPECT_NE(lua::isnumber(L, -1), 0);
    EXPECT_EQ(lua::type(L, -1), lua::TNUMBER);
    EXPECT_DOUBLE_EQ(lua::tonumber(L, -1), 3.14);
    lua::close(L);
}

TEST(PushTest, PushInteger) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 42);
    EXPECT_NE(lua::isinteger(L, -1), 0);
    EXPECT_NE(lua::isnumber(L, -1), 0);
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    lua::close(L);
}

TEST(PushTest, PushString) {
    auto* L = lua::L_newstate();
    lua::pushstring(L, "hello");
    EXPECT_NE(lua::isstring(L, -1), 0);
    EXPECT_EQ(lua::type(L, -1), lua::TSTRING);
    EXPECT_STREQ(lua::tostring(L, -1), "hello");
    lua::close(L);
}

TEST(PushTest, PushLString) {
    auto* L = lua::L_newstate();
    lua::pushlstring(L, "hello world", 5);
    EXPECT_STREQ(lua::tostring(L, -1), "hello");
    lua::close(L);
}

TEST(PushTest, PushBoolean) {
    auto* L = lua::L_newstate();
    lua::pushboolean(L, 1);
    EXPECT_NE(lua::isboolean(L, -1), 0);
    EXPECT_EQ(lua::type(L, -1), lua::TBOOLEAN);
    EXPECT_EQ(lua::toboolean(L, -1), 1);

    lua::pushboolean(L, 0);
    EXPECT_EQ(lua::toboolean(L, -1), 0);
    lua::close(L);
}

TEST(PushTest, PushLightUserdata) {
    auto* L = lua::L_newstate();
    int dummy { 0 };
    lua::pushlightuserdata(L, &dummy);
    EXPECT_NE(lua::islightuserdata(L, -1), 0);
    EXPECT_EQ(lua::touserdata(L, -1), &dummy);
    lua::close(L);
}

TEST(PushTest, PushCFunction) {
    auto* L = lua::L_newstate();
    lua::CFunction fn = [](lua::State*) -> int { return 0; };
    lua::pushcfunction(L, fn);
    EXPECT_NE(lua::iscfunction(L, -1), 0);
    EXPECT_NE(lua::isfunction(L, -1), 0);
    lua::close(L);
}

TEST(PushTest, PushFString) {
    auto* L = lua::L_newstate();
    const char* result = lua::pushfstring(L, "hello %s %d", "world", 42);
    EXPECT_STREQ(result, "hello world 42");
    EXPECT_STREQ(lua::tostring(L, -1), "hello world 42");
    lua::close(L);
}

// ============================================================================
// Type Names
// ============================================================================

TEST(TypeNameTest, AllTypes) {
    auto* L = lua::L_newstate();
    EXPECT_STREQ(lua::type_name(L, lua::TNIL), "nil");
    EXPECT_STREQ(lua::type_name(L, lua::TBOOLEAN), "boolean");
    EXPECT_STREQ(lua::type_name(L, lua::TNUMBER), "number");
    EXPECT_STREQ(lua::type_name(L, lua::TSTRING), "string");
    EXPECT_STREQ(lua::type_name(L, lua::TTABLE), "table");
    EXPECT_STREQ(lua::type_name(L, lua::TFUNCTION), "function");
    EXPECT_STREQ(lua::type_name(L, lua::TUSERDATA), "userdata");
    EXPECT_STREQ(lua::type_name(L, lua::TTHREAD), "thread");
    lua::close(L);
}

TEST(TypeNameTest, LTypeName) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 42);
    EXPECT_STREQ(lua::L_typename(L, -1), "number");
    lua::pushstring(L, "test");
    EXPECT_STREQ(lua::L_typename(L, -1), "string");
    lua::close(L);
}

// ============================================================================
// Access Functions
// ============================================================================

TEST(AccessTest, ToNumberX) {
    auto* L = lua::L_newstate();
    lua::pushnumber(L, 2.718);
    int isnum { 0 };
    auto val = lua::tonumberx(L, -1, &isnum);
    EXPECT_NE(isnum, 0);
    EXPECT_DOUBLE_EQ(val, 2.718);
    lua::close(L);
}

TEST(AccessTest, ToIntegerX) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 100);
    int isnum { 0 };
    auto val = lua::tointegerx(L, -1, &isnum);
    EXPECT_NE(isnum, 0);
    EXPECT_EQ(val, 100);
    lua::close(L);
}

TEST(AccessTest, ToLString) {
    auto* L = lua::L_newstate();
    lua::pushstring(L, "test");
    unsigned long long len { 0 };
    auto* s = lua::tolstring(L, -1, &len);
    EXPECT_STREQ(s, "test");
    EXPECT_EQ(len, 4u);
    lua::close(L);
}

TEST(AccessTest, RawLen) {
    auto* L = lua::L_newstate();
    lua::pushstring(L, "hello");
    EXPECT_EQ(lua::rawlen(L, -1), 5u);
    lua::close(L);
}

TEST(AccessTest, ToPointer) {
    auto* L = lua::L_newstate();
    lua::newtable(L);
    auto* p = lua::topointer(L, -1);
    EXPECT_NE(p, nullptr);
    lua::close(L);
}

// ============================================================================
// Isnone / Isnoneornil
// ============================================================================

TEST(TypeCheckTest, IsNone) {
    auto* L = lua::L_newstate();
    EXPECT_NE(lua::isnone(L, 1), 0);
    lua::pushinteger(L, 1);
    EXPECT_EQ(lua::isnone(L, 1), 0);
    lua::close(L);
}

TEST(TypeCheckTest, IsNoneOrNil) {
    auto* L = lua::L_newstate();
    EXPECT_NE(lua::isnoneornil(L, 1), 0);
    lua::pushnil(L);
    EXPECT_NE(lua::isnoneornil(L, 1), 0);
    lua::pushinteger(L, 1);
    EXPECT_EQ(lua::isnoneornil(L, 2), 0);
    lua::close(L);
}

// ============================================================================
// Table Operations
// ============================================================================

TEST(TableTest, CreateAndSetField) {
    auto* L = lua::L_newstate();
    lua::newtable(L);
    lua::pushinteger(L, 42);
    lua::setfield(L, -2, "x");

    lua::getfield(L, -1, "x");
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    lua::close(L);
}

TEST(TableTest, CreateTablePreAlloc) {
    auto* L = lua::L_newstate();
    lua::createtable(L, 3, 2);
    EXPECT_NE(lua::istable(L, -1), 0);
    lua::close(L);
}

TEST(TableTest, RawSetAndGet) {
    auto* L = lua::L_newstate();
    lua::newtable(L);

    lua::pushstring(L, "key");
    lua::pushstring(L, "value");
    lua::rawset(L, -3);

    lua::pushstring(L, "key");
    lua::rawget(L, -2);
    EXPECT_STREQ(lua::tostring(L, -1), "value");
    lua::close(L);
}

TEST(TableTest, RawSetIAndGetI) {
    auto* L = lua::L_newstate();
    lua::newtable(L);

    lua::pushstring(L, "first");
    lua::rawseti(L, -2, 1);
    lua::pushstring(L, "second");
    lua::rawseti(L, -2, 2);

    lua::rawgeti(L, -1, 1);
    EXPECT_STREQ(lua::tostring(L, -1), "first");
    lua::pop(L, 1);

    lua::rawgeti(L, -1, 2);
    EXPECT_STREQ(lua::tostring(L, -1), "second");
    lua::close(L);
}

TEST(TableTest, SetIAndGetI) {
    auto* L = lua::L_newstate();
    lua::newtable(L);

    lua::pushinteger(L, 100);
    lua::seti(L, -2, 1);

    lua::geti(L, -1, 1);
    EXPECT_EQ(lua::tointeger(L, -1), 100);
    lua::close(L);
}

TEST(TableTest, SetAndGetTable) {
    auto* L = lua::L_newstate();
    lua::newtable(L);

    lua::pushstring(L, "name");
    lua::pushstring(L, "mcpp");
    lua::settable(L, -3);

    lua::pushstring(L, "name");
    lua::gettable(L, -2);
    EXPECT_STREQ(lua::tostring(L, -1), "mcpp");
    lua::close(L);
}

TEST(TableTest, Next) {
    auto* L = lua::L_newstate();
    lua::newtable(L);
    lua::pushinteger(L, 10);
    lua::setfield(L, -2, "a");
    lua::pushinteger(L, 20);
    lua::setfield(L, -2, "b");

    int count { 0 };
    lua::pushnil(L);
    while (lua::next(L, -2) != 0) {
        count++;
        lua::pop(L, 1);
    }
    EXPECT_EQ(count, 2);
    lua::close(L);
}

TEST(TableTest, RawLen) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::L_dostring(L, "t = {10, 20, 30}");
    lua::getglobal(L, "t");
    EXPECT_EQ(lua::rawlen(L, -1), 3u);
    lua::close(L);
}

TEST(TableTest, Metatable) {
    auto* L = lua::L_newstate();
    lua::newtable(L);
    lua::newtable(L);
    lua::setmetatable(L, -2);
    EXPECT_NE(lua::getmetatable(L, -1), 0);
    lua::close(L);
}

// ============================================================================
// Global Variables
// ============================================================================

TEST(GlobalTest, SetAndGet) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 123);
    lua::setglobal(L, "myvar");

    lua::getglobal(L, "myvar");
    EXPECT_EQ(lua::tointeger(L, -1), 123);
    lua::close(L);
}

TEST(GlobalTest, PushGlobalTable) {
    auto* L = lua::L_newstate();
    lua::pushglobaltable(L);
    EXPECT_NE(lua::istable(L, -1), 0);
    lua::close(L);
}

// ============================================================================
// Lua Script Execution
// ============================================================================

TEST(ExecTest, DoString) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    int result = lua::L_dostring(L, "x = 1 + 2");
    EXPECT_EQ(result, lua::OK);
    lua::getglobal(L, "x");
    EXPECT_EQ(lua::tointeger(L, -1), 3);
    lua::close(L);
}

TEST(ExecTest, DoStringSyntaxError) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    int result = lua::L_dostring(L, "this is not valid lua!");
    EXPECT_NE(result, lua::OK);
    lua::close(L);
}

TEST(ExecTest, DoStringReturnValue) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::L_dostring(L, "function add(a, b) return a + b end");
    lua::getglobal(L, "add");
    lua::pushinteger(L, 10);
    lua::pushinteger(L, 20);
    int result = lua::pcall(L, 2, 1, 0);
    EXPECT_EQ(result, lua::OK);
    EXPECT_EQ(lua::tointeger(L, -1), 30);
    lua::close(L);
}

TEST(ExecTest, LoadString) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    int result = lua::L_loadstring(L, "return 42");
    EXPECT_EQ(result, lua::OK);
    EXPECT_NE(lua::isfunction(L, -1), 0);
    result = lua::pcall(L, 0, 1, 0);
    EXPECT_EQ(result, lua::OK);
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    lua::close(L);
}

// ============================================================================
// Function Calls
// ============================================================================

TEST(CallTest, PCallSuccess) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::L_dostring(L, "function greet(name) return 'hello ' .. name end");
    lua::getglobal(L, "greet");
    lua::pushstring(L, "mcpp");
    int result = lua::pcall(L, 1, 1, 0);
    EXPECT_EQ(result, lua::OK);
    EXPECT_STREQ(lua::tostring(L, -1), "hello mcpp");
    lua::close(L);
}

TEST(CallTest, PCallError) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::L_dostring(L, "function bad() error('oops') end");
    lua::getglobal(L, "bad");
    int result = lua::pcall(L, 0, 0, 0);
    EXPECT_EQ(result, lua::ERRRUN);
    lua::close(L);
}

TEST(CallTest, CFunctionCallback) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    lua::CFunction add_func = [](lua::State* L) -> int {
        auto a = lua::tointeger(L, 1);
        auto b = lua::tointeger(L, 2);
        lua::pushinteger(L, a + b);
        return 1;
    };

    lua::register_func(L, "c_add", add_func);
    lua::L_dostring(L, "result = c_add(10, 32)");
    lua::getglobal(L, "result");
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    lua::close(L);
}

TEST(CallTest, PushCClosure) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    lua::pushinteger(L, 100);
    lua::CFunction closure_fn = [](lua::State* L) -> int {
        auto upval = lua::tointeger(L, lua::upvalueindex(1));
        auto arg = lua::tointeger(L, 1);
        lua::pushinteger(L, upval + arg);
        return 1;
    };
    lua::pushcclosure(L, closure_fn, 1);
    lua::setglobal(L, "add_100");

    lua::L_dostring(L, "result = add_100(5)");
    lua::getglobal(L, "result");
    EXPECT_EQ(lua::tointeger(L, -1), 105);
    lua::close(L);
}

// ============================================================================
// Arithmetic and Comparison
// ============================================================================

TEST(ArithTest, Add) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 10);
    lua::pushinteger(L, 20);
    lua::arith(L, lua::OPADD);
    EXPECT_EQ(lua::tointeger(L, -1), 30);
    lua::close(L);
}

TEST(ArithTest, Mul) {
    auto* L = lua::L_newstate();
    lua::pushnumber(L, 3.0);
    lua::pushnumber(L, 4.0);
    lua::arith(L, lua::OPMUL);
    EXPECT_DOUBLE_EQ(lua::tonumber(L, -1), 12.0);
    lua::close(L);
}

TEST(ArithTest, Unm) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 42);
    lua::arith(L, lua::OPUNM);
    EXPECT_EQ(lua::tointeger(L, -1), -42);
    lua::close(L);
}

TEST(CompareTest, RawEqual) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 42);
    lua::pushinteger(L, 42);
    EXPECT_NE(lua::rawequal(L, -1, -2), 0);

    lua::pushinteger(L, 43);
    EXPECT_EQ(lua::rawequal(L, -1, -2), 0);
    lua::close(L);
}

TEST(CompareTest, Compare) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 10);
    lua::pushinteger(L, 20);
    EXPECT_NE(lua::compare(L, -2, -1, lua::OPLT), 0);
    EXPECT_EQ(lua::compare(L, -1, -2, lua::OPLT), 0);
    EXPECT_NE(lua::compare(L, -2, -1, lua::OPLE), 0);
    lua::close(L);
}

// ============================================================================
// String Operations
// ============================================================================

TEST(StringTest, Concat) {
    auto* L = lua::L_newstate();
    lua::pushstring(L, "hello");
    lua::pushstring(L, " ");
    lua::pushstring(L, "world");
    lua::concat(L, 3);
    EXPECT_STREQ(lua::tostring(L, -1), "hello world");
    lua::close(L);
}

TEST(StringTest, StringToNumber) {
    auto* L = lua::L_newstate();
    auto len = lua::stringtonumber(L, "42");
    EXPECT_GT(len, 0u);
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    lua::close(L);
}

TEST(StringTest, Len) {
    auto* L = lua::L_newstate();
    lua::pushstring(L, "hello");
    lua::len(L, -1);
    EXPECT_EQ(lua::tointeger(L, -1), 5);
    lua::close(L);
}

// ============================================================================
// Auxiliary Library
// ============================================================================

TEST(AuxTest, Ref) {
    auto* L = lua::L_newstate();
    lua::pushstring(L, "stored");
    int ref = lua::L_ref(L, lua::REGISTRYINDEX);
    EXPECT_NE(ref, lua::NOREF);
    EXPECT_NE(ref, lua::REFNIL);

    lua::rawgeti(L, lua::REGISTRYINDEX, ref);
    EXPECT_STREQ(lua::tostring(L, -1), "stored");

    lua::L_unref(L, lua::REGISTRYINDEX, ref);
    lua::close(L);
}

TEST(AuxTest, NewMetatable) {
    auto* L = lua::L_newstate();
    int created = lua::L_newmetatable(L, "MyType");
    EXPECT_NE(created, 0);
    lua::pop(L, 1);

    created = lua::L_newmetatable(L, "MyType");
    EXPECT_EQ(created, 0);
    lua::close(L);
}

TEST(AuxTest, LLen) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::L_dostring(L, "t = {1, 2, 3, 4, 5}");
    lua::getglobal(L, "t");
    EXPECT_EQ(lua::L_len(L, -1), 5);
    lua::close(L);
}

TEST(AuxTest, GSub) {
    auto* L = lua::L_newstate();
    auto* result = lua::L_gsub(L, "hello world", "world", "mcpp");
    EXPECT_STREQ(result, "hello mcpp");
    EXPECT_STREQ(lua::tostring(L, -1), "hello mcpp");
    lua::close(L);
}

TEST(AuxTest, GetSubtable) {
    auto* L = lua::L_newstate();
    lua::pushglobaltable(L);
    int existed = lua::L_getsubtable(L, -1, "mymodule");
    EXPECT_EQ(existed, 0);
    EXPECT_NE(lua::istable(L, -1), 0);

    existed = lua::L_getsubtable(L, -2, "mymodule");
    EXPECT_NE(existed, 0);
    lua::close(L);
}

TEST(AuxTest, Traceback) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::L_traceback(L, L, "test traceback", 0);
    EXPECT_NE(lua::isstring(L, -1), 0);
    lua::close(L);
}

TEST(AuxTest, Where) {
    auto* L = lua::L_newstate();
    lua::L_where(L, 0);
    EXPECT_NE(lua::isstring(L, -1), 0);
    lua::close(L);
}

TEST(AuxTest, LTolstring) {
    auto* L = lua::L_newstate();
    lua::pushinteger(L, 42);
    unsigned long long len { 0 };
    auto* s = lua::L_tolstring(L, -1, &len);
    EXPECT_STREQ(s, "42");
    EXPECT_GT(len, 0u);
    lua::close(L);
}

// ============================================================================
// Buffer Operations
// ============================================================================

TEST(BufferTest, BasicBuffer) {
    auto* L = lua::L_newstate();
    lua::L_Buffer buf;
    lua::L_buffinit(L, &buf);
    lua::L_addstring(&buf, "hello");
    lua::L_addstring(&buf, " ");
    lua::L_addstring(&buf, "world");
    lua::L_pushresult(&buf);
    EXPECT_STREQ(lua::tostring(L, -1), "hello world");
    lua::close(L);
}

TEST(BufferTest, AddLString) {
    auto* L = lua::L_newstate();
    lua::L_Buffer buf;
    lua::L_buffinit(L, &buf);
    lua::L_addlstring(&buf, "hello world", 5);
    lua::L_pushresult(&buf);
    EXPECT_STREQ(lua::tostring(L, -1), "hello");
    lua::close(L);
}

// ============================================================================
// GC
// ============================================================================

TEST(GCTest, CollectAndCount) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);
    lua::gc(L, lua::GCCOLLECT);
    auto count = lua::gc(L, lua::GCCOUNT);
    EXPECT_GE(count, 0);
    lua::close(L);
}

TEST(GCTest, IsRunning) {
    auto* L = lua::L_newstate();
    auto running = lua::gc(L, lua::GCISRUNNING);
    EXPECT_NE(running, 0);
    lua::close(L);
}

// ============================================================================
// Thread
// ============================================================================

TEST(ThreadTest, NewThread) {
    auto* L = lua::L_newstate();
    auto* T = lua::newthread(L);
    EXPECT_NE(T, nullptr);
    EXPECT_NE(lua::isthread(L, -1), 0);
    lua::close(L);
}

TEST(ThreadTest, PushThread) {
    auto* L = lua::L_newstate();
    int isMain = lua::pushthread(L);
    EXPECT_NE(isMain, 0);
    lua::close(L);
}

TEST(ThreadTest, Status) {
    auto* L = lua::L_newstate();
    EXPECT_EQ(lua::status(L), lua::OK);
    lua::close(L);
}

TEST(ThreadTest, XMove) {
    auto* L = lua::L_newstate();
    auto* T = lua::newthread(L);
    lua::pushinteger(L, 42);
    lua::xmove(L, T, 1);
    EXPECT_EQ(lua::tointeger(T, -1), 42);
    lua::close(L);
}

// ============================================================================
// Userdata
// ============================================================================

TEST(UserdataTest, NewUserdata) {
    auto* L = lua::L_newstate();
    auto* ptr = lua::newuserdata(L, sizeof(int));
    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(lua::isuserdata(L, -1), 0);
    *static_cast<int*>(ptr) = 42;
    EXPECT_EQ(*static_cast<int*>(lua::touserdata(L, -1)), 42);
    lua::close(L);
}

TEST(UserdataTest, NewUserdataUV) {
    auto* L = lua::L_newstate();
    auto* ptr = lua::newuserdatauv(L, sizeof(double), 2);
    EXPECT_NE(ptr, nullptr);
    lua::close(L);
}

// ============================================================================
// Standard Libraries
// ============================================================================

TEST(StdLibTest, OpenBase) {
    auto* L = lua::L_newstate();
    lua::L_requiref(L, "_G", lua::open_base, 1);
    lua::pop(L, 1);
    lua::L_dostring(L, "x = type(42)");
    lua::getglobal(L, "x");
    EXPECT_STREQ(lua::tostring(L, -1), "number");
    lua::close(L);
}

TEST(StdLibTest, OpenMath) {
    auto* L = lua::L_newstate();
    lua::L_requiref(L, "_G", lua::open_base, 1);
    lua::pop(L, 1);
    lua::L_requiref(L, "math", lua::open_math, 1);
    lua::pop(L, 1);
    lua::L_dostring(L, "x = math.floor(3.7)");
    lua::getglobal(L, "x");
    EXPECT_EQ(lua::tointeger(L, -1), 3);
    lua::close(L);
}

TEST(StdLibTest, OpenString) {
    auto* L = lua::L_newstate();
    lua::L_requiref(L, "_G", lua::open_base, 1);
    lua::pop(L, 1);
    lua::L_requiref(L, "string", lua::open_string, 1);
    lua::pop(L, 1);
    lua::L_dostring(L, "x = string.upper('hello')");
    lua::getglobal(L, "x");
    EXPECT_STREQ(lua::tostring(L, -1), "HELLO");
    lua::close(L);
}

TEST(StdLibTest, OpenTable) {
    auto* L = lua::L_newstate();
    lua::L_requiref(L, "_G", lua::open_base, 1);
    lua::pop(L, 1);
    lua::L_requiref(L, "table", lua::open_table, 1);
    lua::pop(L, 1);
    lua::L_dostring(L, "t = {3, 1, 2}; table.sort(t); x = t[1]");
    lua::getglobal(L, "x");
    EXPECT_EQ(lua::tointeger(L, -1), 1);
    lua::close(L);
}

// ============================================================================
// Complex Scenarios
// ============================================================================

TEST(ScenarioTest, LuaCalculator) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    lua::L_dostring(L,
        "function calc(op, a, b)          \n"
        "  if op == '+' then return a + b  \n"
        "  elseif op == '-' then return a - b \n"
        "  elseif op == '*' then return a * b \n"
        "  elseif op == '/' then return a / b \n"
        "  end                             \n"
        "end                               \n"
    );

    lua::getglobal(L, "calc");
    lua::pushstring(L, "+");
    lua::pushnumber(L, 10.5);
    lua::pushnumber(L, 20.5);
    EXPECT_EQ(lua::pcall(L, 3, 1, 0), lua::OK);
    EXPECT_DOUBLE_EQ(lua::tonumber(L, -1), 31.0);
    lua::pop(L, 1);

    lua::getglobal(L, "calc");
    lua::pushstring(L, "*");
    lua::pushinteger(L, 6);
    lua::pushinteger(L, 7);
    EXPECT_EQ(lua::pcall(L, 3, 1, 0), lua::OK);
    EXPECT_EQ(lua::tointeger(L, -1), 42);

    lua::close(L);
}

TEST(ScenarioTest, NestedTables) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    lua::newtable(L);
    lua::newtable(L);
    lua::pushinteger(L, 42);
    lua::setfield(L, -2, "value");
    lua::setfield(L, -2, "inner");
    lua::setglobal(L, "outer");

    lua::L_dostring(L, "result = outer.inner.value");
    lua::getglobal(L, "result");
    EXPECT_EQ(lua::tointeger(L, -1), 42);
    lua::close(L);
}

TEST(ScenarioTest, SetFuncs) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    static const lua::L_Reg mylib[] = {
        {"add", [](lua::State* L) -> int {
            lua::pushinteger(L, lua::tointeger(L, 1) + lua::tointeger(L, 2));
            return 1;
        }},
        {"mul", [](lua::State* L) -> int {
            lua::pushinteger(L, lua::tointeger(L, 1) * lua::tointeger(L, 2));
            return 1;
        }},
        {nullptr, nullptr}
    };

    lua::newtable(L);
    lua::L_setfuncs(L, mylib, 0);
    lua::setglobal(L, "mylib");

    lua::L_dostring(L, "r1 = mylib.add(3, 4)");
    lua::getglobal(L, "r1");
    EXPECT_EQ(lua::tointeger(L, -1), 7);
    lua::pop(L, 1);

    lua::L_dostring(L, "r2 = mylib.mul(5, 6)");
    lua::getglobal(L, "r2");
    EXPECT_EQ(lua::tointeger(L, -1), 30);

    lua::close(L);
}

TEST(ScenarioTest, ErrorHandling) {
    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    lua::CFunction safe_div = [](lua::State* L) -> int {
        auto b = lua::tonumber(L, 2);
        if (b == 0.0) {
            lua::pushnil(L);
            lua::pushstring(L, "division by zero");
            return 2;
        }
        lua::pushnumber(L, lua::tonumber(L, 1) / b);
        return 1;
    };

    lua::register_func(L, "safe_div", safe_div);
    lua::L_dostring(L, "r = safe_div(10, 0)");
    lua::getglobal(L, "r");
    EXPECT_NE(lua::isnil(L, -1), 0);

    lua::L_dostring(L, "r = safe_div(10, 2)");
    lua::getglobal(L, "r");
    EXPECT_DOUBLE_EQ(lua::tonumber(L, -1), 5.0);

    lua::close(L);
}

// ============================================================================
// Debug Interface
// ============================================================================

TEST(DebugTest, GetStack) {
    auto* L = lua::L_newstate();
    lua::Debug ar;
    int result = lua::getstack(L, 0, &ar);
    EXPECT_EQ(result, 0);
    lua::close(L);
}

TEST(DebugTest, HookSetAndGet) {
    auto* L = lua::L_newstate();

    lua::Hook hook = [](lua::State*, lua::Debug*) {};
    lua::sethook(L, hook, lua::MASKCALL | lua::MASKRET, 0);
    EXPECT_EQ(lua::gethook(L), hook);
    EXPECT_NE(lua::gethookmask(L) & lua::MASKCALL, 0);

    lua::sethook(L, nullptr, 0, 0);
    lua::close(L);
}

// ============================================================================
// Extra Space / Warn
// ============================================================================

TEST(MiscTest, GetExtraSpace) {
    auto* L = lua::L_newstate();
    auto* extra = lua::getextraspace(L);
    EXPECT_NE(extra, nullptr);
    lua::close(L);
}

TEST(MiscTest, AtPanic) {
    auto* L = lua::L_newstate();
    lua::CFunction panic_fn = [](lua::State*) -> int { return 0; };
    lua::atpanic(L, panic_fn);
    lua::close(L);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
