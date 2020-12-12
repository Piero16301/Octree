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
    vector <CImg <char>> arrayMat;

protected:
    static CImg <int> binarize(CImg <float> &img, int umbral);

    bool isUniqueColor(Node* &oNode);

    void insertRecursive(Node* &oNode);

public:
    Octree();

    void loadImages();

    void showImages();

    ~Octree();
};


#endif //OCTREE_OCTREE_H
