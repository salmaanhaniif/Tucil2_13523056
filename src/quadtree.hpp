#include "iostream"
#include "block.hpp"
#include "pixel.hpp"
#include "point.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef QUADTREE_H
#define QUADTREE_H

class QuadTree
{

private:
    Point position;
    Block size;
    Pixel* intensity;
    QuadTree* *child;
    int maxChild = 4;

public:
    QuadTree(Point pos, Block sz);
    static QuadTree* buildFromImage (const char* filename);
    ~QuadTree();

    double getAverage(int colourCode);
    double calculateVariance();
    double calculateVarianceHelper(int colourCode);
    double calculateMAD();
    double calculateMADHelper(int colourCode);
    double calculateMPD();
    std::pair<int, int> calculateMPDHelper(int colourCode, int l, int r);
    double calculateEntropy();
    double calculateEntropyHelper(int colourCode);

    void printQuadTree();
};



#endif