#include <iostream>
#include "point.hpp"

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int xIn, int yIn)
{
    x = xIn;
    y = yIn;
}


Point::~Point()
{
}

void Point::displayPosition() {
    std::cout << "Posisi saat ini : (" << x << "," << y << ")." <<std::endl;
}

int Point::getX() {
    return x;
}

int Point::getY() {
    return y;
}