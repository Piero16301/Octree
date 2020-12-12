#include "Node.h"

Node::Node(pair<unsigned short, unsigned short> x, pair<unsigned short, unsigned short> y, pair<unsigned short, unsigned short> z) {
    this->oct[0] = x;
    this->oct[1] = y;
    this->oct[2] = z;
    for (auto & i : this->m_pSon) {
        i = nullptr;
    }
    this->color = (char)0;
}

void Node::killSelf() {
    if (this->m_pSon[0] != nullptr) {
        for (auto &i : this->m_pSon) {
            i->killSelf();
        }
    }
    delete this;
}
