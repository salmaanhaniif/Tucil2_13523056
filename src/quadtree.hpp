#include "iostream"
#include "block.hpp"
#include "pixel.hpp"
#include "point.hpp"
#include <iostream>
#include <fstream>

#ifndef QUADTREE_H
#define QUADTREE_H

class QuadTree
{

private:
    Point position;
    Block block;
    QuadTree* *child;
    int maxChild = 4;
    Pixel average;

public:
    QuadTree(Point pos, Block sz);
    static QuadTree* buildFromImage (const char* filename);
    void mainProcess(int emd, double threshold, int minSize);
    ~QuadTree();
    void saveToImage(std::ofstream &out);
    static QuadTree* loadFromImage(std::ifstream &in);
    void fillCanvas(Pixel** canvas);
    QuadTree* buildDummyTree(int width, int height);
    void printQuadTree();
    int getDepth();
    int countNodes();
    int getWidth();
    int getHeight();
};



#endif