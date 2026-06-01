#pragma once

constexpr int H = 27;
constexpr int W = 60;

struct Pos {
    int x;
    int y;
};

enum class BlockType {
    EMPTY = 0,
    FOOD = 1,
};

