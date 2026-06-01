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
    const int moveFrequency = 200;

    map.init();
    snake.init();
    map.draw();
    snake.draw();

    while (true) {
        if (_kbhit()) {
            switch (_getch()) {
            case 'w': if (snake.getDir() != 2) snake.setDir(0); break;
            case 'd': if (snake.getDir() != 3) snake.setDir(1); break;
            case 's': if (snake.getDir() != 0) snake.setDir(2); break;
            case 'a': if (snake.getDir() != 1) snake.setDir(3); break;
            }
        }

        int cur = GetTickCount();
        if (cur - lastMoveTime > moveFrequency) {
            Pos tail = snake.getTail();
            Renderer::drawUnit(tail, " ");

            snake.move();

            Pos head = snake.getHeadPos();
            if (head.x <= 0 || head.x >= W || head.y <= 0 || head.y >= H) {
                break;
            }

            if (map.isFoodAt(head.x, head.y)) {
                snake.grow(tail);
                map.removeFood(head.x, head.y);
                Renderer::drawUnit(tail, "O");
                score++;
            }

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
