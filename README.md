# Snake Game — 贪吃蛇

一个基于 Windows 控制台的经典贪吃蛇游戏，使用 C++ 编写。

## 运行方式

### Visual Studio
打开 `Snake game.sln`，选择配置（Debug/Release, x86/x64），按 `F5` 编译运行。

### 命令行（MSVC）
```bash
cl main.cpp *.cpp
main.exe
```

### 环境要求
- Windows 系统
- Visual Studio 2015 或更高版本（项目使用 v141+ 工具集）
- 或任意支持 `Windows.h` 和 `conio.h` 的 C++ 编译器

## 游戏操作

| 按键 | 功能 |
|------|------|
| W / ↑ | 上移 |
| S / ↓ | 下移 |
| A / ← | 左移 |
| D / → | 右移 |
| E | 加速冲刺（持续 1 秒，冷却 3 秒） |
| P | 暂停 / 继续 |
| ESC | 返回菜单（菜单页）/ 退出（菜单页） |

## 游戏规则

- 控制蛇头移动，吃 `*`（食物）可得分并增长
- 撞墙或撞自身则游戏结束，弹出 **"Play Again" / "Main Menu"** 选择
- 不可反向移动（如向右时不能立即向左）
- 每吃 5 个食物，游戏速度提升一级（移动间隔 -15ms，最低 50ms）

## 项目结构

```
Snake game/
├── Snake game.sln              # Visual Studio 解决方案文件
└── Snake game/
    ├── Snake game.vcxproj      # Visual Studio 项目文件
    ├── Common.h                # 常量 H/W、Pos 结构体、BlockType 枚举
    ├── main.cpp                # 程序入口：菜单 → 游戏循环
    ├── Game.h / Game.cpp       # 游戏主循环、输入处理、暂停、冲刺、计分
    ├── Map.h / Map.cpp         # 地图网格、边界绘制、食物生成
    ├── Snake.h / Snake.cpp     # 蛇的数据结构、移动、自碰撞检测
    ├── Menu.h / Menu.cpp       # 主菜单、操作说明页
    └── Renderer.h / Renderer.cpp # 控制台渲染封装（光标定位、颜色、光标隐藏）
```

## 技术实现

| 技术 | 说明 |
|------|------|
| `Windows.h` | 控制台光标定位 (`SetConsoleCursorPosition`)、光标隐藏 (`SetConsoleCursorInfo`) |
| `conio.h` | 非阻塞键盘输入 (`_kbhit` / `_getch`) |
| `GetTickCount()` | 基于时间的移动控制，不受帧率影响 |

### 核心数据结构

- **`Map`** — 60×27 字符网格，`hasFood` 标记食物存在状态
- **`Snake`** — 数组存储蛇身坐标（最大 `H*W`），包含方向、长度
- **`Sprint`** — 冲刺状态：`active`、`startTime`、`lastEndTime`（冷却控制）

### 游戏循环

```
handleInput       → 检测按键输入，更新方向（禁止反向）、暂停/继续、冲刺
updateSprint      → 冲刺持续 1 秒后自动结束，冷却 3 秒
handlePauseLoop   → 暂停时阻塞等待 P 键继续
moveTick          → 按时间间隔移动蛇身、检测边界/自碰撞、吃食物、更新分数
generateFood      → 食物被吃后随机生成新食物（避开蛇身）
```

### 蛇的移动

采用**尾部前置法**：将蛇尾坐标保存，从尾部到头部逐段前移覆盖，蛇头按当前方向前进。吃食物时将保存的尾部坐标追加到蛇尾，实现长度增长。

## 编译配置

Visual Studio 项目支持四种配置：

| 配置 | 平台 | 说明 |
|------|------|------|
| Debug | Win32 | 32 位调试 |
| Release | Win32 | 32 位发布 |
| Debug | x64 | 64 位调试 |
| Release | x64 | 64 位发布 |
