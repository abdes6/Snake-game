#include "Menu.h"
#include "Renderer.h"
#include <conio.h>
#include <cstring>
#include <iostream>

int Menu::show(int& difficulty) {
    int selected = 0;
    int itemCount = 4;
    const char* itemLabels[] = { "Start Game", "", "How to Play", "Exit" };
    const char* diffLabels[] = { "Easy", "Medium", "Hard" };
    while (true) {
        system("cls");
        Renderer::hideCursor();
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

            const char* label;
            char buf[32];
            if (i == 1) {
                sprintf_s(buf, "Difficulty: %s", diffLabels[difficulty]);
                label = buf;
            } else {
                label = itemLabels[i];
            }

            if (i == selected) {
                Renderer::setColor(10); std::cout << ">> ";
                Renderer::setColor(14); std::cout << label;
            } else {
                std::cout << "   " << label;
            }
        }


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
            if (selected == 1) {
                difficulty = (difficulty + 1) % 3;
            } else {
                if (selected == 0) return 0;
                if (selected == 2) return 1;
                if (selected == 3) return 2;
            }
        } else if (key == 27) {
            return 2;
        }
    }
}

void Menu::showHowToPlay() {
    system("cls");
    Renderer::hideCursor();
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
    Renderer::gotoXY(W / 2 - 12, 14); std::cout << "*  Hit obstacle (#) = Game Over";
    Renderer::gotoXY(W / 2 - 12, 15); std::cout << "*  Can't reverse direction";

    Renderer::setColor(8);
    Renderer::gotoXY(W / 2 - 14, H - 3); std::cout << "ESC = Menu    Enter = Start Game";

    while (true) {
        int key = _getch();
        if (key == 27) return;
        if (key == 13) return;
    }
}
