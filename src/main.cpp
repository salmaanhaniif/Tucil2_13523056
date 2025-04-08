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

// #define GIF_IMPLEMENTATION
#include "gif.h"

std::vector<unsigned char> load_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main(int argc, char const *argv[])
{
    string imagepathIn;
    int emd;
    double threshold;
    int minSize;
    string imagepathOut;
    string gifOut;

    cout << "Welcome to Image Compressor by QuadTree" << endl << endl;
    cout << "Tuliskan filepath dari Image (input): "; cin >> imagepathIn;
    cout << "Error Measurement Method" << endl << "1.Variance" << endl << "2.MAD" << endl << "3.MPD" << endl << "4.Entropy" << endl;
    cout << "Choose one (number): "; cin >> emd;
    cout << "Tentukan Nilai Threshold: "; cin >> threshold;
    cout << "Tentukan Area Minimum Block: ";cin >> minSize;
    cout << "Tuliskan filepath untuk Hasil: "; cin >> imagepathOut;
    cout << "Tuliskan filepath untuk Gif: "; cin >> gifOut;
    cout << endl;

    auto start = chrono::high_resolution_clock::now();

    cout << "Processing..." << endl << endl;
    QuadTree* tree = QuadTree::buildFromImage(imagepathIn.c_str());
    // Dapatkan dimensi gambar dari block root
    int imgWidth = tree->getWidth();
    int imgHeight = tree->getHeight();

    Pixel** canvas = new Pixel*[imgHeight];
    for (int i = 0; i < imgHeight; i++) {
        canvas[i] = new Pixel[imgWidth];
    }
    // Inisialisasi canvas dengan intensitas asli dari tree->block
    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < imgWidth; x++) {
            canvas[y][x] = tree->getIntensity(Point(x,y));
        }
    }

    // Inisialisasi GIFWriter (menggunakan gif.h)
    GifWriter gif;
    // Mulai GIF dengan filename outputGIF, lebar, tinggi, dan delay frame (misalnya 10 = 100ms per frame)
    if (!GifBegin(&gif, gifOut.c_str(), imgWidth, imgHeight, 10)) {
        std::cerr << "Error: Gagal menginisialisasi GIF.\n";
        return 1;
    }

    // Jalankan proses kompresi dengan memasukkan pointer ke canvas dan gif
    tree->mainProcess(1, threshold, minSize, canvas, imgWidth, imgHeight, &gif);

    // (Opsional) Pastikan canvas final sudah penuh terisi
    tree->fillCanvas(canvas);
    
    // Tambahkan frame final ke GIF
    // Misalnya: captureFrame() sudah dipanggil di mainProcess, jadi tidak perlu dipanggil lagi.
    
    // Selesaikan GIF
    GifEnd(&gif);

    int treeDepth = tree->getDepth();
    int nodesCount = tree->countNodes();
    tree->exportToPNG(imagepathOut.c_str());
    for (int i = 0; i < imgHeight; i++) {
        delete[] canvas[i];
    }
    delete[] canvas;

    auto end = chrono::high_resolution_clock::now();
    auto exec_time = chrono::duration<double>(end-start).count();

    int widthIn, heightIn, channelsIn;
    int widthOut, heightOut, channelsOut;
    auto bufferIn = load_file(imagepathIn);
    auto bufferOut = load_file(imagepathOut);
    stbi_info_from_memory(bufferIn.data(), bufferIn.size(), &widthIn, &heightIn, &channelsIn);
    stbi_info_from_memory(bufferOut.data(), bufferOut.size(), &widthOut, &heightOut, &channelsOut);
    uintmax_t sizeIn = filesystem::file_size(imagepathIn);
    uintmax_t sizeOut = filesystem::file_size(imagepathOut);
    double cp = (1 - (int)sizeOut/(int)sizeIn) * 100;


    cout << "Compressing Complete!" << endl;
    cout << "DETAILS: " << endl;
    cout << "Execution Time : " << exec_time*1000 << " ms"<<endl;
    cout << "Size before : " << sizeIn/1000 << " kb, " << widthIn << "x" << heightIn << "pixels" << endl;
    cout << "Size after : " << sizeOut/1000 << " kb, " << widthOut << "x" << heightOut << "pixels" << endl;
    cout << "Compression Percentage : " << cp << " %" << endl;
    cout << "Tree's Depth : " << treeDepth << endl;
    cout << "Nodes Count : " << nodesCount << endl;
    cout << "Result Path : " << imagepathOut << endl;
    return 0;
}