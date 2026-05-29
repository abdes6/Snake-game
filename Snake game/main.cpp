#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// 地图尺寸
#define H 27  // 高度
#define W 60  // 宽度

// 方向偏移表 {dy, dx}：0上 1右 2下 3左
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
	bool hasFood;          // 是否有食物存在
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
	Pos snake[H * W];       // 蛇身坐标数组
	int snakeDir;           // 当前方向 0上 1右 2下 3左
	int snakeLength;        // 蛇当前长度
	int lastMoveTime = 0;   // 上次移动时间戳
	int moveFrequency = 200;// 移动间隔（毫秒）
};

// 初始化蛇：长度为3，水平居中，方向朝右
void initSnake(Snake* snk) {
	snk->snakeLength = 3;
	snk->snakeDir = 1;                      // 方向朝右
	snk->snake[0] = { W / 2, H / 2 };       // 蛇头
	snk->snake[1] = { W / 2 - 1, H / 2 };   // 蛇身
	snk->snake[2] = { W / 2 - 2, H / 2 };   // 蛇尾
}

// 设置光标位置（绝对坐标，无偏移）
void gotoXY(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 设置控制台文字颜色
void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 隐藏控制台闪烁光标
void hideCursor() {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfor = { 1, FALSE };
	SetConsoleCursorInfo(hOutput, &curInfor);
}

// 在指定位置绘制字符（+1 偏移避开边框）
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
	cout << "+";
	for (int i = 0; i < W; i++) cout << "-";
	cout << "+" << endl;
	for (int i = 0; i < H; i++) {
		cout << "|";
		for (int j = 0; j < W; j++) cout << " ";
		cout << "|" << endl;
	}
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
	// 从尾部到头部，逐个前移覆盖
	for (int i = snk->snakeLength - 1; i >= 1; i--) {
		snk->snake[i] = snk->snake[i - 1];
	}
	// 蛇头沿当前方向前进一格
	snk->snake[0].x += dir[snk->snakeDir][1];
	snk->snake[0].y += dir[snk->snakeDir][0];
}
// 检测蛇头是否吃到食物，是则增长长度，返回 true 表示吃到了
bool checkEatFood(Snake* snk, Pos tail, Map* map) {
	Pos head = snk->snake[0];
	if (map->data[head.y][head.x] == BlockType::FOOD) {
		snk->snake[snk->snakeLength++] = tail;  // 尾部追加，长度+1
		map->data[head.y][head.x] = BlockType::EMPTY;
		map->hasFood = false;
		drawUnit(tail, "O");                     // 绘制新尾部
		return true;
	}
	return false;
}
// 执行一次完整移动，返回 true 成功 / false 撞墙结束，ate 传出是否吃到食物
bool doMove(Snake* snk, Map* map, bool* ate) {
	Pos tail = snk->snake[snk->snakeLength - 1];
	drawUnit(tail, " ");
	MoveSnake(snk);
	if (checkOutofBound(snk->snake[0])) {
		return false;
	}
	if (checkEatFood(snk, tail, map) && ate) {
		*ate = true;
	}
	drawUnit(snk->snake[0], "O");
	return true;
}
// 按时间间隔控制移动（200ms），返回 false 即游戏结束，ate 传出是否吃到食物
bool checkSnakeMove(Snake* snk, Map* map, bool* ate) {
	int cur = GetTickCount();  // 当前时间 ms
	if (cur - snk->lastMoveTime > snk->moveFrequency) {
		if (doMove(snk, map, ate) == false) {
			return false;
		}
		snk->lastMoveTime = cur;
	}
	return true;
}
// 检测键盘输入并更新方向（禁止反向：如向右时不能直接向左）
void checkChangeDir(Snake* snk) {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':  // 上
			if (snk->snakeDir != 2)
				snk->snakeDir = 0;
			break;
		case 'd':  // 右
			if (snk->snakeDir != 3)
				snk->snakeDir = 1;
			break;
		case 's':  // 下
			if (snk->snakeDir != 0)
				snk->snakeDir = 2;
			break;
		case 'a':  // 左
			if (snk->snakeDir != 1)
				snk->snakeDir = 3;
			break;
		default:
			break;
		}
	}
}
// 地图上无食物时，随机生成新食物（避开蛇身）
void checkFoodGenerate(Snake* snk, Map* map) {
	if (false == map->hasFood) {
		while (1) {
			int x = rand() % W;
			int y = rand() % H;
			int i = 0;
			while (i < snk->snakeLength) {
				if (x == snk->snake[i].x && y == snk->snake[i].y) {
					break;  // 与蛇身重合，重新生成
				}
				i++;
			}
			if (i == snk->snakeLength) {  // 不重合则放置食物
				map->data[y][x] = BlockType::FOOD;
				map->hasFood = true;
				drawUnit({ x, y }, "*");
				return;
			}
		}
	}
}
// 游戏初始化：隐藏光标、初始化地图和蛇、绘制界面
void initGame(Snake* snk, Map* map) {
	hideCursor();
	initMap(map);
	initSnake(snk);
	drawMap(map);
	drawSnake(snk);
}
// 菜单选项
enum MenuChoice {
	MENU_START = 0,
	MENU_HOWTO,
	MENU_EXIT,
};

// 显示主菜单，返回用户选择
int showMenu() {
	int selected = 0;
	const char* items[] = { "Start Game", "How to Play", "Exit" };
	int itemCount = 3;

	while (1) {
		system("cls");
		setColor(7);

		// 上边框
		gotoXY(0, 0); cout << "+";
		for (int i = 0; i < W; i++) cout << "-";
		cout << "+";

		// 两侧空白
		for (int i = 1; i < H; i++) {
			gotoXY(0, i); cout << "|";
			gotoXY(W + 1, i); cout << "|";
		}

		// 下边框
		gotoXY(0, H); cout << "+";
		for (int i = 0; i < W; i++) cout << "-";
		cout << "+";

		// 标题
		const char* title = "S N A K E   G A M E";
		int titleLen = strlen(title);
		setColor(11); gotoXY((W - titleLen) / 2 + 1, 4); cout << title;

		// 副标题
		const char* sub = "C L A S S I C   S N A K E";
		int subLen = strlen(sub);
		setColor(8); gotoXY((W - subLen) / 2 + 1, 6); cout << sub;

		// 菜单项
		for (int i = 0; i < itemCount; i++) {
			int y = 10 + i * 3;
			setColor(7);
			gotoXY(W / 2 - 8, y);
			if (i == selected) {
				setColor(10); cout << ">> ";
				setColor(14); cout << items[i];
			} else {
				cout << "   " << items[i];
			}
		}

		// 底部提示
		setColor(8);
		gotoXY(W / 2 -18 , H - 3); cout << "W S  Select  Enter Confirm  ESC Exit";

		// 输入处理
		int key = _getch();
		if (key == 224 || key == 0) {
			key = _getch();
			if (key == 72) selected = (selected - 1 + itemCount) % itemCount;
			else if (key == 80) selected = (selected + 1) % itemCount;
		} else if (key == 'w' || key == 'W') {
			selected = (selected - 1 + itemCount) % itemCount;
		} else if (key == 's' || key == 'S') {
			selected = (selected + 1) % itemCount;
		} else if (key == 13) {
			return selected;
		} else if (key == 27) {
			return MENU_EXIT;
		}
	}
}

// 显示操作说明
void showHowToPlay() {
	system("cls");
	for (int i = 0; i <= H; i++) {
		gotoXY(0, i); cout << "|";
		gotoXY(W + 1, i); cout << "|";
	}
	gotoXY(0, 0); cout << "+";
	for (int i = 0; i < W; i++) cout << "-";
	cout << "+";
	gotoXY(0, H); cout << "+";
	for (int i = 0; i < W; i++) cout << "-";
	cout << "+";

	setColor(14);
	gotoXY(W / 2 - 6, 3); cout << "H O W   T O   P L A Y";

	setColor(7);
	gotoXY(W / 2 - 12, 6); cout << "W / Up      S / Down";
	gotoXY(W / 2 - 12, 7); cout << "A / Left    D / Right";

	setColor(14);
	gotoXY(W / 2 - 6, 10); cout << "G A M E   R U L E S";
	setColor(7);
	gotoXY(W / 2 - 12, 12); cout << "*  Eat food (*) to grow & score";
	gotoXY(W / 2 - 12, 13); cout << "*  Hit wall = Game Over";
	gotoXY(W / 2 - 12, 14); cout << "*  Can't reverse direction";

	setColor(8);
	gotoXY(W / 2 - 14, H - 3); cout << "ESC = Menu    Enter = Start Game";

	while (1) {
		int key = _getch();
		if (key == 27) return;
		if (key == 13) return;
	}
}

// 显示游戏结束界面，返回用户选择 0=再来一次 1=返回菜单
int showGameOver(int score) {
	int selected = 0;
	const char* items[] = { "Play Again", "Main Menu" };
	int itemCount = 2;

	while (1) {
		setColor(12);
		gotoXY(W / 2 - 4, H / 2 - 4); cout << "GAME OVER";
		setColor(14);
		gotoXY(W / 2 - 4, H / 2 - 2); cout << "Score: " << score;

		for (int i = 0; i < itemCount; i++) {
			int y = H / 2 + i * 2;
			setColor(7);
			gotoXY(W / 2 - 6, y);
			if (i == selected) {
				setColor(10); cout << ">> ";
				setColor(15); cout << items[i];
			} else {
				cout << "   " << items[i];
			}
		}

		setColor(8);
		gotoXY(W / 2 - 12, H / 2 + 5); cout << "W S  Select  Enter Confirm";

		int key = _getch();
		if (key == 224 || key == 0) {
			key = _getch();
			if (key == 72) selected = (selected - 1 + itemCount) % itemCount;
			else if (key == 80) selected = (selected + 1) % itemCount;
		} else if (key == 'w' || key == 'W') {
			selected = (selected - 1 + itemCount) % itemCount;
		} else if (key == 's' || key == 'S') {
			selected = (selected + 1) % itemCount;
		} else if (key == 13) {
			return selected;
		}
	}
}

// 完整游戏流程，返回 0=正常结束 1=要退出
int runGame() {
	Map map;
	Snake snk;
	int score = 0;

	initGame(&snk, &map);

	while (1) {
		checkChangeDir(&snk);
		bool ate = false;
		if (checkSnakeMove(&snk, &map, &ate) == false) break;
		if (ate) score++;
		checkFoodGenerate(&snk, &map);
	}

	int choice = showGameOver(score);
	return choice;
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	hideCursor();
	srand((unsigned int)time(NULL));

	while (1) {
		int menuChoice = showMenu();
		if (menuChoice == MENU_HOWTO) {
			showHowToPlay();
			continue;
		}
		if (menuChoice == MENU_EXIT) break;

		while (1) {
			int result = runGame();
			if (result == 1) break;
		}
	}

	system("cls");
	setColor(7);
	cout << "Thanks for playing!" << endl;
	return 0;
}
