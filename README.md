# Snake Game — 贪吃蛇

一个基于 Windows 控制台的经典贪吃蛇游戏，使用 C++ 编写，单文件实现。

## 运行方式

### Visual Studio
打开 `Snake game.sln`，选择配置（Debug/Release, x86/x64），按 `F5` 编译运行。

### 命令行（MSVC）
```bash
cl main.cpp
main.exe
```

### 环境要求
- Windows 系统
- Visual Studio 2015 或更高版本（项目使用 v141+ 工具集）
- 或任意支持 `Windows.h` 和 `conio.h` 的 C++ 编译器

## 菜单操作

| 按键 | 功能 |
|------|------|
| ↑ / W | 上移选项 |
| ↓ / S | 下移选项 |
| Enter | 确认选择 |
| ESC | 返回 / 退出 |

## 游戏操作

| 按键 | 方向 |
|------|------|
| W / ↑ | 上 |
| S / ↓ | 下 |
| A / ← | 左 |
| D / → | 右 |

## 游戏规则

- 控制蛇头移动，吃 `*`（食物）可得分并增长
- 撞墙则游戏结束，显示得分并提供「再来一次」和「返回菜单」选项
- **当前版本未实现自身体碰撞**，蛇可以穿过自己身体

## 菜单界面

游戏启动后进入主菜单，支持三个选项：

1. **开始游戏** — 进入游戏
2. **操作说明** — 查看操作方法和游戏规则
3. **退出游戏** — 退出程序

游戏结束后可在结算界面选择「再来一次」或「返回菜单」。

## 项目结构

```
Snake game/
├── Snake game.sln              # Visual Studio 解决方案文件
└── Snake game/
    ├── Snake game.vcxproj      # Visual Studio 项目文件
    └── main.cpp                # 单文件源代码（共 ~194 行）
```

## 技术实现

| 技术 | 说明 |
|------|------|
| `Windows.h` | 控制台光标定位 (`SetConsoleCursorPosition`)、光标隐藏 (`SetConsoleCursorInfo`) |
| `conio.h` | 非阻塞键盘输入 (`_kbhit` / `_getch`) |
| `GetTickCount()` | 基于时间的移动控制（200ms 间隔），不受帧率影响 |
| `SetConsoleOutputCP(CP_UTF8)` | 启用 UTF-8 控制台输出 |

### 核心数据结构

- **`Map`** — 60×27 字符网格，`hasFood` 标记食物存在状态
- **`Snake`** — 数组存储蛇身坐标（最大 `H*W`），包含方向、长度、移动时间戳
- **`dir[4][2]`** — 方向常量表：上/右/下/左

### 游戏循环

```
checkChangeDir   → 检测按键输入，更新方向（禁止反向）
checkSnakeMove   → 按时间间隔移动蛇身，检测边界碰撞
checkFoodGenerate → 食物被吃后随机生成新食物（避开蛇身）
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
