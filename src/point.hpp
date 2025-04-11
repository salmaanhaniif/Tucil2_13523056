#include "iostream"
#ifndef POINT_H
#define POINT_H

// Class Point merepresentasikan sebuah titik/koordinat (x, y) di dalam citra atau bidang 2D
class Point
{
private:
    // Menyimpan posisi horizontal (x) dan vertikal (y)
    int x;
    int y;

public:
    // Konstruktor default
    // Menginisialisasi titik ke koordinat (0, 0)
    Point();

    // Konstruktor dengan parameter
    // Menginisialisasi titik ke koordinat (x, y) sesuai input
    Point(int x, int y);

    // Destructor
    ~Point();

    // Mengembalikan nilai posisi x dari titik
    int getX();

    // Mengembalikan nilai posisi y dari titik
    int getY();

    // Menampilkan posisi titik dalam format (x, y)
    // Biasanya digunakan untuk debugging
    void displayPosition();
};

#endif
