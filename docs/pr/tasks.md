# mcpplibs.capi.lua 任务拆解

> PR 实施计划与任务清单

## 任务列表

### Phase 1: 模块实现

- [x] **T1** 创建 `src/capi/lua.cppm` — 主模块接口文件
  - 全局模块片段引入 `lua.h` / `lauxlib.h` / `lualib.h`
  - 导出类型别名（State, Number, Integer, CFunction 等）
  - 导出常量（OK, TNIL, TBOOLEAN, MULTRET 等）
  - 导出函数（状态管理、栈操作、入栈、取值、表操作、全局变量、调用、辅助库、标准库）

### Phase 2: 构建配置

- [x] **T2** 更新 `xmake.lua` — 新 target `mcpplibs-capi-lua`，添加 lua 依赖
- [x] **T3** 更新 `tests/xmake.lua` — 测试 target 关联新库
- [x] **T4** 更新 `examples/xmake.lua` — 示例 targets 关联新库
- [x] **T5** 更新 `CMakeLists.txt` — CMake 构建支持
- [x] **T6** 更新 `config.xlings` — 项目名称

### Phase 3: 测试

- [x] **T7** 重写 `tests/main.cpp` — 全面的单元测试
  - 状态创建与销毁
  - 栈操作（push/pop/get/set）
  - 类型检查
  - 数值操作
  - 字符串操作
  - 布尔操作
  - 表操作（创建、读写、遍历）
  - 全局变量
  - Lua 脚本执行（luaL_dostring）
  - 函数调用（pcall）
  - 辅助库函数
  - 错误处理
  - 常量正确性

### Phase 4: 示例

- [x] **T8** `examples/basic.cpp` — 基本用法：创建状态、执行脚本、获取结果
- [x] **T9** `examples/table.cpp` — 表操作：创建表、嵌套表、遍历
- [x] **T10** `examples/function.cpp` — C 函数注册：注册 C 函数到 Lua、回调
- [x] **T11** `examples/eval.cpp` — 脚本求值：执行 Lua 代码、获取返回值

### Phase 5: 文档与 CI

- [x] **T12** 更新 `README.md` — 项目说明、用法示例
- [x] **T13** 更新 `.github/workflows/ci.yml` — CI 流水线适配
- [x] **T14** 创建 `docs/pr/design.md` — 设计文档
- [x] **T15** 创建 `docs/pr/tasks.md` — 任务文档（本文件）
- [x] **T16** 更新 `docs/architecture.md` — 架构文档

### Phase 6: 验证

- [x] **T17** 本地构建通过
- [x] **T18** 本地测试通过
- [x] **T19** 本地示例运行通过
- [x] **T20** 推送 GitHub 并等待 CI 全部通过

## 文件变更清单

| 文件 | 操作 | 说明 |
|---|---|---|
| `src/capi/lua.cppm` | 新增 | 主模块接口 |
| `src/templates.cppm` | 删除 | 移除模板占位模块 |
| `xmake.lua` | 修改 | 新 target + lua 依赖 |
| `tests/main.cpp` | 重写 | 全面的 Lua 绑定测试 |
| `tests/xmake.lua` | 修改 | 关联新库 |
| `examples/basic.cpp` | 重写 | Lua 基本用法 |
| `examples/table.cpp` | 新增 | 表操作示例 |
| `examples/function.cpp` | 新增 | 函数注册示例 |
| `examples/eval.cpp` | 新增 | 脚本求值示例 |
| `examples/xmake.lua` | 修改 | 多示例 targets |
| `CMakeLists.txt` | 修改 | CMake 适配 |
| `config.xlings` | 修改 | 项目名称 |
| `README.md` | 重写 | 新项目说明 |
| `.github/workflows/ci.yml` | 修改 | CI 适配 |
| `docs/architecture.md` | 修改 | 架构文档适配 |
| `docs/pr/design.md` | 新增 | 设计文档 |
| `docs/pr/tasks.md` | 新增 | 任务文档 |
