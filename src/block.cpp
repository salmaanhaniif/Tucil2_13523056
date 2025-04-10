#include <iostream>
#include "block.hpp"
#include <cmath>
#include <vector>

Block::Block() {
    width = 0;
    height = 0;
    intensity = nullptr;
}

Block::Block(int w, int h)
{
    width = w;
    height = h;
    intensity = new Pixel*[height];
    for (int i = 0; i < height; i++) {
        intensity[i] = new Pixel[width];
    }
}

Block::Block(const Block& other) {
    width = other.width;
    height = other.height;

    intensity = new Pixel*[height];
    for (int i = 0; i < height; i++) {
        intensity[i] = new Pixel[width];
        for (int j = 0; j < width; j++) {
            intensity[i][j] = other.intensity[i][j];
        }
    }
}

Block& Block::operator=(const Block& other) {
    if (this == &other) return *this; // handle self assign

    // bersihin data lama
    if (intensity) {
        for (int i = 0; i < height; i++)
            delete[] intensity[i];
        delete[] intensity;
    }

    width = other.width;
    height = other.height;

    intensity = new Pixel*[height];
    for (int i = 0; i < height; i++) {
        intensity[i] = new Pixel[width];
        for (int j = 0; j < width; j++) {
            intensity[i][j] = other.intensity[i][j];
        }
    }
    return *this;
}

Block::~Block()
{
    if (intensity != nullptr) {
        for (int i = 0; i < height; i++) {
            delete[] intensity[i];
        }
        delete[] intensity;
    }
}

int Block::getWidth() {
    return width;
}
int Block::getHeight() {
    return height;
}

int Block::getArea() {
    return width * height;
}

Pixel** Block::getBlockIntensity() const {
    return intensity;
}

Block* Block::getSubBlock(Point startPoint, int w, int h) {
    Block* subBlock = new Block(w,h); 

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            subBlock->intensity[y][x] = intensity[startPoint.getY() + y][startPoint.getX() + x];
        }
    }

    return subBlock; 
}

Pixel Block::getIntensity(Point pos) const {
    return intensity[pos.getY()][pos.getX()];
}

void Block::setIntensity(Point pos, const Pixel& pixel) {
    intensity[pos.getY()][pos.getX()] = pixel;
}

void Block::displayBlock() {
    std::cout << "Lebar: " << width << " Tinggi: " << height << " Luas: " << width*height << std::endl;
}

double Block::getAverage(int colourCode) {
    double avg = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            avg += intensity[y][x].getColour(colourCode);
        }
    }
    avg = avg / getArea();
    return avg;
}

Pixel Block::calculateAverageColor() {
    int avgR = getAverage(0); // 0 = Red
    int avgG = getAverage(1); // 1 = Green
    int avgB = getAverage(2); // 2 = Blue

    return Pixel(avgR, avgG, avgB);
}

void Block::fillWithColor(const Pixel &pix) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            intensity[y][x] = pix;
        }
    }
}

void Block::normalise() {
    double redAvg = getAverage(0);
    double greenAvg = getAverage(1);
    double blueAvg = getAverage(2);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            intensity[y][x].setColour(0, redAvg);
            intensity[y][x].setColour(1, greenAvg);
            intensity[y][x].setColour(2, blueAvg);
        }
    }
    
}

double Block::calculateVariance()
{
    double variance = 0;
    variance = variance + calculateVarianceHelper(0) + calculateVarianceHelper(1) + calculateVarianceHelper(2);
    return variance / 3;
}

double Block::calculateVarianceHelper(int colourCode)
{
    double avg = getAverage(colourCode);
    double sigma = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            sigma += powf(intensity[y][x].getColour(colourCode) - avg, 2);
        }
    }
    sigma = sigma / getArea();
    return sigma;
}

double Block::calculateMAD()
{
    double mad = 0;
    mad = mad + calculateMADHelper(0) + calculateMADHelper(1) + calculateMADHelper(2);
    return mad / 3;
}

double Block::calculateMADHelper(int colourCode)
{
    double avg = getAverage(colourCode);
    double sigma = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            sigma += abs((int)intensity[y][x].getColour(colourCode) - avg);
        }
    }
    sigma = sigma / getArea();
    return sigma;
}

double Block::calculateMPD()
{
    auto resultRed = calculateMPDHelper(0);
    int minr = resultRed.first;
    int maxr = resultRed.second;
    auto resultGreen = calculateMPDHelper(1);
    int ming = resultGreen.first;
    int maxg = resultGreen.second;
    auto resultBlue = calculateMPDHelper(2);
    int minb = resultBlue.first;
    int maxb = resultBlue.second;

    return ((maxr - minr + maxg - ming + maxb - minb) / 3);
}

std::pair<int, int> Block::calculateMPDHelper(int colourCode)
{
    int minVal = INT_MAX;
    int maxVal = INT_MIN;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int val = (int)intensity[y][x].getColour(colourCode);
            minVal = std::min(minVal, val);
            maxVal = std::max(maxVal, val);
        }
    }

    return {minVal, maxVal};
}

double Block::calculateEntropy()
{
    return ((calculateEntropyHelper(0) + calculateEntropyHelper(1) + calculateEntropyHelper(2))/3);
}

double Block::calculateEntropyHelper(int colourCode)
{
    int N = 256;
    std::vector<int> histogram(N, 0);
    int totalPixel = getArea();

    // Hitung histogram untuk channel tertentu
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int colourVal = (int)intensity[y][x].getColour(colourCode);
            histogram[colourVal]++;
        }
    }
    
    double entropy = 0;

    for (int i = 0; i < N; i++)
    {
        if (histogram[i] > 0) {
            double prob = (double) histogram[i]/totalPixel;
            entropy -= prob * log2(prob);
        }
    }
    return entropy;
}