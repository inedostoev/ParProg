#include "skiplist.hpp"

extern int max_level;

int main() {
    SkipList<int> *S = new SkipList<int>();
    std::vector<atomicNode<int>*> a(max_level + 1);
    std::vector<atomicNode<int>*> b(max_level + 1);
    S->search(5, a, b);
    S->insert(5);
    S->insert(15);
    S->insert(115);
    S->insert(110);
    S->insert(41);
    S->insert(50);
    
    S->print();

    S->remove(5);
    S->remove(110);
    S->print();
}
