#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
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
                R(i, j) = (char)255;
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

CImg <char> Octree::getPlaneAroundY(const vector<int>& point1, const vector<int>& point2, const vector <int>& point3, const vector <int>& point4) {
    CImg <char> R((point4[0]-point3[0]+1),(point1[1]-point3[1])+1);
    double intevalZ = ((double)(abs(point3[2] - point4[2])) / 512);
    double counterZ = point4[2];
    int i_1 = 0;
    for (int i = point3[0]; i <= point4[0]; i++) {
        int zValue = (int)round(counterZ);
        int j_1 = 0;
        for (int j = point3[1]; j <= point1[1]; j++) {
            R(i_1,j_1) = this->arrayMat[zValue](i,j);
            j_1++;
        }
        counterZ += ((point3[2]-point4[2]>=0) ? intevalZ : intevalZ*-1);
        i_1++;
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

void Octree::memoryRecursive(unsigned long long &totalSize, Node* &oNode) {
    if (oNode->m_pSon[0] == nullptr) {
        totalSize += sizeof(Node);
        return;
    }
    for (auto & i: oNode->m_pSon) {
        memoryRecursive(totalSize, i);
    }
}

void Octree::rebuildImagesFromOctree() {
    vector <CImg <char>> arrayMatRebuilt(40,CImg<char>(512,512));
    recreateRecursive(arrayMatRebuilt, this->root);
    // Show rebuilt Images
    /*for(const auto& image : arrayMatRebuilt){
        image.display();
    }*/
    this->arrayMat = arrayMatRebuilt;
}

void Octree::generateCutsX() {
    int xPoint = 0;
    while (xPoint < 512) {
        CImg <char> frame(this->arrayMat[0].width(), this->arrayMat.size());
        for (int i = 0; i < this->arrayMat[0].width(); i++) {
            for (int j = 0; j < this->arrayMat.size(); j++) {
                frame(i,j) = this->arrayMat[j](xPoint,i);
            }
        }
        string path = "../../axis_cuts/axis_x/image" + to_string((xPoint+1)) + ".jpg";
        frame.save(path.c_str());
        xPoint++;
    }
    system("rm ../../axis_cuts/axis_x/video.avi ../../axis_cuts/axis_x.gif");
    system("ffmpeg -f image2 -framerate 10 -i ../../axis_cuts/axis_x/image%d.jpg ../../axis_cuts/axis_x/video.avi");
    system("ffmpeg -i ../../axis_cuts/axis_x/video.avi -pix_fmt rgb24 -loop 0 ../../axis_cuts/axis_x.gif");
}

void Octree::generateCutsY() {
    int yPoint = 0;
    while (yPoint < 512) {
        CImg <char> frame(this->arrayMat[0].height(), this->arrayMat.size());
        for (int i = 0; i < this->arrayMat[0].height(); i++) {
            for (int j = 0; j < this->arrayMat.size(); j++) {
                frame(i,j) = this->arrayMat[j](i,yPoint);
            }
        }
        string path = "../../axis_cuts/axis_y/image" + to_string((yPoint+1)) + ".jpg";
        frame.save(path.c_str());
        yPoint++;
    }
    system("rm ../../axis_cuts/axis_y/video.avi ../../axis_cuts/axis_y.gif");
    system("ffmpeg -f image2 -framerate 10 -i ../../axis_cuts/axis_y/image%d.jpg ../../axis_cuts/axis_y/video.avi");
    system("ffmpeg -i ../../axis_cuts/axis_y/video.avi -pix_fmt rgb24 -loop 0 ../../axis_cuts/axis_y.gif");
}

void Octree::generateCutsZ() {
    int counter = 1;
    for (const auto& it : this->arrayMat) {
        string path = "../../axis_cuts/axis_z/image" + to_string(counter++) + ".jpg";
        it.save(path.c_str());
    }
    system("rm ../../axis_cuts/axis_z/video.avi ../../axis_cuts/axis_z.gif");
    system("ffmpeg -f image2 -framerate 10 -i ../../axis_cuts/axis_z/image%d.jpg ../../axis_cuts/axis_z/video.avi");
    system("ffmpeg -i ../../axis_cuts/axis_z/video.avi -pix_fmt rgb24 -loop 0 ../../axis_cuts/axis_z.gif");
}

void Octree::showImages() {
    for(const auto& image : arrayMat){
        image.display();
    }
}

void Octree::testRandomCuts(int numCuts) {
    // Generar 4 puntos random
    srand (time(nullptr));
    system("rm ../../testRandomResults/*.jpg");
    for(int i = 0; i < numCuts; i++){
        int xP1P3 = rand() % 200;
        int xP2P4 = rand() % 200 + 312;
        int yP1P2 = rand() % 200 + 312;
        int yP3P4 = rand() % 200;
        int zP2P4 = rand() % 10;
        int zP1P3 = rand() % 10 + 30;
        CImg <char> result = getPlaneAroundY(
                {xP1P3, yP1P2, zP1P3},
                {xP2P4, yP1P2, zP2P4},
                {xP1P3, yP3P4, zP1P3},
                {xP2P4, yP3P4, zP2P4});
        string path = "../../testRandomResults/image_" +
                to_string((xP1P3)) + "-" + to_string((yP1P2)) + "-" + to_string((zP1P3)) + "_" +
                to_string((xP2P4)) + "-" + to_string((yP1P2)) + "-" + to_string((zP2P4)) + "_" +
                to_string((xP1P3)) + "-" + to_string((yP3P4)) + "-" + to_string((zP1P3)) + "_" +
                to_string((xP2P4)) + "-" + to_string((yP3P4)) + "-" + to_string((zP2P4)) + ".jpg";
        result.save(path.c_str());
    }
}

unsigned long long Octree::getMemoryCube() {
    unsigned long long result = 0;
    for (auto & i : this->arrayMat) {
        result += i.size();
    }
    return result;
}

unsigned long long Octree::getMemoryOctree() {
    unsigned long long result = 0;
    memoryRecursive(result, this->root);
    return result;
}

Octree::~Octree() {
    if (this->root) {
        this->root->killSelf();
    }
}
