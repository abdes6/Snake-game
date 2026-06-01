#pragma once
#include "Common.h"

class Snake {
public:
    void init();
    void draw() const;
    void move();
    void setDir(int newDir);
    int getDir() const;
    Pos getHeadPos() const;
    Pos getTail() const;
    const Pos* getBody() const;
    int getLength() const;
    void grow(Pos tail);

private:
    Pos body[H * W];
    int dir;
    int length;
};
