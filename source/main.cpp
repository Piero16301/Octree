#include <iostream>

#include "Octree.h"

int main() {
    Octree myoctree;
    myoctree.loadImages();
    //myoctree.showImages();
    //CImg <char> result = myoctree.getCut({0,511,3},{511,511,3},{0,0,3},{511,0,3});
    //result.display();
    myoctree.rebuildImagesFromOctree();
    CImg <char> result = myoctree.getCut({0,511,39},{511,511,0},{0,0,39},{511,0,0});
    result.display();
    cout << "SUCCESSFULLY FINISHED" << endl;
    return 0;
}
