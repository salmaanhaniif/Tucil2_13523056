#include "iostream"
#include "pixel.hpp"
#include "point.hpp"
#ifndef BLOCK_H
#define BLOCK_H

class Block
{
private:
    int width;
    int height;
    Pixel** intensity;

public:
    Block();
    Block(int w, int h);
    Block(const Block& other);
    Block& operator=(const Block& other);
    ~Block();
    int getWidth();
    int getHeight();
    int getArea();
    void displayBlock();
    Pixel** getBlockIntensity() const;
    Block* getSubBlock(Point startPoint, int w, int h);
    Pixel getIntensity(Point pos) const;
    void setIntensity(Point pos, const Pixel& pixel);
    void fillWithColor(const Pixel &pix);

    void normalise();
    double getAverage(int colourCode);
    Pixel calculateAverageColor();
    double calculateVariance();
    double calculateVarianceHelper(int colourCode);
    double calculateMAD();
    double calculateMADHelper(int colourCode);
    double calculateMPD();
    std::pair<int, int> calculateMPDHelper(int colourCode);
    double calculateEntropy();
    double calculateEntropyHelper(int colourCode);
};

#endif