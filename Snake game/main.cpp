#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;
#define H 27
#define W 60

const int dir[4][2] = {
	{-1,0},
	{0,1},
	{1,0},
	{0,-1},
};
enum BlockType {
	EMPTY = 0,
	FOOD = 1,
};
struct Map {
	bool hasFood;
	BlockType data[H][W];
};
void initMap(Map* map) {
	map->hasFood = false;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			map->data[y][x] = BlockType::EMPTY;
		}
	}
}
struct Pos {
	int x;
	int y;
};
struct Snake {
	Pos snake[H * W];
	int snakeDir;
	int snakeLength;
	int lastMoveTime=0;
	int moveFrequency=200;
};

void initSnake(Snake* snk) {
	snk->snakeLength = 3;
	snk->snakeDir = 1;
	snk->snake[0] = { W / 2,H / 2 };
	snk->snake[1] = { W / 2 - 1,H / 2 };
	snk->snake[2] = { W / 2 - 2,H / 2 };
}

void hideCursor() {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfor = { 1,FALSE };
	SetConsoleCursorInfo(hOutput, &curInfor);
}

void drawUnit(Pos p, const char unit[]) {
	COORD coord;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = p.x + 1;
	coord.Y = p.y + 1;
	SetConsoleCursorPosition(hOutput, coord);
	cout << unit;
}

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

void drawSnake(Snake* snk) {
	for (int i = 0; i < snk->snakeLength; i++)
		drawUnit(snk->snake[i], "O");
}


bool checkOutofBound(Pos p) {
	return (p.x <= 0 || p.x >= W || p.y <= 0 || p.y >= H );
	return false;
}
void MoveSnake(Snake* snk) {
	for (int i = snk->snakeLength - 1; i >= 1; i--) {
		snk->snake[i] = snk->snake[i - 1];
	}
	snk->snake[0].x += dir[snk->snakeDir][1];
	snk->snake[0].y += dir[snk->snakeDir][0];
}
void checkEatFood(Snake* snk,Pos tail, Map* map) {
	Pos head = snk->snake[0];
	if (map->data[head.y][head.x] == BlockType::FOOD) {
		snk->snake[snk->snakeLength++] = tail;
		map->data[head.y][head.x] = BlockType::EMPTY;
		map->hasFood = false;
		drawUnit(tail, "O");
	}
}
bool doMove(Snake* snk, Map* map) {
	Pos tail = snk->snake[snk->snakeLength - 1];
	drawUnit(tail, " ");
	MoveSnake(snk);
	if (checkOutofBound(snk->snake[0])) {
		return false;
	}
	checkEatFood(snk, tail, map);
	drawUnit(snk->snake[0], "O");
	return true;
}
bool checkSnakeMove(Snake* snk, Map* map) {
	int cur = GetTickCount();
	if (cur - snk->lastMoveTime > snk->moveFrequency) {
		if (doMove(snk, map) == false) {
			return false;
		}
		snk->lastMoveTime = cur;
	}
	return true;
}
void checkChangeDir(Snake* snk) {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':
			if(snk->snakeDir!=2)
				snk->snakeDir = 0;
			break;
		case 'd':
			if (snk->snakeDir != 3)
				snk->snakeDir = 1;
			break;
		case 's':
			if (snk->snakeDir != 0)
				snk->snakeDir = 2;
			break;
		case 'a':
			if (snk->snakeDir != 1)
				snk->snakeDir = 3;
			break;
		default:
			break;
		}
	}
}
void checkFoodGenerate(Snake* snk, Map* map) {
	if (false == map->hasFood) {
		while (1) {
			int x = rand() % W;
			int y = rand() % H;
			int i = 0;
			while (i < snk->snakeLength) {
				if (x == snk->snake[i].x && y == snk->snake[i].y) {
					break;
				}
				i++;
			}
			if (i == snk->snakeLength) {
				map->data[y][x] = BlockType::FOOD;
				map->hasFood = true;
				drawUnit({ x,y }, "*");
				return;
			}
		}
	}
}
void initGame(Snake* snk, Map* map) {
	hideCursor();
	initMap(map);
	initSnake(snk);
	drawMap(map);
	drawSnake(snk);
}
int main() {
	SetConsoleOutputCP(CP_UTF8);
	Map map;
	Snake snk;
	initGame(&snk, &map);
	while (1) {
		checkChangeDir(&snk);
		if (checkSnakeMove(&snk, &map) == false) {
			break;
		}
		checkFoodGenerate(&snk, &map);
	}
	drawUnit({ W / 2 - 4,H / 2 }, "Game Over");
	while(1){}
	return 0;
}
