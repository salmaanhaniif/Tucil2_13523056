#include <iostream>
#include "quadtree.hpp"

QuadTree::QuadTree(Point pos, Block sz) {
    position = pos;
    size = sz;
    intensity = new Pixel[size.getArea()];
    child = new QuadTree*[maxChild];
    for (int i = 0; i < maxChild; i++) {
        child[i] = nullptr;
    }
    intensity = new Pixel[size.getArea()];
}

QuadTree* QuadTree::buildFromImage(const char* filename) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    if (image == nullptr) {
        std::cout << "Gagal memuat gambar" << std::endl;
        return nullptr;
    }

    QuadTree* node = new QuadTree(Point(), Block(width, height));

    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            int index = (y*width+x) * channels;
            node->intensity[y*width+x] = Pixel(image[index], image[index+1], image[index+2]);
        }
    }
    stbi_image_free(image);
    return node;
}

void QuadTree::printQuadTree() {
    position.displayPosition();
    size.displayBlock();

    std::cout << "[";
    for (int i = 0; i < size.getArea(); i++)
    {
        intensity[i].displayPixel();
        std::cout << " ";
    }
    std::cout << "]";
}

QuadTree::~QuadTree()
{
    for (int i=0; i<maxChild; i++) {
        delete child[i];
    }
    delete[] child;
    delete[] intensity;
}

int main(int argc, char const *argv[])
{
    QuadTree* tes = QuadTree::buildFromImage("../test/monyet.jpg");
    tes->printQuadTree();
    return 0;
}
