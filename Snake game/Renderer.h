#pragma once
#include "Common.h"

class Renderer {
public:
    static void gotoXY(int x, int y);
    static void setColor(int color);
    static void hideCursor();
    static void drawUnit(Pos p, const char unit[]);
};
