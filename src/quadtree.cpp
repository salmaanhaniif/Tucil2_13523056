#include <iostream>
#include "quadtree.hpp"
#include <cmath>
#include <vector>

QuadTree::QuadTree(Point pos, Block sz)
{
    position = pos;
    size = sz;
    intensity = new Pixel[size.getArea()];
    child = new QuadTree *[maxChild];
    for (int i = 0; i < maxChild; i++)
    {
        child[i] = nullptr;
    }
}

QuadTree *QuadTree::buildFromImage(const char *filename)
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
            node->intensity[y * width + x] = Pixel(image[index], image[index + 1], image[index + 2]);
        }
    }
    stbi_image_free(image);
    return node;
}

void QuadTree::printQuadTree()
{
    position.displayPosition();
    size.displayBlock();

    std::cout << "[";
    for (int i = 0; i < size.getArea(); i++)
    {
        intensity[i].displayPixel();
        std::cout << " ";
    }
    std::cout << "]" << std::endl;
}

double QuadTree::getAverage(int colourCode) {
    double avg = 0;
    for (int i = 0; i < size.getArea(); i++)
    {
        avg += (int)intensity[i].getColour(colourCode);
    }
    avg = avg / size.getArea();
}

double QuadTree::calculateVariance()
{
    double variance = 0;
    variance = variance + calculateVarianceHelper(0) + calculateVarianceHelper(1) + calculateVarianceHelper(2);
    return variance / 3;
}

double QuadTree::calculateVarianceHelper(int colourCode)
{
    double avg = getAverage(colourCode);
    double sigma = 0;
    for (int i = 0; i < size.getArea(); i++)
    {
        sigma += pow(((int)intensity[i].getColour(colourCode) - avg), 2);
    }
    sigma = sigma / size.getArea();
    return sigma;
}

double QuadTree::calculateMAD()
{
    double mad = 0;
    mad = mad + calculateMADHelper(0) + calculateMADHelper(1) + calculateMADHelper(2);
    return mad / 3;
}

double QuadTree::calculateMADHelper(int colourCode)
{
    double avg = getAverage(colourCode);
    double sigma = 0;
    for (int i = 0; i < size.getArea(); i++)
    {
        sigma += abs((int)intensity[i].getColour(colourCode) - avg);
    }
    sigma = sigma / size.getArea();
    return sigma;
}

double QuadTree::calculateMPD()
{
    auto result = calculateMPDHelper(0, 0, size.getArea()-1);
    int minr = result.first;
    int maxr = result.second;
    auto result1 = calculateMPDHelper(1, 0, size.getArea()-1);
    int ming = result.first;
    int maxg = result.second;
    auto result2 = calculateMPDHelper(2, 0, size.getArea()-1);
    int minb = result.first;
    int maxb = result.second;

    return ((maxr - minr + maxg - ming + maxb - minb) / 3);
}

std::pair<int, int> QuadTree::calculateMPDHelper(int colourCode, int l, int r)
{
    if (l == r)
    {
        int val = intensity[l].getColour(colourCode);
        return {val, val}; // min = max = 1 pixel itu
    }

    int mid = (l + r) / 2;
    auto left = calculateMPDHelper(colourCode, l, mid);
    auto right = calculateMPDHelper(colourCode, mid + 1, r);

    int minVal = std::min(left.first, right.first);
    int maxVal = std::max(left.second, right.second);
    return {minVal, maxVal};
}

double QuadTree::calculateEntropy()
{
    return ((calculateEntropyHelper(0) + calculateEntropyHelper(1) + calculateEntropyHelper(2))/3);
}

double QuadTree::calculateEntropyHelper(int colourCode)
{
    int N = 256;
    std::vector<int> histogram(N, 0);
    int totalPixel = size.getArea();

    // Hitung histogram untuk channel tertentu
    for (int i = 0; i < totalPixel; i++)
    {
        int colourVal = (int) intensity[i].getColour(colourCode);
        histogram[colourVal]++;
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

QuadTree::~QuadTree()
{
    for (int i = 0; i < maxChild; i++)
    {
        delete child[i];
    }
    delete[] child;
    delete[] intensity;
}

int main(int argc, char const *argv[])
{
    QuadTree *tes = QuadTree::buildFromImage("../test/monyet.jpg");
    tes->printQuadTree();
    std::cout << "Variance: " << tes->calculateVariance() << ", MAD: " << tes->calculateMAD() << ", MPD: " << tes->calculateMPD() << ", Entropy: " << tes->calculateEntropy() << std::endl;
    return 0;
}
