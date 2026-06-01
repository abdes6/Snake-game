#include <cstdlib>
#include <ctime>
#include <iostream>
#include <Windows.h>

#include "Game.h"
#include "Menu.h"
#include "Renderer.h"


int main() {
    Game game;
    Menu menu;
    game.init();

    while (true) {
        int menuChoice = menu.show();
        if (menuChoice == 1) {
            menu.showHowToPlay();
            continue;
        }
        if (menuChoice == 2) break;

        while (true) {
            int result = game.run();
            if (result == 1) break;
        }
    }

    system("cls");
    Renderer::setColor(7);
    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}
