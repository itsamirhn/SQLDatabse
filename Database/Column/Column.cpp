//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "Column.h"
#include "../Utils/Utils.h"

Node *Column::insertField(Field *f) {
    Node *newNode = new Node();
    newNode->data = f->getHash();
    T->insert(newNode);
    MP[newNode] = f;
    return newNode;
}

Node *Column::getNode(Operator conditionOperator, Field *value) {
    Node *node = T->searchNodeWithDataAndOperator(value->getHash(), conditionOperator);
    return node;
}

vector<Node *> Column::selectNodes(Operator conditionOperator, Field *value) {
    int hashData = value->getHash();
    vector<Node *> v;
    Node *node = T->searchNodeWithDataAndOperator(hashData, conditionOperator);
    while (node != nullptr && checkOperator(conditionOperator, node->data, hashData)) {
        v.push_back(node);
        node = T->getNext(withEqual(conditionOperator), node);
    }
    return v;
}

void Column::removeNode(Node *node) {
    T->removeNode(node);
}

vector<Node *> Column::getNodes() {
    return T->traverse();
}

Field *Column::getField(Node * node) {
    if (node == nullptr) return nullptr;
    if (MP.find(node) == MP.end()) throw invalid_argument("This Node doesn't belong to this Column");
    return MP[node];
}