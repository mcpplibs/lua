## Cursor Cloud specific instructions

### Project overview

C++23 module binding for Lua C API: `import mcpplibs.capi.lua;`. Uses xmake as primary build system, GCC 15.1 for C++23 module support.

### Environment

- **xlings** provides both xmake and GCC 15.1; install via `curl -fsSL https://raw.githubusercontent.com/d2learn/xlings/refs/heads/main/tools/other/quick_install.sh | XLINGS_NON_INTERACTIVE=1 bash`
- After installing xlings, run `xlings install gcc@15.1 -y` to get GCC 15.1
- PATH must include `$HOME/.xlings/bin` and `$HOME/.xlings/subos/current/bin`

### Build, test, run

Standard commands documented in `README.md`:
- `xmake f -m release -y` to configure
- `xmake -y -j$(nproc)` to build
- `xmake run capi_lua_test` to run tests (97 tests)
- `xmake run basic|table|function|eval` to run examples

### Important caveats

- **GCC C++ modules + C headers**: Lua headers must be wrapped with explicit `extern "C"` via `src/capi/lua_headers.h` in the global module fragment. Without this, GCC applies C++ name mangling to the C functions and linking fails. This is a GCC 15.1 behavior with C++ modules.
- **Interface + implementation split**: The module uses `.cppm` for declarations and `.cpp` for definitions. Functions cannot be `inline` in the `.cppm` because GCC would inline them at the import site where the Lua C declarations are not available, causing link errors.
- **Lua dependency**: xmake auto-downloads Lua via `add_requires("lua")`. Both `tests/xmake.lua` and `examples/xmake.lua` must include their own `add_requires("lua")` and `add_packages("lua")`.
- **Test target**: The test target is named `capi_lua_test` (not `templates_test` from the original template).
- **mcpp style**: Follow [mcpp-style-ref](https://github.com/mcpp-community/mcpp-style-ref). See `.agents/skills/mcpp-style-ref/SKILL.md` for details.
