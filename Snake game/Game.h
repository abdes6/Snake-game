#pragma once
#include "Common.h"
#include "Map.h"
#include "Snake.h"

struct Sprint {
    bool active = false;
    int startTime = 0;
    int lastEndTime = -5000;
};

class Game {
public:
    void init();
    int run(int difficulty);
    static int loadHighScore();
    static void saveHighScore(int score);

private:
    void drawUI(int score);
    void updateSprint(int cur, Sprint& sprint, int& moveFrequency, int baseMoveFrequency);
    void handleInput(int cur, Snake& snake, bool& paused, int& lastMoveTime, Sprint& sprint, int& moveFrequency);
    void handlePauseLoop(bool& paused, int& lastMoveTime);
    bool moveTick(int cur, Map& map, Snake& snake, int& score, int& lastMoveTime, int& baseMoveFrequency, int& moveFrequency, Sprint& sprint);
    int showGameOver(int score, int highScore, bool isNewHighScore);
};