#include <iostream>
#include "quadtree.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    string imagepathIn;
    int emd;
    double threshold;
    int minSize;
    string imagepathOut;

    cout << "Welcome to Image Compressor by QuadTree" << endl;
    cout << "Tuliskan filepath dari Image (input): "; cin >> imagepathIn;
    cout << "Error Measurement Method" << endl << "1.Variance" << endl << "2.MAD" << endl << "3.MPD" << endl << "4.Entropy" << endl;
    cout << "Choose one (number): "; cin >> emd;
    cout << "Tentukan Nilai Threshold: "; cin >> threshold;
    cout << "Tentukan Area Minimum Block: ";cin >> minSize;
    cout << "Tuliskan filepath untuk Hasil: "; cin >> imagepathOut;

    // cout << imagepathIn << endl;
    // cout << emd << endl;
    // cout << threshold << endl;
    // cout << minSize << endl;
    // cout << imagepathOut << endl;

    return 0;
}
