#include "Menu.h"
#include "Renderer.h"
#include <conio.h>
#include <cstring>
#include <iostream>

int Menu::show() {
    int selected = 0;
    const char* items[] = { "Start Game", "How to Play", "Exit" };
    int itemCount = 3;

    while (true) {
        system("cls");
        Renderer::setColor(7);

        Renderer::gotoXY(0, 0); std::cout << "+";
        for (int i = 0; i < W; i++) std::cout << "-";
        std::cout << "+";

        for (int i = 1; i < H; i++) {
            Renderer::gotoXY(0, i); std::cout << "|";
            Renderer::gotoXY(W + 1, i); std::cout << "|";
        }

        Renderer::gotoXY(0, H); std::cout << "+";
        for (int i = 0; i < W; i++) std::cout << "-";
        std::cout << "+";

        const char* title = "S N A K E   G A M E";
        int titleLen = strlen(title);
        Renderer::setColor(11); Renderer::gotoXY((W - titleLen) / 2 + 1, 4); std::cout << title;

        const char* sub = "C L A S S I C   S N A K E";
        int subLen = strlen(sub);
        Renderer::setColor(8); Renderer::gotoXY((W - subLen) / 2 + 1, 6); std::cout << sub;

        for (int i = 0; i < itemCount; i++) {
            int y = 10 + i * 3;
            Renderer::setColor(7);
            Renderer::gotoXY(W / 2 - 8, y);
            if (i == selected) {
                Renderer::setColor(10); std::cout << ">> ";
                Renderer::setColor(14); std::cout << items[i];
            } else {
                std::cout << "   " << items[i];
            }
        }

        Renderer::setColor(8);
        Renderer::gotoXY(W / 2 - 18, H - 3); std::cout << "W S  Select  Enter Confirm  ESC Exit";

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
            return 2;
        }
    }
}

void Menu::showHowToPlay() {
    system("cls");
    for (int i = 0; i <= H; i++) {
        Renderer::gotoXY(0, i); std::cout << "|";
        Renderer::gotoXY(W + 1, i); std::cout << "|";
    }
    Renderer::gotoXY(0, 0); std::cout << "+";
    for (int i = 0; i < W; i++) std::cout << "-";
    std::cout << "+";
    Renderer::gotoXY(0, H); std::cout << "+";
    for (int i = 0; i < W; i++) std::cout << "-";
    std::cout << "+";

    Renderer::setColor(14);
    Renderer::gotoXY(W / 2 - 6, 3); std::cout << "H O W   T O   P L A Y";

    Renderer::setColor(7);
    Renderer::gotoXY(W / 2 - 12, 6); std::cout << "W / Up      S / Down";
    Renderer::gotoXY(W / 2 - 12, 7); std::cout << "A / Left    D / Right";

    Renderer::setColor(14);
    Renderer::gotoXY(W / 2 - 6, 10); std::cout << "G A M E   R U L E S";
    Renderer::setColor(7);
    Renderer::gotoXY(W / 2 - 12, 12); std::cout << "*  Eat food (*) to grow & score";
    Renderer::gotoXY(W / 2 - 12, 13); std::cout << "*  Hit wall = Game Over";
    Renderer::gotoXY(W / 2 - 12, 14); std::cout << "*  Can't reverse direction";

    Renderer::setColor(8);
    Renderer::gotoXY(W / 2 - 14, H - 3); std::cout << "ESC = Menu    Enter = Start Game";

    while (true) {
        int key = _getch();
        if (key == 27) return;
        if (key == 13) return;
    }
}
