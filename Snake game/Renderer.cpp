#include "Renderer.h"
#include <iostream>
#include <Windows.h>

void Renderer::gotoXY(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Renderer::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Renderer::hideCursor() {
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo = { 1, FALSE };
    SetConsoleCursorInfo(hOutput, &curInfo);
}

void Renderer::drawUnit(Pos p, const char unit[]) {
    gotoXY(p.x + 1, p.y + 1);
    std::cout << unit;
}
