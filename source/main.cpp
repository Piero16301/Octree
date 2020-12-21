#include <iostream>

#include "Octree.h"

int main() {
    // Declaración del objeto Octree
    Octree octree;

    // Se construye el Octree a partir de la base de datos
    octree.loadImages();

    // Se muestran las imágenes del cubo original
    //octree.showImages();

    // Se sobreescribe el cubo original y se reeplaza por los datos
    // obtenidos de las hojas del Octree
    //octree.rebuildImagesFromOctree();

    // Prueba de recorte alrededor del eje X, y se le envía 4 puntos
    // en 3D (como vectores) y se guardan como jpg
    //CImg <char> result = octree.getPlaneAroundY({148,396,36},{452,396,5},{148,8,36},{452,8,5});
    //result.save("../../test.jpg");

    // Funciones que generan GIF's atravesando la estructura 3D
    // a lo largo de cada uno de los ejes
    octree.generateCutsX();
    octree.generateCutsY();
    octree.generateCutsZ();

    // Se generan 20 cortes random y se guardan como jpg
    octree.testRandomCuts(20);

    // Cantidad de memoria RAM usada por el cubo original
    // y por el Octree
    cout << "Memory usage cube: " << octree.getMemoryCube() << " bytes" << endl;
    cout << "Memory usage octree: " << octree.getMemoryOctree() << " bytes" << endl;

    cout << "SUCCESSFULLY FINISHED" << endl;

    return 0;
}
