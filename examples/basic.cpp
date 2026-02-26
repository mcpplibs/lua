import std;
import mcpplibs.capi.lua;

namespace lua = mcpplibs::capi::lua;

int main() {
    std::println("=== mcpplibs.capi.lua basic example ===\n");

    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    std::println("[1] Lua version: {}", lua::version(L));

    std::println("[2] Execute Lua code:");
    lua::L_dostring(L, "print('    hello from Lua!')");

    std::println("[3] Push and read values:");
    lua::pushinteger(L, 42);
    lua::pushstring(L, "mcpp");
    lua::pushnumber(L, 3.14);
    lua::pushboolean(L, 1);

    std::println("    stack size = {}", lua::gettop(L));
    std::println("    [1] integer = {}", lua::tointeger(L, 1));
    std::println("    [2] string  = {}", lua::tostring(L, 2));
    std::println("    [3] number  = {}", lua::tonumber(L, 3));
    std::println("    [4] boolean = {}", lua::toboolean(L, 4));

    std::println("[4] Compute in Lua:");
    lua::L_dostring(L, "result = 6 * 7");
    lua::getglobal(L, "result");
    std::println("    6 * 7 = {}", lua::tointeger(L, -1));

    lua::close(L);
    std::println("\ndone.");
    return 0;
}
