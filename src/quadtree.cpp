#include <iostream>
#include "quadtree.hpp"
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

#include "stb_image_write.h"
#include "stb_image.h"

QuadTree::QuadTree(Point pos, Block bl)
{
    position = pos;
    block = bl;
    child = new QuadTree *[maxChild] {nullptr};
    // average = block.calculateAverageColor();
    average = Pixel(0,0,0);
}

QuadTree *QuadTree::buildFromImage(const char* filename)
{
    int width, height, channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);

    if (image == nullptr)
    {
        std::cout << "Gagal memuat gambar" << std::endl;
        return nullptr;
    }

    // std::cout << "Berhasil memuat gambar" << std::endl;

    QuadTree *node = new QuadTree(Point(), Block(width, height));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * channels;
            Point temp = Point(x, y);
            // node->block.intensity[y][x] = Pixel(image[index], image[index + 1], image[index + 2]);
            node->block.setIntensity(temp, Pixel(image[index], image[index + 1], image[index + 2]));
        }
    }
    // std::cout << "Image Loaded: width=" << width << ", height=" << height << ", channels=" << channels << std::endl;
    stbi_image_free(image);
    return node;
}

void QuadTree::mainProcess(int emd, double threshold, int minblock) {
    // std::cout << "Starting mainProcess" << std::endl;
    // std::cout << "Current Block Width: " << block.getWidth() << ", Height: " << block.getHeight() << std::endl;
    double variance;
    switch (emd)
    {
    case 1:
        variance = block.calculateVariance();
        break;
    case 2:
        variance = block.calculateMAD();
        break;
    case 3:
        variance = block.calculateMPD();
        break;
    default:
        variance = block.calculateEntropy();
        break;
    }

    if (variance < threshold || block.getArea() < minblock|| (block.getArea()/4) < minblock)
    {
        average = block.calculateAverageColor();
        return;
    }
    else
    {
        int midWidth = (int) block.getWidth()/2;
        int midHeight = (int) block.getHeight()/2;

        int w0 = midWidth;
        int w1 = (block.getWidth() % 2 == 0) ? midWidth : midWidth+1;
        int h0 = midHeight;
        int h1 = (block.getHeight() % 2 == 0) ? midHeight : midHeight+1;

        Point pos1(position.getX(), position.getY());
        Point pos2(position.getX() + midWidth, position.getY());
        Point pos3(position.getX(), position.getY() + midHeight);
        Point pos4(position.getX() + midWidth, position.getY() + midHeight);

        Block* b0 = block.getSubBlock(Point(0,0), w0, h0);
        Block* b1 = block.getSubBlock(Point(midWidth, 0), w1, h0);
        Block* b2 = block.getSubBlock(Point(0, midHeight), w0, h1);
        Block* b3 = block.getSubBlock(Point(midWidth, midHeight), w1, h1);

        child[0] = new QuadTree(pos1, *b0);
        child[1] = new QuadTree(pos2, *b1);
        child[2] = new QuadTree(pos3, *b2);
        child[3] = new QuadTree(pos4, *b3);

        delete b0;
        delete b1;
        delete b2;
        delete b3;
    
        for (int i = 0; i < 4; i++) {
            child[i]->mainProcess(emd, threshold, minblock);
        }  

    }
}

// Serialisasi struktur quadtree ke file biner
void QuadTree::saveToImage(std::ofstream &out) {
    bool isLeaf = (child[0] == nullptr);
    out.write(reinterpret_cast<const char*>(&isLeaf), sizeof(isLeaf));
    // Tulis informasi posisi dan ukuran blok
    int posX = position.getX();
    int posY = position.getY();
    int width = block.getWidth();
    int height = block.getHeight();
    out.write(reinterpret_cast<const char*>(&posX), sizeof(posX));
    out.write(reinterpret_cast<const char*>(&posY), sizeof(posY));
    out.write(reinterpret_cast<const char*>(&width), sizeof(width));
    out.write(reinterpret_cast<const char*>(&height), sizeof(height));

    if (isLeaf) {
        // Jika leaf, simpan informasi warna (misalnya dari 'average')
        unsigned char r = average.getColour(0);
        unsigned char g = average.getColour(1);
        unsigned char b = average.getColour(2);
        out.write(reinterpret_cast<const char*>(&r), sizeof(r));
        out.write(reinterpret_cast<const char*>(&g), sizeof(g));
        out.write(reinterpret_cast<const char*>(&b), sizeof(b));
    } else {
        // Simpan rekursif keempat child
        for (int i = 0; i < 4; i++) {
            child[i]->saveToImage(out);
        }
    }
}

// Deserialisasi quadtree dari file biner
QuadTree* QuadTree::loadFromImage(std::ifstream &in) {
    if (in.eof())
        return nullptr;

    bool isLeaf;
    in.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
    if (in.fail())
        return nullptr;

    int posX, posY, width, height;
    in.read(reinterpret_cast<char*>(&posX), sizeof(posX));
    in.read(reinterpret_cast<char*>(&posY), sizeof(posY));
    in.read(reinterpret_cast<char*>(&width), sizeof(width));
    in.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Buat node dengan informasi yang sudah dibaca
    QuadTree* node = new QuadTree(Point(posX, posY), Block(width, height));

    if (isLeaf) {
        unsigned char r, g, b;
        in.read(reinterpret_cast<char*>(&r), sizeof(r));
        in.read(reinterpret_cast<char*>(&g), sizeof(g));
        in.read(reinterpret_cast<char*>(&b), sizeof(b));
        node->average = Pixel(r, g, b);
        // Isi blok dengan warna rata-rata
        node->block.fillWithColor(node->average);
    } else {
        for (int i = 0; i < 4; i++) {
            node->child[i] = QuadTree::loadFromImage(in);
        }
    }
    return node;
}

// Fungsi untuk mengisi canvas (matriks Pixel) menggunakan informasi quadtree
void QuadTree::fillCanvas(Pixel** canvas) {
    if (child[0] == nullptr) { // leaf node
        for (int y = 0; y < block.getHeight(); y++) {
            for (int x = 0; x < block.getWidth(); x++) {
                canvas[position.getY() + y][position.getX() + x] = average;
            }
        }
    } else {
        for (int i = 0; i < 4; i++) {
            if (child[i] != nullptr)
                child[i]->fillCanvas(canvas);
        }
    }
}

// Fungsi dummy untuk membangun pohon quadtree dari "gambar" buatan
// Di sini, kita buat blok dengan ukuran width x height dan mengisi
// setengah kiri merah, setengah kanan biru
QuadTree* QuadTree::buildDummyTree(int width, int height) {
    Block blk(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width / 2)
                blk.setIntensity(Point(x, y), Pixel(255, 0, 0));   // Merah
            else
                blk.setIntensity(Point(x, y), Pixel(0, 0, 255));   // Biru
        }
    }
    QuadTree* node = new QuadTree(Point(0, 0), blk);
    return node;
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

// int main(int argc, char const *argv[])
// {
//     QuadTree *tes = QuadTree::buildFromImage("../test/monyet.jpg");
//     tes->printQuadTree();
//     // std::cout << "Variance: " <<  << ", MAD: " << tes->calculateMAD() << ", MPD: " << tes->calculateMPD() << ", Entropy: " << tes->calculateEntropy() << std::endl;
//     return 0;
// }
