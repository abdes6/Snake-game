#include "Map.h"
#include "Snake.h"
#include "Renderer.h"
#include <cstdlib>
#include <iostream>

void Map::init() {
    hasFood = false;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            data[y][x] = BlockType::EMPTY;
        }
    }
}

void Map::draw() {
    system("cls");
    std::cout << "+";
    for (int i = 0; i < W; i++) std::cout << "-";
    std::cout << "+" << std::endl;
    for (int i = 0; i < H; i++) {
        std::cout << "|";
        for (int j = 0; j < W; j++) std::cout << " ";
        std::cout << "|" << std::endl;
    }
    std::cout << "+";
    for (int i = 0; i < W; i++) std::cout << "-";
    std::cout << "+" << std::endl;
}

void Map::generateFood(const Snake& snake) {
    if (hasFood) return;
    while (true) {
        int x = rand() % W;
        int y = rand() % H;
        bool occupied = false;
        for (int i = 0; i < snake.getLength(); i++) {
            if (x == snake.getBody()[i].x && y == snake.getBody()[i].y) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
            data[y][x] = BlockType::FOOD;
            hasFood = true;
            Renderer::drawUnit({ x, y }, "*");
            return;
        }
    }
}

bool Map::isFoodAt(int x, int y) const {
    return data[y][x] == BlockType::FOOD;
}

void Map::removeFood(int x, int y) {
    data[y][x] = BlockType::EMPTY;
    hasFood = false;
}
