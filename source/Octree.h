#ifndef OCTREE_OCTREE_H
#define OCTREE_OCTREE_H
#include "CImg.h"
#include "Node.h"
#include <vector>

using namespace cimg_library;
using namespace std;

class Octree {
private:
    Node* root;
    vector<CImg<char>> arrayMat;
    static CImg <int> binarize(CImg <float> &img, int umbral);
public:
    Octree();

    void loadImages();
    void showImages();
};


#endif //OCTREE_OCTREE_H
