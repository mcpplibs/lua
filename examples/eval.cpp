import std;
import mcpplibs.capi.lua;

namespace lua = mcpplibs::capi::lua;

static void run_and_show(lua::State* L, const char* code) {
    std::println("  lua> {}", code);
    if (lua::L_dostring(L, code) != lua::OK) {
        std::println("  error: {}", lua::tostring(L, -1));
        lua::pop(L, 1);
    }
}

int main() {
    std::println("=== mcpplibs.capi.lua eval example ===\n");

    auto* L = lua::L_newstate();
    lua::L_openlibs(L);

    // --- Evaluate expressions and get results ---
    std::println("[1] Evaluate expressions:");
    auto eval = [](lua::State* L, const char* expr) -> bool {
        auto code = std::string("return ") + expr;
        if (lua::L_dostring(L, code.c_str()) == lua::OK) {
            const char* typeName = lua::L_typename(L, -1);
            if (lua::isinteger(L, -1)) {
                std::println("    {} = {} ({})", expr, lua::tointeger(L, -1), typeName);
            } else if (lua::isnumber(L, -1)) {
                std::println("    {} = {} ({})", expr, lua::tonumber(L, -1), typeName);
            } else if (lua::isboolean(L, -1)) {
                std::println("    {} = {} ({})", expr, lua::toboolean(L, -1) ? "true" : "false", typeName);
            } else if (lua::isstring(L, -1)) {
                std::println("    {} = \"{}\" ({})", expr, lua::tostring(L, -1), typeName);
            } else if (lua::isnil(L, -1)) {
                std::println("    {} = nil ({})", expr, typeName);
            } else {
                std::println("    {} = [{}]", expr, typeName);
            }
            lua::pop(L, 1);
            return true;
        }
        std::println("    {} => error: {}", expr, lua::tostring(L, -1));
        lua::pop(L, 1);
        return false;
    };

    eval(L, "1 + 2 * 3");
    eval(L, "2 ^ 10");
    eval(L, "10 // 3");
    eval(L, "10 % 3");
    eval(L, "'hello' .. ' ' .. 'world'");
    eval(L, "#'abcdef'");
    eval(L, "true and false");
    eval(L, "not false");
    eval(L, "type(42)");
    eval(L, "math.pi");

    // --- Multi-line Lua script ---
    std::println("\n[2] Multi-line script:");
    run_and_show(L,
        "local sum = 0\n"
        "for i = 1, 100 do sum = sum + i end\n"
        "print('    sum(1..100) = ' .. sum)\n"
    );

    // --- Error handling ---
    std::println("\n[3] Error handling:");
    run_and_show(L, "error('intentional error')");

    // --- Load and execute chunks ---
    std::println("\n[4] Load and execute chunk:");
    int status = lua::L_loadstring(L,
        "local function fact(n)\n"
        "    if n <= 1 then return 1 end\n"
        "    return n * fact(n - 1)\n"
        "end\n"
        "return fact(10)\n"
    );
    if (status == lua::OK) {
        std::println("    chunk loaded successfully");
        if (lua::pcall(L, 0, 1, 0) == lua::OK) {
            std::println("    10! = {}", lua::tointeger(L, -1));
            lua::pop(L, 1);
        }
    }

    // --- Multiple return values ---
    std::println("\n[5] Multiple return values:");
    lua::L_dostring(L,
        "function multi_ret()\n"
        "    return 'hello', 42, true\n"
        "end\n"
    );
    lua::getglobal(L, "multi_ret");
    if (lua::pcall(L, 0, 3, 0) == lua::OK) {
        std::println("    ret[1] = \"{}\" ({})", lua::tostring(L, -3), lua::L_typename(L, -3));
        std::println("    ret[2] = {} ({})", lua::tointeger(L, -2), lua::L_typename(L, -2));
        std::println("    ret[3] = {} ({})", lua::toboolean(L, -1) ? "true" : "false", lua::L_typename(L, -1));
        lua::pop(L, 3);
    }

    // --- String buffer operations ---
    std::println("\n[6] Buffer operations:");
    lua::L_Buffer buf;
    lua::L_buffinit(L, &buf);
    lua::L_addstring(&buf, "Lua ");
    lua::L_addstring(&buf, std::to_string(static_cast<int>(lua::version(L) / 100)).c_str());
    lua::L_addstring(&buf, ".");
    lua::L_addstring(&buf, std::to_string(static_cast<int>(lua::version(L)) % 100).c_str());
    lua::L_pushresult(&buf);
    std::println("    built string: \"{}\"", lua::tostring(L, -1));
    lua::pop(L, 1);

    lua::close(L);
    std::println("\ndone.");
    return 0;
}
