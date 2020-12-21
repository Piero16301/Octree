#include <iostream>

#include "Octree.h"

int main() {
    Octree myoctree;
    myoctree.loadImages();
    //myoctree.showImages();
    //myoctree.rebuildImagesFromOctree();
    //CImg <char> result = myoctree.getPlaneAroundY({148,396,36},{452,396,5},{148,8,36},{452,8,5});
    //result.save("../../test.jpeg");
    //myoctree.generateCutsX();
    //myoctree.generateCutsY();
    //myoctree.generateCutsZ();
    //myoctree.testRandomCuts(20);
    cout << "Memory usage cube: " << myoctree.getMemoryCube() << " bytes" << endl;
    cout << "Memory usage octree: " << myoctree.getMemoryOctree() << " bytes" << endl;
    cout << "SUCCESSFULLY FINISHED" << endl;
    return 0;
}
