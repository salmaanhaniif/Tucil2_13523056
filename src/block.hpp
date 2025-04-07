#include "iostream"
#ifndef BLOCK_H
#define BLOCK_H

class Block
{
private:
    int width;
    int height;
public:
    Block();
    Block(int w, int h);
    ~Block();
    int getArea();
    void displayBlock();
};

#endif