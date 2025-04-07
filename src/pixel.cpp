#include <iostream>
#include "pixel.hpp"

Pixel::Pixel() {
    red = 0;
    green = 0;
    blue = 0;
}

Pixel::Pixel(int r, int g, int b)
{
    red = r;
    green = g;
    blue = b;
}

Pixel::~Pixel()
{
}

void Pixel::displayPixel() {
    std::cout << "(" << (int) red << "," << (int) green << "," << (int) blue << ")";
}