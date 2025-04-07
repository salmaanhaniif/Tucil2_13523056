#include <iostream>
#include "block.hpp"

Block::Block()
{
    width = 0;
    height = 0;
}

Block::Block(int w, int h)
{
    width = w;
    height = h;
}

Block::~Block()
{
}

int Block::getArea() {
    return width * height;
}

void Block::displayBlock() {
    std::cout << "Lebar: " << width << " Tinggi: " << height << " Luas: " << width*height << std::endl;
}