#include <iostream>
#include "quadtree.hpp"
#include <cmath>
#include <vector>
#include "gif.h"

#include "stb_image_write.h"
#include "stb_image.h"

QuadTree::QuadTree(Point pos, Block bl)
{
    position = pos;
    block = bl;
    child = new QuadTree *[maxChild] {nullptr};
}

// Fungsi membangun QuadTree dari image file
QuadTree* QuadTree::buildFromImage(const char* filename) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image == nullptr) {
        std::cout << "Gagal memuat gambar" << std::endl;
        return nullptr;
    }
    
    // Buat Block berdasarkan ukuran gambar
    Block blk(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            Point p(x, y);
            // Asumsikan setIntensity meng-copy pixel ke array 2D intensity
            blk.setIntensity(p, Pixel(image[index], image[index+1], image[index+2]));
        }
    }
    stbi_image_free(image);
    // Node akar berada di posisi (0,0)
    return new QuadTree(Point(0, 0), blk);
}

// Fungsi captureFrame: konversi canvas (Pixel** dengan dimensi canvasWidth x canvasHeight)
// ke buffer unsigned char dan tambahkan sebagai frame ke GIF
void QuadTree::captureFrame(Pixel** canvas, int canvasWidth, int canvasHeight, GifWriter* gif) {
    // Alokasikan buffer untuk frame (RGB)
    unsigned char* frameBuffer = new unsigned char[canvasWidth * canvasHeight * 3];
    for (int y = 0; y < canvasHeight; y++) {
        for (int x = 0; x < canvasWidth; x++) {
            int idx = (y * canvasWidth + x) * 3;
            frameBuffer[idx]     = canvas[y][x].getColour(0);
            frameBuffer[idx + 1] = canvas[y][x].getColour(1);
            frameBuffer[idx + 2] = canvas[y][x].getColour(2);
        }
    }
    // Misal, panggil fungsi dari library GIF untuk menambahkan frame.
    // Contoh (sesuaikan dengan API library GIF yang kamu gunakan):
    // GifWriteFrame(gif, frameBuffer, canvasWidth, canvasHeight, canvasWidth * 3, delay);
    
    // Contoh dummy:
    // appendFrameToGIF(gif, frameBuffer, canvasWidth, canvasHeight, 10); // delay 10 centisecond

    delete[] frameBuffer;
}

// Fungsi mainProcess yang diperbarui:  
// - Jika error rendah (atau area sudah kecil) → node menjadi leaf, dan blok diisi dengan warna rata-rata.
// - Setiap kali node leaf dihasilkan, update canvas (area sesuai posisi dan ukuran block) dan capture frame.
void QuadTree::mainProcess(int emd, double threshold, int minblock, 
                           Pixel** canvas, int canvasWidth, int canvasHeight, GifWriter* gif) {
    double error;
    switch (emd) {
        case 1: error = block.calculateVariance(); break;
        case 2: error = block.calculateMAD(); break;
        case 3: error = block.calculateMPD(); break;
        default: error = block.calculateEntropy(); break;
    }
    
    if (error < threshold || block.getArea() < minblock || (block.getArea()/4) < minblock) {
        // Hitung warna rata-rata untuk block ini
        averageColor = block.calculateAverageColor();
        // Normalisasi block: isi seluruh intensitas block dengan averageColor
        block.normalise(); // Pastikan di dalam normalise() block diisi dengan averageColor,
                           // atau bisa juga manual:
        // for (int y = 0; y < block.getHeight(); y++) {
        //     for (int x = 0; x < block.getWidth(); x++) {
        //         block.setIntensity(Point(x,y), averageColor);
        //     }
        // }
        // Update kanvas pada area block ini
        for (int y = 0; y < block.getHeight(); y++) {
            for (int x = 0; x < block.getWidth(); x++) {
                int canvasY = position.getY() + y;
                int canvasX = position.getX() + x;
                if (canvasY < canvasHeight && canvasX < canvasWidth) {
                    canvas[canvasY][canvasX] = averageColor;
                }
            }
        }
        // Capture frame untuk GIF
        captureFrame(canvas, canvasWidth, canvasHeight, gif);
        return;
    } else {
        int midWidth = block.getWidth() / 2;
        int midHeight = block.getHeight() / 2;
        
        int w0 = midWidth;
        int w1 = (block.getWidth() % 2 == 0) ? midWidth : midWidth + 1;
        int h0 = midHeight;
        int h1 = (block.getHeight() % 2 == 0) ? midHeight : midHeight + 1;
        
        // Hitung posisi untuk setiap kuadran berdasarkan posisi global
        Point pos1(position.getX(), position.getY());
        Point pos2(position.getX() + midWidth, position.getY());
        Point pos3(position.getX(), position.getY() + midHeight);
        Point pos4(position.getX() + midWidth, position.getY() + midHeight);
        
        // Dapatkan sub-block dengan koordinat relatif (dalam block)
        Block* b0 = block.getSubBlock(Point(0,0), w0, h0);
        Block* b1 = block.getSubBlock(Point(midWidth, 0), w1, h0);
        Block* b2 = block.getSubBlock(Point(0, midHeight), w0, h1);
        Block* b3 = block.getSubBlock(Point(midWidth, midHeight), w1, h1);
        
        // Buat child QuadTree untuk keempat kuadran
        child[0] = new QuadTree(pos1, *b0);
        child[1] = new QuadTree(pos2, *b1);
        child[2] = new QuadTree(pos3, *b2);
        child[3] = new QuadTree(pos4, *b3);
        
        delete b0;
        delete b1;
        delete b2;
        delete b3;
        
        // Proses rekursif untuk setiap child, meneruskan canvas dan pointer GIF
        for (int i = 0; i < maxChild; i++) {
            child[i]->mainProcess(emd, threshold, minblock, canvas, canvasWidth, canvasHeight, gif);
        }
        // Opsional: capture frame setelah seluruh subdivisi pada node ini selesai
        captureFrame(canvas, canvasWidth, canvasHeight, gif);
    }
}

// Fungsi untuk me-*render* gambar terkompres ke canvas berdasarkan QuadTree akhir
void QuadTree::fillCompressedImage(Pixel** outputImage) {
    if (child[0] == nullptr) {  // Leaf node
        for (int y = 0; y < block.getHeight(); y++) {
            for (int x = 0; x < block.getWidth(); x++) {
                Point p(x, y);
                outputImage[position.getY() + y][position.getX() + x] = block.getIntensity(p);
            }
        }
    } else {
        for (int i = 0; i < maxChild; i++) {
            if(child[i])
                child[i]->fillCompressedImage(outputImage);
        }
    }
}

// Fungsi untuk finalisasi canvas (jika diperlukan) dengan menyalin seluruh intensitas block dari QuadTree
void QuadTree::fillCanvas(Pixel** canvas) {
    fillCompressedImage(canvas);
}

// Fungsi export ke PNG (hasil kompresi final)
void QuadTree::exportToPNG(const char* filename) {
    int width = block.getWidth();
    int height = block.getHeight();
    unsigned char* imgData = new unsigned char[width * height * 3];
    Pixel** outputImage = new Pixel*[height];
    for (int i = 0; i < height; i++) {
        outputImage[i] = new Pixel[width];
    }
    
    fillCompressedImage(outputImage);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            imgData[idx]     = outputImage[y][x].getColour(0);
            imgData[idx + 1] = outputImage[y][x].getColour(1);
            imgData[idx + 2] = outputImage[y][x].getColour(2);
        }
    }
    
    stbi_write_png(filename, width, height, 3, imgData, width * 3);
    
    for (int i = 0; i < height; i++) {
        delete[] outputImage[i];
    }
    delete[] outputImage;
    delete[] imgData;
}

void QuadTree::printQuadTree()
{
    position.displayPosition();
    block.displayBlock();

    std::cout << "[";
    for (int y = 0; y < block.getHeight(); y++)
    {
        for (int x = 0; x < block.getWidth(); x++)
        {
            // block.intensity[y][x].displayPixel();
            Point temp = Point(x,y);
            block.getIntensity(temp).displayPixel();
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "]" << std::endl;
}

QuadTree::~QuadTree()
{
    for (int i = 0; i < maxChild; i++)
    {
        delete child[i];
    }
    delete[] child;
}

int QuadTree::getDepth() {
    int depth = 0;
    if (child == nullptr) return 1; // leaf node

    for (int i = 0; i < maxChild; i++) {
        if (child[i] != nullptr) {
            depth = std::max(depth, child[i]->getDepth());
        }
    }
    return depth + 1;
}

int QuadTree::countNodes() {
    int cnt = 1; // hitung diri sendiri
    if (child == nullptr) return cnt;

    for (int i = 0; i < maxChild; i++) {
        if (child[i] != nullptr) {
            cnt += child[i]->countNodes();
        }
    }
    return cnt;
}

int QuadTree::getWidth() {
    return block.getWidth();
}

int QuadTree::getHeight() {
    return block.getHeight();
}

Pixel QuadTree::getIntensity(Point pos) {
    return block.getIntensity(pos);
}

// int main(int argc, char const *argv[])
// {
//     QuadTree *tes = QuadTree::buildFromImage("../test/monyet.jpg");
//     tes->printQuadTree();
//     // std::cout << "Variance: " <<  << ", MAD: " << tes->calculateMAD() << ", MPD: " << tes->calculateMPD() << ", Entropy: " << tes->calculateEntropy() << std::endl;
//     return 0;
// }
