#include <iostream>
#include "quadtree.hpp"
#include <cmath>
#include <vector>

QuadTree::QuadTree(Point pos, Block bl)
{
    position = pos;
    block = bl;
    // intensity = new Pixel*[sz.getHeight()];
    // for (int i = 0; i < sz.getHeight(); i++) {
    //     intensity[i] = new Pixel[sz.getWidth()];
    // }
    child = new QuadTree *[maxChild] {nullptr};
    // for (int i = 0; i < maxChild; i++)
    // {
    //     child[i] = nullptr;
    // }
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

    QuadTree *node = new QuadTree(Point(), Block(width, height));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * channels;
            // node->block.intensity[y][x] = Pixel(image[index], image[index + 1], image[index + 2]);
            node->block.setIntensity(x, y, Pixel(image[index], image[index + 1], image[index + 2]));
        }
    }
    stbi_image_free(image);
    return node;
}

void QuadTree::mainProcess(int emd, double threshold, int minblock) {
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
        block.normalise();
        return;
    }
    else
    {
        int midWidth = (int) block.getWidth()/2;
        int midHeight = (int) block.getHeight()/2;
        Point pos1(0,0);
        Point pos2(midWidth,0);
        Point pos1(0,midHeight);
        Point pos1(midWidth,midHeight);
    }
    
    
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
            block.getIntensity(x, y).displayPixel();
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

int main(int argc, char const *argv[])
{
    QuadTree *tes = QuadTree::buildFromImage("../test/monyet.jpg");
    tes->printQuadTree();
    // std::cout << "Variance: " <<  << ", MAD: " << tes->calculateMAD() << ", MPD: " << tes->calculateMPD() << ", Entropy: " << tes->calculateEntropy() << std::endl;
    return 0;
}
