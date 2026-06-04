#include "Game.h"
#include "Renderer.h"
#include <conio.h>
#include <iostream>
#include <Windows.h>

void Game::drawUI(int score) {
    Renderer::gotoXY(2, H + 2);
    Renderer::setColor(14);
    std::cout << "Score: " << score;
    Renderer::setColor(8);
    Renderer::gotoXY(20, H + 2);
    std::cout << "P = Pause";
}

void Game::updateSprint(int cur, Sprint& sprint, int& moveFrequency, int baseMoveFrequency) {
    if (sprint.active && cur - sprint.startTime > 1000) {
        sprint.active = false;
        sprint.lastEndTime = cur;
        moveFrequency = baseMoveFrequency;
    }
}

void Game::handleInput(int cur, Snake& snake, bool& paused, int& lastMoveTime, Sprint& sprint, int& moveFrequency) {
    int key = _getch();
    if (key == 'p' || key == 'P') {
        paused = !paused;
        if (!paused) {
            Renderer::setColor(0);
            Renderer::gotoXY(W / 2 - 3, H / 2);
            std::cout << "      ";
            Renderer::setColor(10);
            lastMoveTime = GetTickCount();
        }
        return;
    }
    if (paused) return;

    if (key >= 'A' && key <= 'Z') key += 32;
    switch (key) {
    case 'w': if (snake.getDir() != 2) snake.setDir(0); break;
    case 'd': if (snake.getDir() != 3) snake.setDir(1); break;
    case 's': if (snake.getDir() != 0) snake.setDir(2); break;
    case 'a': if (snake.getDir() != 1) snake.setDir(3); break;
    case 'e':
        if (!sprint.active && cur - sprint.lastEndTime > 3000) {
            sprint.active = true;
            sprint.startTime = cur;
            moveFrequency = 50;
        }
        break;
    }
}

void Game::handlePauseLoop(bool& paused, int& lastMoveTime) {
    if (!paused) return;
    Renderer::setColor(11);
    Renderer::gotoXY(W / 2 - 3, H / 2);
    std::cout << "PAUSED";
    do {
        int k = _getch();
        if (k == 'p' || k == 'P') {
            lastMoveTime = GetTickCount();
            paused = false;
            Renderer::setColor(0);
            Renderer::gotoXY(W / 2 - 3, H / 2);
            std::cout << "      ";
            Renderer::setColor(10);
        }
    } while (paused);
}

bool Game::moveTick(int cur, Map& map, Snake& snake, int& score,
                     int& lastMoveTime, int& baseMoveFrequency,
                     int& moveFrequency, Sprint& sprint) {
    if (cur - lastMoveTime <= moveFrequency) return false;

    Pos tail = snake.getTail();
    Renderer::setColor(0);
    Renderer::drawUnit(tail, " ");

    snake.move();

    if (snake.checkSelfCollision()) return true;

    Pos head = snake.getHeadPos();
    if (head.x <= 0 || head.x >= W || head.y <= 0 || head.y >= H) return true;
    if (map.isObstacleAt(head.x, head.y)) return true;

    if (map.isFoodAt(head.x, head.y)) {
        snake.grow(tail);
        map.removeFood(head.x, head.y);
        Renderer::setColor(10);
        Renderer::drawUnit(tail, "O");
        score++;
        Renderer::gotoXY(9, H + 2);
        std::cout << score << "  ";
        if (score % 5 == 0 && baseMoveFrequency > 50) {
            baseMoveFrequency -= 15;
            moveFrequency = sprint.active ? max(40, baseMoveFrequency - 60) : baseMoveFrequency;
        }
    }

    Renderer::setColor(10);
    Renderer::drawUnit(head, "O");
    lastMoveTime = cur;
    return false;
}

void Game::init() {
    Renderer::hideCursor();
}

int Game::run(int difficulty) {
    Map map;
    Snake snake;
    int score = 0;
    int lastMoveTime = 0;
    const int obstacleCounts[] = { 5, 10, 15 };
    const int baseSpeeds[] = { 250, 200, 150 };
    int baseMoveFrequency = baseSpeeds[difficulty];
    int moveFrequency = baseMoveFrequency;
    Sprint sprint;
    bool paused = false;

    map.init();
    snake.init();
    map.draw();
    map.generateObstacles(snake, obstacleCounts[difficulty]);
    Renderer::setColor(10);
    snake.draw();
    drawUI(score);

    while (true) {
        int cur = GetTickCount();

        updateSprint(cur, sprint, moveFrequency, baseMoveFrequency);

        if (_kbhit()) {
            handleInput(cur, snake, paused, lastMoveTime, sprint, moveFrequency);
        }

        handlePauseLoop(paused, lastMoveTime);

        if (moveTick(cur, map, snake, score, lastMoveTime, baseMoveFrequency, moveFrequency, sprint)) {
            break;
        }

        map.generateFood(snake);
    }

    return showGameOver(score);
}

int Game::showGameOver(int score) {
    int selected = 0;
    const char* items[] = { "Play Again", "Main Menu" };
    int itemCount = 2;

    while (true) {
        Renderer::setColor(12);
        Renderer::gotoXY(W / 2 - 4, H / 2 - 4); std::cout << "GAME OVER";
        Renderer::setColor(14);
        Renderer::gotoXY(W / 2 - 4, H / 2 - 2); std::cout << "Score: " << score;

        for (int i = 0; i < itemCount; i++) {
            int y = H / 2 + i * 2;
            Renderer::setColor(7);
            Renderer::gotoXY(W / 2 - 6, y);
            if (i == selected) {
                Renderer::setColor(10); std::cout << ">> ";
                Renderer::setColor(15); std::cout << items[i];
            }
            else {
                std::cout << "   " << items[i];
            }
        }


        int key = _getch();
        if (key == 224 || key == 0) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + itemCount) % itemCount;
            else if (key == 80) selected = (selected + 1) % itemCount;
        }
        else if (key == 'w' || key == 'W') {
            selected = (selected - 1 + itemCount) % itemCount;
        }
        else if (key == 's' || key == 'S') {
            selected = (selected + 1) % itemCount;
        }
        else if (key == 13) {
            return selected;
        }
    }
}
