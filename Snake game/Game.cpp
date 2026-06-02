#include "Game.h"
#include "Map.h"
#include "Snake.h"
#include "Renderer.h"
#include <conio.h>
#include <iostream>
#include <Windows.h>

void Game::init() {
    Renderer::hideCursor();
}

int Game::run() {
    Map map;
    Snake snake;
    int score = 0;
    int lastMoveTime = 0;
    int baseMoveFrequency = 200;
    int moveFrequency = baseMoveFrequency;

    bool sprinting = false;
    int sprintStartTime = 0;
    int lastSprintEndTime = -5000;
    const int sprintDuration = 1000;
    const int sprintCooldown = 3000;

    map.init();
    snake.init();
    map.draw();
    Renderer::setColor(10);
    snake.draw();
    Renderer::gotoXY(2, H + 2);
    Renderer::setColor(14);
    std::cout << "Score: " << score;
    Renderer::setColor(8);
    Renderer::gotoXY(20, H + 2);
    std::cout << "P = Pause";

    bool paused = false;

    while (true) {
        int cur = GetTickCount();

        if (sprinting && cur - sprintStartTime > sprintDuration) {
            sprinting = false;
            lastSprintEndTime = cur;
            moveFrequency = baseMoveFrequency;
        }

        if (_kbhit()) {
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
                continue;
            }
            if (!paused) {
                if (key >= 'A' && key <= 'Z') key += 32;
                switch (key) {
                case 'w': if (snake.getDir() != 2) snake.setDir(0); break;
                case 'd': if (snake.getDir() != 3) snake.setDir(1); break;
                case 's': if (snake.getDir() != 0) snake.setDir(2); break;
                case 'a': if (snake.getDir() != 1) snake.setDir(3); break;
                case 'e':
                    if (!sprinting && cur - lastSprintEndTime > sprintCooldown) {
                        sprinting = true;
                        sprintStartTime = cur;
                        moveFrequency = 50;
                    }
                    break;
                }
            }
        }

        if (paused) {
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

        if (cur - lastMoveTime > moveFrequency) {
            Pos tail = snake.getTail();
            Renderer::setColor(0);
            Renderer::drawUnit(tail, " ");

            snake.move();

            if (snake.checkSelfCollision()) {
                break;
            }

            Pos head = snake.getHeadPos();
            if (head.x <= 0 || head.x >= W || head.y <= 0 || head.y >= H) {
                break;
            }

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
                    moveFrequency = sprinting ? max(40, baseMoveFrequency - 60) : baseMoveFrequency;
                }
            }

            Renderer::setColor(10);
            Renderer::drawUnit(head, "O");
            lastMoveTime = cur;
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

        Renderer::setColor(8);
        Renderer::gotoXY(W / 2 - 12, H / 2 + 5); std::cout << "W S  Select  Enter Confirm";

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
