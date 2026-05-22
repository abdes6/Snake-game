# Snake Game — 贪吃蛇

一个基于 Windows 控制台的经典贪吃蛇游戏，使用 C++ 编写。

## 运行方式

直接用 Visual Studio 2015 打开 `Snake game.sln` 编译运行，或编译 `main.cpp`：

```bash
cl main.cpp
```

## 操作说明

| 按键 | 方向 |
|------|------|
| W    | 上   |
| S    | 下   |
| A    | 左   |
| D    | 右   |

- 吃 `*` 得分并增长
- 撞墙则游戏结束

## 技术要点

- Windows API (`Windows.h`)：控制台光标定位与隐藏
- `conio.h`：非阻塞键盘输入
- 60×27 字符网格地图
- 蛇身使用数组存储，逐帧更新坐标
