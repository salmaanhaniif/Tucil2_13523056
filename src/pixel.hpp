#include "iostream"
#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
private:
    unsigned char red, green, blue;
public:
    Pixel();
    Pixel(int r, int g, int b);
    ~Pixel();
    int getColour(int colourcode);
    void displayPixel();
};
#endif