#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "quadtree.hpp"
#include <string>

using namespace std;

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::vector<unsigned char> load_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main(int argc, char const *argv[])
{
    // Input dari user:
    string imagepathIn;
    int emd;
    double threshold;
    int minSize;
    string imagepathOut;
    
    cout << endl << "~Welcome to Image Compressor by QuadTree~" << endl << endl;
    cout << "Tuliskan filepath dari Image (input): ";
    cin >> imagepathIn;

    cout << "Error Measurement Method" << endl;
    cout << "1. Variance" << endl;
    cout << "2. Mean Absolute Deviation" << endl;
    cout << "3. Max Pixel Difference" << endl;
    cout << "4. Entropy" << endl;
    cout << "Choose one (number): ";
    cin >> emd;
    cout << "Tentukan Nilai Threshold: ";
    cin >> threshold;
    cout << "Tentukan Area Minimum Block: ";
    cin >> minSize;
    cout << "Tuliskan filepath untuk hasil rekonstruksi (output image): ";
    cin >> imagepathOut;
    cout << endl;
    
    auto start = chrono::high_resolution_clock::now();
    
    cout << "Processing..." << endl << endl;
    // 1. Membangun quadtree dari gambar menggunakan fungsi impor buildFromImage
    QuadTree* tree = QuadTree::buildFromImage(imagepathIn.c_str());
    if(tree == nullptr){
        cerr << "Gagal membangun QuadTree dari gambar!" << endl;
        return 1;
    }
    
    // 2. Melakukan proses kompresi dengan subdivisi (sesuai error measurement, threshold, dan minimum block)
    tree->mainProcess(emd, threshold, minSize);
    
    // 3. Serialisasi struktur quadtree ke file biner (misalnya "compressed.quadtree")
    ofstream outFile("compressed.quadtree", ios::binary);
    if (!outFile) {
        cerr << "Gagal membuka file untuk penulisan quadtree!" << endl;
        return 1;
    }
    tree->saveToImage(outFile);
    outFile.close();
    
    // 4. Deserialisasi quadtree dari file untuk merekonstruksi gambar
    ifstream inFile("compressed.quadtree", ios::binary);
    if (!inFile) {
        cerr << "Gagal membuka file untuk pembacaan quadtree!" << endl;
        return 1;
    }
    QuadTree* loadedTree = QuadTree::loadFromImage(inFile);
    inFile.close();
    
    // 5. Rekonstruksi canvas dari quadtree yang dideserialisasi
    int width = loadedTree->getWidth();
    int height = loadedTree->getHeight();
    
    // Alokasi canvas sesuai ukuran gambar
    Pixel** canvas = new Pixel*[height];
    for (int i = 0; i < height; i++) {
        canvas[i] = new Pixel[width];
    }
    // Inisialisasi canvas dengan warna default (misal hitam)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            canvas[y][x] = Pixel(0,0,0);
        }
    }
    loadedTree->fillCanvas(canvas);
    
    // 6. (Opsional) Export canvas hasil rekonstruksi ke file gambar
    // Jika Anda menggunakan library seperti stb_image_write, Anda bisa membuat buffer pixel dan memanggil fungsi write
    // Contoh: membuat buffer dan memanggil stbi_write_png:
    unsigned char* imgData = new unsigned char[width * height * 3];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            imgData[idx]     = canvas[y][x].getColour(0);
            imgData[idx + 1] = canvas[y][x].getColour(1);
            imgData[idx + 2] = canvas[y][x].getColour(2);
        }
    }
    // Contoh pemanggilan stbi_write_png (pastikan sudah include header yang diperlukan)
    stbi_write_png(imagepathOut.c_str(), width, height, 3, imgData, width * 3);
    
    // 7. Menghitung waktu eksekusi dan perbandingan ukuran file
    auto end = chrono::high_resolution_clock::now();
    auto exec_time = chrono::duration<double>(end-start).count();
    
    auto bufferIn = load_file(imagepathIn);
    auto bufferOut = load_file(imagepathOut);
    int widthIn, heightIn, channelsIn;
    int widthOut, heightOut, channelsOut;
    stbi_info_from_memory(bufferIn.data(), bufferIn.size(), &widthIn, &heightIn, &channelsIn);
    stbi_info_from_memory(bufferOut.data(), bufferOut.size(), &widthOut, &heightOut, &channelsOut);
    uintmax_t sizeIn = filesystem::file_size(imagepathIn);
    uintmax_t sizeOut = filesystem::file_size(imagepathOut);
    double cp = (1 - (double)sizeOut/(double)sizeIn) * 100;
    
    cout << "\nCompressing Complete!" << endl;
    cout << "DETAILS:" << endl;
    cout << "Execution Time       : " << exec_time*1000 << " ms" << endl;
    cout << "Size before (input)  : " << sizeIn/1000 << " kb, " << widthIn << "x" << heightIn << " pixels" << endl;
    cout << "Size after (output)  : " << sizeOut/1000 << " kb, " << widthOut << "x" << heightOut << " pixels" << endl;
    cout << "Compression Percent  : " << fixed << setprecision(3) << cp << " %" << endl;
    cout << "Tree's Depth         : " << tree->getDepth() << endl;     // Pastikan fungsi getDepth() sudah diimplementasikan
    cout << "Nodes Count          : " << tree->countNodes() << endl;     // Pastikan fungsi countNodes() sudah diimplementasikan
    cout << "Result Path          : " << imagepathOut << endl;
    
    // 8. Cleanup memori
    for (int i = 0; i < height; i++) {
        delete[] canvas[i];
    }
    delete[] canvas;
    delete[] imgData;
    delete tree;
    delete loadedTree;
    
    return 0;
}