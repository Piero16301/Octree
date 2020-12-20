#include <iostream>

#include "Octree.h"

int main() {
    Octree myoctree;
    myoctree.loadImages();
    //myoctree.showImages();
    //myoctree.rebuildImagesFromOctree();
    //CImg <char> result = myoctree.getPlaneAroundY({0,199,39},{199,199,0},{0,0,39},{199,0,0});
    //result.save("../../test.jpeg");
    //myoctree.generateCutsX();
    //myoctree.generateCutsY();
    //myoctree.generateCutsZ();
    myoctree.testRandomCuts(20);
    cout << "SUCCESSFULLY FINISHED" << endl;
    return 0;
}
