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

int Pixel::getColour(int colourcode) {
    if (colourcode == 0) { 
        return (int) red;
    } else if (colourcode == 1) {
        return (int) green;
    } else {return (int) blue;}
}

void Pixel::setColour(int colourCode, double value) {
    if (colourCode = 0) {red = (unsigned char) value;}
    else if (colourCode = 1) {green = (unsigned char) value;}
    else {blue = (unsigned char) value;}
}

Pixel::~Pixel()
{
}

void Pixel::displayPixel() {
    std::cout << "(" << (int) red << "," << (int) green << "," << (int) blue << ")";
}