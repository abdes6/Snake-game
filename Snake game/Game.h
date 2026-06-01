#pragma once
#include "Common.h"

class Game {
public:
    void init();
    int run();

private:
    int showGameOver(int score);
};
