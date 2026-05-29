#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

// 地图尺寸常量
#define H 27  // 高度（行数）
#define W 60  // 宽度（列数）

// 方向偏移表：索引 0=上, 1=右, 2=下, 3=左
// 每个元素为 {dy, dx}
const int dir[4][2] = {
	{-1,0},  // 上
	{0,1},   // 右
	{1,0},   // 下
	{0,-1},  // 左
};

// 地图格子类型
enum BlockType {
	EMPTY = 0,  // 空格
	FOOD = 1,   // 食物
};

// 地图网格
struct Map {
	bool hasFood;          // 当前是否有食物存在
	BlockType data[H][W];  // 每个格子的类型
};

// 初始化地图：清空所有格子，标记无食物
void initMap(Map* map) {
	map->hasFood = false;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			map->data[y][x] = BlockType::EMPTY;
		}
	}
}

// 坐标点
struct Pos {
	int x;
	int y;
};

// 蛇的数据结构
struct Snake {
	Pos snake[H * W];       // 蛇身坐标数组（最大长度 = 地图格子总数）
	int snakeDir;           // 当前移动方向（0=上, 1=右, 2=下, 3=左）
	int snakeLength;        // 蛇当前长度
	int lastMoveTime = 0;   // 上次移动的时间戳
	int moveFrequency = 200;// 移动间隔（毫秒）
};

// 初始化蛇：长度为 3，水平居中，方向朝右
void initSnake(Snake* snk) {
	snk->snakeLength = 3;
	snk->snakeDir = 1;                      // 方向 = 右
	snk->snake[0] = { W / 2, H / 2 };       // 蛇头
	snk->snake[1] = { W / 2 - 1, H / 2 };   // 蛇身
	snk->snake[2] = { W / 2 - 2, H / 2 };   // 蛇尾
}

// 隐藏控制台光标
void hideCursor() {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfor = { 1, FALSE };
	SetConsoleCursorInfo(hOutput, &curInfor);
}

// 在指定位置绘制一个单位（字符）
void drawUnit(Pos p, const char unit[]) {
	COORD coord;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = p.x + 1;  // +1 避开左边框
	coord.Y = p.y + 1;  // +1 避开上边框
	SetConsoleCursorPosition(hOutput, coord);
	cout << unit;
}

// 绘制地图边框
void drawMap(Map* map) {
	system("cls");
	// 上边框
	cout << "+";
	for (int i = 0; i < W; i++) cout << "-";
	cout << "+" << endl;
	// 中间区域（空格）
	for (int i = 0; i < H; i++) {
		cout << "|";
		for (int j = 0; j < W; j++) cout << " ";
		cout << "|" << endl;
	}
	// 下边框
	cout << "+";
	for (int i = 0; i < W; i++) cout << "-";
	cout << "+" << endl;
}

// 绘制蛇身所有节点
void drawSnake(Snake* snk) {
	for (int i = 0; i < snk->snakeLength; i++)
		drawUnit(snk->snake[i], "O");
}

// 检测坐标是否超出边界
bool checkOutofBound(Pos p) {
	return (p.x <= 0 || p.x >= W || p.y <= 0 || p.y >= H);
	return false;
}

// 蛇的移动核心逻辑：尾部前置法
void MoveSnake(Snake* snk) {
	// 从尾部到头部，逐个将前一个节点的坐标复制到当前节点
	for (int i = snk->snakeLength - 1; i >= 1; i--) {
		snk->snake[i] = snk->snake[i - 1];
	}
	// 蛇头沿当前方向前进一格
	snk->snake[0].x += dir[snk->snakeDir][1];
	snk->snake[0].y += dir[snk->snakeDir][0];
}

// 检测蛇头是否吃到食物，是则增长
void checkEatFood(Snake* snk, Pos tail, Map* map) {
	Pos head = snk->snake[0];
	if (map->data[head.y][head.x] == BlockType::FOOD) {
		// 将移动前保存的尾部坐标追加到蛇尾，实现长度 +1
		snk->snake[snk->snakeLength++] = tail;
		// 清除食物标记
		map->data[head.y][head.x] = BlockType::EMPTY;
		map->hasFood = false;
		// 绘制新追加的尾部
		drawUnit(tail, "O");
	}
}

// 执行一次完整移动
// 返回值：true=移动成功，false=撞墙游戏结束
bool doMove(Snake* snk, Map* map) {
	// 保存蛇尾坐标（后续吃食物增长时需要）
	Pos tail = snk->snake[snk->snakeLength - 1];
	// 擦除蛇尾（用空格覆盖）
	drawUnit(tail, " ");
	// 移动蛇身
	MoveSnake(snk);
	// 检查蛇头是否撞墙
	if (checkOutofBound(snk->snake[0])) {
		return false;
	}
	// 检查是否吃到食物
	checkEatFood(snk, tail, map);
	// 绘制新蛇头
	drawUnit(snk->snake[0], "O");
	return true;
}

// 检查是否到达移动时间间隔
// 返回值：true=继续游戏，false=游戏结束
bool checkSnakeMove(Snake* snk, Map* map) {
	int cur = GetTickCount();  // 当前系统时间（毫秒）
	if (cur - snk->lastMoveTime > snk->moveFrequency) {
		if (doMove(snk, map) == false) {
			return false;
		}
		snk->lastMoveTime = cur;
	}
	return true;
}

// 检测键盘输入并更新蛇的方向（禁止反向）
void checkChangeDir(Snake* snk) {
	if (_kbhit()) {  // 有按键输入
		switch (_getch()) {
		case 'w':  // 向上
			if (snk->snakeDir != 2)  // 禁止当前向下时直接向上
				snk->snakeDir = 0;
			break;
		case 'd':  // 向右
			if (snk->snakeDir != 3)  // 禁止当前向左时直接向右
				snk->snakeDir = 1;
			break;
		case 's':  // 向下
			if (snk->snakeDir != 0)  // 禁止当前向上时直接向下
				snk->snakeDir = 2;
			break;
		case 'a':  // 向左
			if (snk->snakeDir != 1)  // 禁止当前向右时直接向左
				snk->snakeDir = 3;
			break;
		default:
			break;
		}
	}
}

// 检查是否需要生成新食物（地图上无食物时生成）
void checkFoodGenerate(Snake* snk, Map* map) {
	if (false == map->hasFood) {
		while (1) {
			// 随机生成食物位置
			int x = rand() % W;
			int y = rand() % H;
			// 检查是否与蛇身重合
			int i = 0;
			while (i < snk->snakeLength) {
				if (x == snk->snake[i].x && y == snk->snake[i].y) {
					break;  // 重合，重新生成
				}
				i++;
			}
			if (i == snk->snakeLength) {  // 不重合，有效位置
				map->data[y][x] = BlockType::FOOD;
				map->hasFood = true;
				drawUnit({ x, y }, "*");  // 绘制食物
				return;
			}
		}
	}
}

// 游戏初始化
void initGame(Snake* snk, Map* map) {
	hideCursor();     // 隐藏光标
	initMap(map);     // 初始化地图
	initSnake(snk);   // 初始化蛇
	drawMap(map);     // 绘制地图边框
	drawSnake(snk);   // 绘制蛇
}

int main() {
	SetConsoleOutputCP(CP_UTF8);  // 启用 UTF-8 输出

	Map map;
	Snake snk;
	initGame(&snk, &map);        // 初始化

	// 游戏主循环
	while (1) {
		checkChangeDir(&snk);                            // 处理输入
		if (checkSnakeMove(&snk, &map) == false) {       // 移动 + 碰撞检测
			break;                                       // 撞墙，结束
		}
		checkFoodGenerate(&snk, &map);                   // 生成食物
	}

	// 显示游戏结束
	drawUnit({ W / 2 - 4, H / 2 }, "Game Over");
	while (1) {}  // 暂停窗口
	return 0;
}
