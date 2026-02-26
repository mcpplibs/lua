# mcpplibs Agent Skills

用于指导 Agent 在 mcpplibs 生态中开发的技能集合。

## 可用技能

| 技能 | 说明 |
|------|------|
| [mcpp-style-ref](mcpp-style-ref/SKILL.md) | 面向 mcpp 项目的 Modern/Module C++ (C++23) 命名、模块化与实践规则 |
| [mcpplibs-capi-lua](mcpplibs-capi-lua/SKILL.md) | Lua C API 的 C++23 模块化绑定 — 类型映射、API 速查、常见模式与构建配置 |

## 使用方式

要在 Cursor 中使用，请将技能软链接或复制到项目的 `.cursor/skills/`：

```bash
mkdir -p .cursor/skills
ln -s ../../skills/mcpp-style-ref .cursor/skills/mcpp-style-ref
ln -s ../../skills/mcpplibs-capi-lua .cursor/skills/mcpplibs-capi-lua
```

或安装为个人技能：

```bash
ln -s /path/to/skills/mcpp-style-ref ~/.cursor/skills/mcpp-style-ref
ln -s /path/to/skills/mcpplibs-capi-lua ~/.cursor/skills/mcpplibs-capi-lua
```
