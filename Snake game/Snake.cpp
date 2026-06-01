#include "Snake.h"
#include "Renderer.h"
#include "Common.h"

static const int dirOffset[4][2] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1},
};

void Snake::init() {
    length = 3;
    dir = 1;
    body[0] = { W / 2, H / 2 };
    body[1] = { W / 2 - 1, H / 2 };
    body[2] = { W / 2 - 2, H / 2 };
}

void Snake::draw() const {
    for (int i = 0; i < length; i++) {
        Renderer::drawUnit(body[i], "O");
    }
}

void Snake::move() {
    for (int i = length - 1; i >= 1; i--) {
        body[i] = body[i - 1];
    }
    body[0].x += dirOffset[dir][1];
    body[0].y += dirOffset[dir][0];
}

void Snake::setDir(int newDir) {
    dir = newDir;
}

int Snake::getDir() const {
    return dir;
}

Pos Snake::getHeadPos() const {
    return body[0];
}

Pos Snake::getTail() const {
    return body[length - 1];
}

const Pos* Snake::getBody() const {
    return body;
}

int Snake::getLength() const {
    return length;
}

void Snake::grow(Pos tail) {
    body[length++] = tail;
}
