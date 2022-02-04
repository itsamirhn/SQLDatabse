//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#ifndef SQL_BTNODE_H
#define SQL_BTNODE_H

#include "../Node/Node.h"
#include "../../Operator/Operator.h"
#include <vector>
using namespace std;

class BTree;

class BTNode {
    int t, n;
    bool leaf;
    BTNode* parent;
    int parentIdx;
    Node** K;
    BTNode** C;
public:
    BTNode(int _t, bool _leaf = false) {
        parent = nullptr;
        parentIdx = 0;
        t = _t, leaf = _leaf;
        K = new Node* [2 * t - 1];
        C = new BTNode* [2 * t];
    }
    void print(int indent = 0);
    void traverse(vector<Node *> &v);
    bool isFull() const;
    void placeChild(int index, BTNode *x);
    // C[index] = x
    void insertChild(int index, BTNode *x);
    BTNode* removeChild(int index);
    void placeKey(int index, Node *k);
    // K[index] = x & n++
    void insertKey(int index, Node *x);
    Node* removeKey(int index);

    int findKeyIndexOfNode(Node *node);
    int findKeyIndexWithOperator(Operator op, int data);

    Node* getPredecessor(int index);
    Node* getSuccessor(int index);

    Node* getNext(Operator op, Node *);

    void insertNonFull(Node *k);
    void splitChild(int index);
    // index & index + 1
    void merge(int index);
    void fill(int index);
    void borrowFromPrev(int index);
    void borrowFromNext(int index);

    void removeFromLeaf(int index);
    void removeFromNonLeaf(int index);
    void removeNode(Node *node);

    Node* searchNodeWithDataAndOperator(int data, Operator op);

    friend BTree;
};


#endif //SQL_BTNODE_H
