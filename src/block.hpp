#include "iostream"
#include "pixel.hpp"
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
    ~Block();
    int getWidth();
    int getHeight();
    int getArea();
    void displayBlock();
    Block getSubBlock(int startX, int startY, int w, int h);
    Pixel** getBlockIntensity() const;
    Pixel getIntensity(int x, int y) const;
    void setIntensity(int x, int y, const Pixel& pixel);
    
    void normalise();
    double getAverage(int colourCode);
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