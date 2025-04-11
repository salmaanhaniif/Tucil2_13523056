#include "iostream"
#include "block.hpp"
#include "pixel.hpp"
#include "point.hpp"
#include <iostream>
#include <fstream>

#ifndef QUADTREE_H
#define QUADTREE_H

// Class QuadTree digunakan untuk merepresentasikan struktur data Quadtree
// yang berfungsi untuk kompresi gambar berbasis pembagian area menjadi blok-blok lebih kecil.

class QuadTree
{
private:
    // Koordinat posisi pojok kiri atas dari blok ini dalam gambar
    Point position;

    // Blok gambar yang direpresentasikan oleh node Quadtree ini
    Block block;

    // Array pointer ke anak-anak (sub-blok) Quadtree (maksimal 4 anak)
    QuadTree **child;

    // Maksimal anak dari setiap node Quadtree (tetap 4 sesuai konsep Quadtree)
    const int maxChild = 4;

    // Rata-rata warna pixel dari blok (dipakai kalau blok jadi leaf / tidak dibagi lagi)
    Pixel average;

public:
    // Konstruktor
    // Membuat node Quadtree dengan posisi dan blok tertentu
    QuadTree(Point pos, Block sz);

    // Membuat Quadtree dari sebuah file gambar
    static QuadTree *buildFromImage(const char *filename);

    // Proses utama kompresi Quadtree
    // emd -> metode perhitungan homogenitas (1: Variance, 2: MAD, 3: MPD, 4: Entropy)
    // threshold -> batas homogenitas
    // minSize -> ukuran minimum blok untuk bisa dibagi
    void mainProcess(int emd, double threshold, int minSize);

    // Destructor
    // Menghapus semua alokasi memori dari anak-anak Quadtree
    ~QuadTree();

    // Menyimpan hasil Quadtree ke file
    void saveToImage(std::ofstream &out);

    // Membaca dan membangun kembali Quadtree dari file
    static QuadTree *loadFromImage(std::ifstream &in);

    // Mengisi canvas kosong dengan warna dari Quadtree (untuk rebuild image hasil kompresi)
    void fillCanvas(Pixel **canvas);

    // Membuat Quadtree dummy untuk testing (tanpa file image)
    QuadTree *buildDummyTree(int width, int height);

    // Menampilkan struktur Quadtree (biasanya untuk debugging)
    void printQuadTree();

    // Mengembalikan kedalaman (depth) dari Quadtree
    int getDepth();

    // Menghitung jumlah node total dari Quadtree
    int countNodes();

    // Mengembalikan lebar dari blok Quadtree saat ini
    int getWidth();

    // Mengembalikan tinggi dari blok Quadtree saat ini
    int getHeight();
};

#endif