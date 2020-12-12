#ifndef OCTREE_NODE_H
#define OCTREE_NODE_H
#include <utility>

using namespace std;
class Octree;

class Node {
private:
    // [0] para eje x
    // [1] para eje y
    // [2] para eje z
    std::pair <unsigned short int, unsigned short int> oct [3];
    Node* m_pSon[8]{};
    unsigned char color{};

protected:
    void killSelf();

public:
    Node(pair<unsigned short int, unsigned short int> x, pair<unsigned short int, unsigned short int> y, pair<unsigned short int, unsigned short int> z);
    friend class Octree;
};


#endif //OCTREE_NODE_H
