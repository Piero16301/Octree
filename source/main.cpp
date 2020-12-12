#include <iostream>
#include "CImg.h"
#include <fstream>
#include <vector>

using namespace cimg_library;
using namespace std;

CImg<char> binarize(CImg<float> &img, int umbral) {
    CImg <char> R(img.width(),img.height());
    for(int i=0;i< img.width();i++) {
        for (int j = 0; j < img.height(); j++) {
            // Extrae color rojo (posicion 0 de la 3ra dimension)
            int r = img(i, j, 0);
            // Extrae color verde (posicion 1 de la 3ra dimension)
            int g = img(i, j, 1);
            // Extrae color azul (posicion 2 de la 3ra dimension)
            int b = img(i, j, 2);
            if ((r + g + b) / 3 > umbral) {
                R(i, j) = (char)1;
            } else {
                R(i, j) = (char)0;
            }
        }
    }
    return R;
}

int main() {
    system("find ../../images/paciente1/1 -type f -name \"*.BMP\" | sort > ../paciente1.txt");
    std::ifstream rutasInsert("../paciente1.txt");
    std::string rutaInsert;
    vector<CImg<char>> arrayMat;
    while (getline(rutasInsert, rutaInsert)) {
        cimg_library::CImg <float> A(rutaInsert.c_str());
        CImg<char> R = binarize(A, 60);
        //R.display();
        arrayMat.push_back(R);
    }

    //CImg <float> A("../test.BMP");
    //A.display();
    return 0;
}
