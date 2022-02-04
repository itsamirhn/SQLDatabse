//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "BTNode.h"
#include "../../Utils/Utils.h"
#include <iostream>
#include <vector>
using namespace std;

void BTNode::print(int indent) {
    for (int i = 0; i < indent; i++) cout << '\t';
    for (int i = 0; i < n; i++) cout << K[i]->data << ' ';
    if (!leaf) {
        cout << ':' << endl;
        for (int i = 0; i <= n; i++)  C[i]->print(indent + 1);
    } else cout << endl;
}
void BTNode::traverse(vector<Node *> &v) {
    for (int i = 0; i < n; i++) {
        if (!leaf) C[i]->traverse(v);
        v.push_back(K[i]);
    }
    if (!leaf) C[n]->traverse(v);
}
bool BTNode::isFull() const {
    return n == 2 * t - 1;
}


void BTNode::placeChild(int index, BTNode *x) {
    x->parent = this;
    x->parentIdx = index;
    C[index] = x;
}
// C[index] = x
void BTNode::insertChild(int index, BTNode *x) {
    for (int i = n; i >= index; i--) C[i + 1] = C[i];
    placeChild(index, x);
}
BTNode* BTNode::removeChild(int index) {
    BTNode *tmp = C[index];
    tmp->parent = nullptr;
    for (int i = index + 1; i <= n; i++) C[i - 1] = C[i];
    return tmp;
}

void BTNode::placeKey(int index, Node *k) {
    k->self = this;
    K[index] = k;
}
// K[index] = x & n++
void BTNode::insertKey(int index, Node *x) {
    for (int i = n - 1; i >= index; i--) K[i + 1] = K[i];
    placeKey(index, x);
    n++;
}
Node* BTNode::removeKey(int index) {
    Node *tmp = K[index];
//    tmp->self = nullptr;
    for (int i = index + 1; i < n; i++) K[i - 1] = K[i];
    n--;
    return tmp;
}

int BTNode::findKeyIndexOfNode(Node *node) {
    int i = 0;
    while (i < n && node != K[i]) i++;
    return i;
}

int BTNode::findKeyIndexWithOperator(Operator op, int data) {
    if (isAscending(op)) {
        int i = 0;
        while (i < n && !checkOperator(op, K[i]->data, data)) i++;
        return i;
    } else {
        int i = n - 1;
        while (i >= 0 && !checkOperator(op, K[i]->data, data)) i--;
        return i;
    }
}

Node* BTNode::getPredecessor(int index) {
    if (index < 0 || index >= n) return nullptr;
    if (leaf) {
        BTNode* p = this;
        int pi = index;
        while (p != nullptr) {
            if (pi > 0) return p->K[pi - 1];
            pi = p->parentIdx;
            p = p->parent;
        }
        return nullptr;
    }
    BTNode *cur = C[index];
    while (!cur->leaf) cur = cur->C[cur->n];
    return cur->K[cur->n - 1];
}
Node* BTNode::getSuccessor(int index) {
    if (index < 0 || index >= n) return nullptr;
    if (leaf) {
        if (index + 1 < n) return K[index + 1];
        BTNode* p = parent;
        int pi = parentIdx;
        while (p != nullptr) {
            if (pi < p->n) return p->K[pi];
            pi = p->parentIdx;
            p = p->parent;
        }
        return nullptr;
    }
    BTNode *cur = C[index + 1];
    while (!cur->leaf) cur = cur->C[0];
    return cur->K[0];
}
Node* BTNode::getNext(Operator op, Node* node) {
    if (node->self != this) throw invalid_argument("Node doesn't belong to this BTNode");
    int i = findKeyIndexOfNode(node);
    if (i == n) throw invalid_argument("Node doesn't found in this BTNode");
    if (isAscending(op) || op == Operator::Equal) return getSuccessor(i);
    else return getPredecessor(i);
}



void BTNode::insertNonFull(Node *k) {
    int i = findKeyIndexWithOperator(Operator::GreaterThan, k->data);
    if (leaf) insertKey(i, k);
    else {
        if (C[i]->isFull()) {
            splitChild(i);
            i += (K[i]->data < k->data);
        }
        C[i]->insertNonFull(k);
    }
}
void BTNode::splitChild(int index) {
    BTNode *x = C[index];

    BTNode *l = new BTNode(t, x->leaf), *r = new BTNode(t, x->leaf);

    Node *md = x->K[t - 1];
    for (int i = 0; i < t - 1; i++) {
        l->insertKey(i, x->K[i]);
        r->insertKey(i, x->K[i + t]);
    }

    if (!x->leaf) {
        for (int i = 0; i < t; i++) {
            l->placeChild(i, x->C[i]);
            r->placeChild(i, x->C[i + t]);
        }
    }

    placeChild(index, l);
    insertChild(index + 1, r);
    insertKey(index, md);

    delete x;
}
// index & index + 1
void BTNode::merge(int index) {
    BTNode *child = C[index];
    BTNode *sibling = C[index + 1];

    child->insertKey(t - 1, K[index]);
    for (int i = 0; i < sibling->n; i++) child->insertKey(t + i, sibling->K[i]);

    if (!child->leaf) for (int i = 0; i <= sibling->n; i++) child->placeChild(t + i, sibling->C[i]);

    removeChild(index + 1);
    removeKey(index);
    delete sibling;
}

void BTNode::fill(int index) {
    if (index > 0 && C[index - 1]->n >= t) return borrowFromPrev(index);
    else if (index < n && C[index + 1]->n >= t) return borrowFromNext(index);
    if (index < n) merge(index);
    else merge(index - 1);
}
void BTNode::borrowFromPrev(int index) {
    BTNode *child = C[index];
    BTNode *sibling = C[index - 1];

    if (!child->leaf) child->insertChild(0, sibling->removeChild(sibling->n));
    child->insertKey(0, K[index - 1]);

    placeKey(index - 1, sibling->removeKey(sibling->n - 1));
}
void BTNode::borrowFromNext(int index) {
    BTNode *child = C[index];
    BTNode *sibling = C[index + 1];

    if (!child->leaf) child->insertChild(child->n + 1, sibling->removeChild(0));
    child->insertKey(child->n, K[index]);

    placeKey(index, sibling->removeKey(0));
}
void BTNode::removeFromLeaf(int index) { removeKey(index); }
void BTNode::removeFromNonLeaf(int index) {
    Node *k = K[index];
    if (C[index]->n >= t) {
        Node *pred = getPredecessor(index);
        placeKey(index, pred);
        return C[index]->removeNode(pred);
    } else if (C[index + 1]->n >= t) {
        Node *succ = getSuccessor(index);
        placeKey(index, succ);
        return C[index + 1]->removeNode(succ);
    }
    merge(index);
    return C[index]->removeNode(k);
}

void BTNode::removeNode(Node *node) {
    int i = findKeyIndexWithOperator(Operator::GreaterThanOrEqual, node->data);
    if (i < n && K[i] == node) {
        if (leaf) removeFromLeaf(i);
        else removeFromNonLeaf(i);
    } else if (leaf) return;
    else {
        bool flag = i == n;
        if (C[i]->n < t) fill(i);
        if (flag && i > n) C[i - 1]->removeNode(node);
        else C[i]->removeNode(node);
    }
}

Node* BTNode::searchNodeWithDataAndOperator(int data, Operator op) {
    if (op == Operator::Equal) {
        int i = findKeyIndexWithOperator(Operator::GreaterThanOrEqual, data);
        Node *tmp;
        if (leaf) tmp = nullptr;
        else tmp = C[i]->searchNodeWithDataAndOperator(data, op);
        if (tmp != nullptr) return tmp;
        if (0 <= i && i < n && checkOperator(Operator::Equal, K[i]->data, data)) return K[i];
        return nullptr;
    } else {
        int i = findKeyIndexWithOperator(op, data);
        Node *tmp;
        if (leaf) tmp = nullptr;
        else tmp = C[i + isDescending(op)]->searchNodeWithDataAndOperator(data, op);
        if (tmp != nullptr) return tmp;
        if (0 <= i && i < n) return K[i];
        return nullptr;
    }
}