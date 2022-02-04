//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#ifndef SQL_BTREE_H
#define SQL_BTREE_H

#include "../BTNode/BTNode.h"
#include <vector>
using namespace std;

class BTree {
    BTNode* root;
    int t, n;
public:
    BTree(int _t) { t = _t, root = nullptr, n = 0; }
    void print();
    vector<Node *> traverse();
    void insert(Node *k);
    Node* searchNodeWithDataAndOperator(int data, Operator op);
    Node* getNext(Operator op, Node *node);
    void removeNode(Node *node);
};


#endif //SQL_BTREE_H
