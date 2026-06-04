#pragma once
#include "Common.h"

class Snake;

class Map {
public:
    void init();
    void draw();
    void generateFood(const Snake& snake);
    bool isFoodAt(int x, int y) const;
    void removeFood(int x, int y);
    void generateObstacles(const Snake& snake, int count);
    bool isObstacleAt(int x, int y) const;

private:
    BlockType data[H][W];
    bool hasFood;
};
