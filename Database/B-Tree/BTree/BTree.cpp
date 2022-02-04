//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "BTree.h"

void BTree::print() { if (root != nullptr) return root->print(); }

vector<Node *> BTree::traverse() {
    vector<Node *> v;
    if (root != nullptr) {
        root->traverse(v);
        return v;
    }
    return v;
}
void BTree::insert(Node *k) {
    if (root == nullptr) root = new BTNode(t, true);
    if (root->isFull()) {
        BTNode *newRoot = new BTNode(t);
        newRoot->placeChild(0, root);
        newRoot->splitChild(0);
        int i = (newRoot->K[0]->data < k->data);
        newRoot->C[i]->insertNonFull(k);
        root = newRoot;
    } else root->insertNonFull(k);
    n++;
}

Node* BTree::searchNodeWithDataAndOperator(int data, Operator op) {
    if (root == nullptr) return nullptr;
    return root->searchNodeWithDataAndOperator(data, op);
}
void BTree::removeNode(Node *node) {
    if (root == nullptr) return;
    root->removeNode(node);
    if (root->n == 0) {
        BTNode *tmpRoot = root;
        if (root->leaf) root = nullptr;
        else root = root->C[0], root->parent = nullptr;
        delete tmpRoot;
    }
    n--;
}

Node *BTree::getNext(Operator op, Node *node) {
    if (root == nullptr) return nullptr;
    return node->self->getNext(op, node);
}

