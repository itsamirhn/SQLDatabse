//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#ifndef SQL_COLUMN_H
#define SQL_COLUMN_H
#include "../Field/Field.h"
#include "../B-Tree/BTree/BTree.h"
#include "../Operator/Operator.h"
#include <unordered_map>
using namespace std;

class Column {
    BTree* T;
    unordered_map<Node *, Field *> MP;
public:
    Type type;
    string title;
    Column(string _title, Type _type) {
        title = _title, type = _type;
        T = new BTree(2);
    }
    Node* insertField(Field* f);
    Node* getNode(Operator conditionOperator, Field* value);
    vector<Node *> selectNodes(Operator conditionOperator, Field* value);
    void removeNode(Node* node);
    vector<Node*> getNodes();
    Field* getField(Node *node);
};


#endif //SQL_COLUMN_H
