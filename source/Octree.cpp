#include <iostream>
#include <fstream>
#include <vector>

#include "Octree.h"

Octree::Octree() {
    this->root = nullptr;
}

CImg<int> Octree::binarize(CImg<float> &img, int umbral) {
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

bool Octree::isUniqueColor(Node *&oNode) {
    char firstColor = this->arrayMat[oNode->oct[2].first](oNode->oct[0].first,oNode->oct[1].first);
    for (int i = oNode->oct[0].first; i <= oNode->oct[0].second; i++) {
        for (int j = oNode->oct[1].first; j <= oNode->oct[1].second; j++) {
            for (int k = oNode->oct[2].first; k <= oNode->oct[2].second; k++) {
                if (this->arrayMat[k](i,j) != firstColor) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Octree::insertRecursive(Node *&oNode) {
    if (oNode->oct[0].first == oNode->oct[0].second || oNode->oct[1].first == oNode->oct[1].second || oNode->oct[2].first == oNode->oct[2].second) {
        //cout << "x: " << oNode->oct[0].first << ',' << oNode->oct[0].second << " y: " << oNode->oct[1].first << ',' << oNode->oct[1].second << " z: " << oNode->oct[2].first << ',' << oNode->oct[2].second << endl;
        oNode->color = this->arrayMat[oNode->oct[2].first](oNode->oct[0].first,oNode->oct[1].first);
        return;
    }
    if (isUniqueColor(oNode)) {
        oNode->color = this->arrayMat[oNode->oct[2].first](oNode->oct[0].first,oNode->oct[1].first);
        return;
    }
    int midX = (oNode->oct[0].second + oNode->oct[0].first) / 2;
    int midY = (oNode->oct[1].second + oNode->oct[1].first) / 2;
    int midZ = (oNode->oct[2].second + oNode->oct[2].first) / 2;
    oNode->m_pSon[0] = new Node({oNode->oct[0].first,midX},{midY+1,oNode->oct[1].second},{midZ+1,oNode->oct[2].second});
    oNode->m_pSon[1] = new Node({midX+1,oNode->oct[0].second},{midY+1,oNode->oct[1].second},{midZ+1,oNode->oct[2].second});
    oNode->m_pSon[2] = new Node({oNode->oct[0].first,midX},{oNode->oct[1].first,midY},{midZ+1,oNode->oct[2].second});
    oNode->m_pSon[3] = new Node({midX+1,oNode->oct[0].second},{oNode->oct[1].first,midY},{midZ+1,oNode->oct[2].second});

    oNode->m_pSon[4] = new Node({oNode->oct[0].first,midX},{midY+1,oNode->oct[1].second},{oNode->oct[2].first,midZ});
    oNode->m_pSon[5] = new Node({midX+1,oNode->oct[0].second},{midY+1,oNode->oct[1].second},{oNode->oct[2].first,midZ});
    oNode->m_pSon[6] = new Node({oNode->oct[0].first,midX},{oNode->oct[1].first,midY},{oNode->oct[2].first,midZ});
    oNode->m_pSon[7] = new Node({midX+1,oNode->oct[0].second},{oNode->oct[1].first,midY},{oNode->oct[2].first,midZ});
    insertRecursive(oNode->m_pSon[0]);
    insertRecursive(oNode->m_pSon[1]);
    insertRecursive(oNode->m_pSon[2]);
    insertRecursive(oNode->m_pSon[3]);
    insertRecursive(oNode->m_pSon[4]);
    insertRecursive(oNode->m_pSon[5]);
    insertRecursive(oNode->m_pSon[6]);
    insertRecursive(oNode->m_pSon[7]);
}

void Octree::loadImages() {
    system("find ../../database/paciente1/1 -type f -name \"*.BMP\" | sort > ../paciente1.txt");
    std::ifstream listFile("../paciente1.txt");
    std::string filePath;
    while (getline(listFile, filePath)) {
        cimg_library::CImg <float> A(filePath.c_str());
        CImg<char> R = binarize(A, 60);
        this->arrayMat.push_back(R);
    }
    auto image = this->arrayMat[0];
    this->root = new Node({0,image.width()-1},{0,image.height()-1},{0,this->arrayMat.size()-1});
    insertRecursive(this->root);
}

CImg <char> Octree::getCut(const vector<int>& point1, const vector<int>& point2, const vector <int>& point3, const vector <int>& point4) {
    CImg <char> R(512,512);
    double intevalZ = ((double)(abs(point3[2] - point4[2])) / 512);
    double counterZ = point4[2];
    for (int i = 0; i < 512; i++) {
        int zValue = (int)round(counterZ);
        cout << "Nivel: " << zValue << endl;
        for (int j = 0; j < 512; j++) {
            R(i,j) = this->arrayMat[zValue](i,j);
        }
        counterZ += intevalZ;
    }
    return R;
}

void Octree::recreateRecursive(vector<CImg<char>> &arrayMatRebuilt, Node* &oNode) {
    if (oNode->m_pSon[0] == nullptr) {
        for (int i = oNode->oct[0].first; i <= oNode->oct[0].second; i++) { // Eje X
            for (int j = oNode->oct[1].first; j <= oNode->oct[1].second; j++) { // Eje Y
                for (int k = oNode->oct[2].first; k <= oNode->oct[2].second; k++) { // Eje Z
                    arrayMatRebuilt[k](i, j) = oNode->color;
                }
            }
        }
        return;
    }
    for (auto & i: oNode->m_pSon) {
        recreateRecursive(arrayMatRebuilt, i);
    }
}

void Octree::rebuildImagesFromOctree() {
    vector <CImg <char>> arrayMatRebuilt(40,CImg<char>(512,512));
    recreateRecursive(arrayMatRebuilt, this->root);
    // Show rebuilt Images
    /*for(const auto& image : arrayMatRebuilt){
        image.display();
    }*/
    //this->arrayMat = arrayMatRebuilt;
}

void Octree::showImages() {
    for(const auto& image : arrayMat){
        image.display();
    }
}

Octree::~Octree() {
    if (this->root) {
        this->root->killSelf();
    }
}
