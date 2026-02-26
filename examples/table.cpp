import std;
import mcpplibs.capi.lua;

namespace lua = mcpplibs::capi::lua;

int main() {
    std::println("=== mcpplibs.capi.lua table example ===\n");

    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    // --- Create a table from C++ ---
    std::println("[1] Create table from C++:");
    lua::newtable(L);
    lua::pushstring(L, "Alice");
    lua::setfield(L, -2, "name");
    lua::pushinteger(L, 30);
    lua::setfield(L, -2, "age");
    lua::pushstring(L, "alice@example.com");
    lua::setfield(L, -2, "email");
    lua::setglobal(L, "person");

    lua::L_dostring(L,
        "print('    name  = ' .. person.name)\n"
        "print('    age   = ' .. person.age)\n"
        "print('    email = ' .. person.email)\n"
    );

    // --- Array-style table ---
    std::println("[2] Array table:");
    lua::newtable(L);
    const char* fruits[] { "apple", "banana", "cherry", "date" };
    for (int i { 0 }; i < 4; i++) {
        lua::pushstring(L, fruits[i]);
        lua::rawseti(L, -2, i + 1);
    }
    lua::setglobal(L, "fruits");

    lua::L_dostring(L,
        "for i, fruit in ipairs(fruits) do\n"
        "    print('    [' .. i .. '] = ' .. fruit)\n"
        "end\n"
    );

    // --- Nested tables ---
    std::println("[3] Nested tables:");
    lua::newtable(L);

    lua::newtable(L);
    lua::pushnumber(L, 1.0);
    lua::setfield(L, -2, "x");
    lua::pushnumber(L, 2.0);
    lua::setfield(L, -2, "y");
    lua::setfield(L, -2, "position");

    lua::newtable(L);
    lua::pushnumber(L, 10.0);
    lua::setfield(L, -2, "width");
    lua::pushnumber(L, 20.0);
    lua::setfield(L, -2, "height");
    lua::setfield(L, -2, "size");

    lua::setglobal(L, "rect");

    lua::L_dostring(L,
        "print('    rect.position = (' .. rect.position.x .. ', ' .. rect.position.y .. ')')\n"
        "print('    rect.size     = ' .. rect.size.width .. ' x ' .. rect.size.height)\n"
    );

    // --- Table traversal from C++ ---
    std::println("[4] Traverse table from C++:");
    lua::L_dostring(L, "config = { debug=true, level=5, name='test' }");
    lua::getglobal(L, "config");
    lua::pushnil(L);
    while (lua::next(L, -2) != 0) {
        const char* key = lua::tostring(L, -2);
        const char* typeName = lua::L_typename(L, -1);
        if (lua::isstring(L, -1)) {
            std::println("    {} ({}) = {}", key, typeName, lua::tostring(L, -1));
        } else if (lua::isinteger(L, -1)) {
            std::println("    {} ({}) = {}", key, typeName, lua::tointeger(L, -1));
        } else if (lua::isboolean(L, -1)) {
            std::println("    {} ({}) = {}", key, typeName, lua::toboolean(L, -1) ? "true" : "false");
        } else if (lua::isnumber(L, -1)) {
            std::println("    {} ({}) = {}", key, typeName, lua::tonumber(L, -1));
        } else {
            std::println("    {} ({}) = <{}>", key, typeName, typeName);
        }
        lua::pop(L, 1);
    }

    lua::close(L);
    std::println("\ndone.");
    return 0;
}
