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
    void recreateRecursive(vector <CImg <char>> &arrayMatRebuilt, Node* &oNode);

public:
    Octree();
    void loadImages();
    CImg <char> getCut(const vector <int>& point1, const vector <int>& point2, const vector <int>& point3, const vector <int>& point4);
    void rebuildImagesFromOctree();
    void showImages();

    ~Octree();
};


#endif //OCTREE_OCTREE_H
