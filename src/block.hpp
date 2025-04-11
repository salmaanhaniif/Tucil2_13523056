#include "iostream"
#include "pixel.hpp"
#include "point.hpp"
#ifndef BLOCK_H
#define BLOCK_H

// Class Block merepresentasikan sebuah blok atau area persegi panjang dalam gambar
// Digunakan sebagai elemen dasar dalam proses Quadtree compression

class Block
{
private:
    // Lebar blok (jumlah kolom)
    int width;

    // Tinggi blok (jumlah baris)
    int height;

    // Matriks 2D yang menyimpan nilai warna (Pixel) untuk tiap posisi dalam blok
    Pixel** intensity;

public:
    // Konstruktor default
    Block();

    // Konstruktor dengan parameter
    // Membuat blok dengan ukuran (width x height)
    Block(int w, int h);

    // Copy constructor
    // Digunakan untuk menyalin isi blok dari blok lain
    Block(const Block& other);

    // Operator assignment
    // Meng-assign blok lain ke blok ini
    Block& operator=(const Block& other);

    // Destructor
    // Menghapus alokasi memori dari matriks intensity
    ~Block();

    // Mengembalikan lebar blok
    int getWidth();

    // Mengembalikan tinggi blok
    int getHeight();

    // Mengembalikan luas area blok (width * height)
    int getArea();

    // Menampilkan isi blok (biasanya untuk debugging)
    void displayBlock();

    // Mengembalikan matriks 2D intensity dari blok
    Pixel** getBlockIntensity() const;

    // Mengambil sub-block dari posisi tertentu (startPoint) dengan ukuran (w x h)
    // Digunakan saat proses pembagian (divide) Quadtree
    Block* getSubBlock(Point startPoint, int w, int h);

    // Mengambil pixel di posisi tertentu
    Pixel getIntensity(Point pos) const;

    // Mengatur nilai pixel di posisi tertentu
    void setIntensity(Point pos, const Pixel& pixel);

    // Mengisi seluruh blok dengan warna tertentu
    void fillWithColor(const Pixel &pix);

    // Melakukan normalisasi nilai warna di blok (jika diperlukan untuk preprocessing)
    void normalise();

    // Menghitung rata-rata nilai warna untuk channel tertentu (0:Red, 1:Green, 2:Blue)
    double getAverage(int colourCode);

    // Menghitung rata-rata warna dari seluruh blok (hasilnya berupa Pixel RGB)
    Pixel calculateAverageColor();

    // Menghitung variance dari seluruh blok (ukuran ketidakhomogenan)
    double calculateVariance();

    // Fungsi helper untuk calculateVariance per channel warna
    double calculateVarianceHelper(int colourCode);

    // Menghitung Mean Absolute Deviation (MAD) dari blok
    double calculateMAD();

    // Fungsi helper untuk calculateMAD per channel warna
    double calculateMADHelper(int colourCode);

    // Menghitung Mean Pixel Difference (MPD) dari blok
    double calculateMPD();

    // Fungsi helper untuk calculateMPD per channel warna
    std::pair<int, int> calculateMPDHelper(int colourCode);

    // Menghitung Entropy dari blok (ukuran kompleksitas pola warna)
    double calculateEntropy();

    // Fungsi helper untuk calculateEntropy per channel warna
    double calculateEntropyHelper(int colourCode);
};

#endif