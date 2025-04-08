#include <iostream>
#include "gif.h"
#include "block.hpp"
#include "pixel.hpp"
#include "point.hpp"
// #include "gifwriter.hpp" // Include the header file for GIFWriter

#ifndef QUADTREE_H
#define QUADTREE_H

class QuadTree
{

private:
    Point position;
    Block block;
    QuadTree* *child;
    int maxChild = 4;
    Pixel averageColor;

public:
    QuadTree(Point pos, Block sz);
    static QuadTree* buildFromImage (const char* filename);
    void mainProcess(int emd, double threshold, int minblock, 
        Pixel** canvas, int canvasWidth, int canvasHeight, GifWriter* gif);
    ~QuadTree();
    void captureFrame(Pixel** canvas, int canvasWidth, int canvasHeight, GifWriter* gif);
    void fillCompressedImage(Pixel** outputImage);
    void exportToPNG(const char* filename);
    void printQuadTree();
    int getDepth();
    int countNodes();

    int getWidth();
    int getHeight();
    Pixel getIntensity(Point pos);

    // Untuk membantu meng-copy nilai akhir ke canvas secara rekursif
    void fillCanvas(Pixel** canvas);
};



#endif