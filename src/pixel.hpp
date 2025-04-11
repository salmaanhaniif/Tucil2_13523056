#include "iostream"
#ifndef PIXEL_H
#define PIXEL_H

// Class Pixel merepresentasikan sebuah pixel (titik) warna dalam citra digital
class Pixel
{
private:
    // Menyimpan nilai warna merah (red), hijau (green), dan biru (blue)
    unsigned char red, green, blue;

public:
    // Konstruktor default
    // Menginisialisasi nilai RGB pixel ke 0 (hitam)
    Pixel();

    // Konstruktor dengan parameter
    // Menginisialisasi nilai RGB pixel sesuai input
    // r = nilai Red, g = nilai Green, b = nilai Blue
    Pixel(int r, int g, int b);

    // Destructor
    // Tidak ada resource khusus yang dibebaskan
    ~Pixel();

    // Mengembalikan nilai warna pixel berdasarkan colourcode
    // colourcode: 0 = Red, 1 = Green, 2 = Blue
    int getColour(int colourcode);

    // Mengubah nilai warna pixel berdasarkan colourcode
    // value: nilai baru untuk warna tersebut
    void setColour(int colourcode, double value);

    // Menampilkan nilai warna RGB pixel
    void displayPixel();
};

#endif
