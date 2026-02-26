import std;
import mcpplibs.capi.lua;

namespace lua = mcpplibs::capi::lua;

static int cpp_add(lua::State* L) {
    auto a = lua::tonumber(L, 1);
    auto b = lua::tonumber(L, 2);
    lua::pushnumber(L, a + b);
    return 1;
}

static int cpp_greet(lua::State* L) {
    auto* name = lua::tostring(L, 1);
    lua::pushfstring(L, "hello, %s!", name);
    return 1;
}

static int cpp_range(lua::State* L) {
    auto start = lua::tointeger(L, 1);
    auto stop = lua::tointeger(L, 2);
    lua::newtable(L);
    for (lua::Integer i { start }; i <= stop; i++) {
        lua::pushinteger(L, i);
        lua::rawseti(L, -2, i - start + 1);
    }
    return 1;
}

static int cpp_counter(lua::State* L) {
    auto count = lua::tointeger(L, lua::upvalueindex(1));
    count++;
    lua::pushinteger(L, count);
    lua::copy(L, -1, lua::upvalueindex(1));
    return 1;
}

int main() {
    std::println("=== mcpplibs.capi.lua function example ===\n");

    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    // --- Register simple functions ---
    std::println("[1] Register C++ functions:");
    lua::register_func(L, "cpp_add", cpp_add);
    lua::register_func(L, "cpp_greet", cpp_greet);
    lua::register_func(L, "cpp_range", cpp_range);

    lua::L_dostring(L,
        "print('    cpp_add(10, 20) = ' .. cpp_add(10, 20))\n"
        "print('    cpp_greet(\"mcpp\") = ' .. cpp_greet('mcpp'))\n"
    );

    // --- Return table from C++ function ---
    std::println("[2] Return table from C++:");
    lua::L_dostring(L,
        "local nums = cpp_range(1, 5)\n"
        "local parts = {}\n"
        "for _, v in ipairs(nums) do parts[#parts+1] = tostring(v) end\n"
        "print('    cpp_range(1, 5) = {' .. table.concat(parts, ', ') .. '}')\n"
    );

    // --- Closure with upvalue ---
    std::println("[3] Closure with upvalue:");
    lua::pushinteger(L, 0);
    lua::pushcclosure(L, cpp_counter, 1);
    lua::setglobal(L, "counter");

    lua::L_dostring(L,
        "print('    counter() = ' .. counter())\n"
        "print('    counter() = ' .. counter())\n"
        "print('    counter() = ' .. counter())\n"
    );

    // --- Register a module with luaL_setfuncs ---
    std::println("[4] Register module with L_setfuncs:");
    static const lua::L_Reg mymath[] = {
        {"square", [](lua::State* L) -> int {
            auto x = lua::tonumber(L, 1);
            lua::pushnumber(L, x * x);
            return 1;
        }},
        {"cube", [](lua::State* L) -> int {
            auto x = lua::tonumber(L, 1);
            lua::pushnumber(L, x * x * x);
            return 1;
        }},
        {nullptr, nullptr}
    };

    lua::newtable(L);
    lua::L_setfuncs(L, mymath, 0);
    lua::setglobal(L, "mymath");

    lua::L_dostring(L,
        "print('    mymath.square(5) = ' .. mymath.square(5))\n"
        "print('    mymath.cube(3)   = ' .. mymath.cube(3))\n"
    );

    // --- Call Lua function from C++ ---
    std::println("[5] Call Lua function from C++:");
    lua::L_dostring(L,
        "function fibonacci(n)\n"
        "    if n <= 1 then return n end\n"
        "    return fibonacci(n-1) + fibonacci(n-2)\n"
        "end\n"
    );

    lua::getglobal(L, "fibonacci");
    lua::pushinteger(L, 10);
    if (lua::pcall(L, 1, 1, 0) == lua::OK) {
        std::println("    fibonacci(10) = {}", lua::tointeger(L, -1));
    }
    lua::pop(L, 1);

    lua::close(L);
    std::println("\ndone.");
    return 0;
}
