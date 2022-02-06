//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "Table.h"
using namespace std;

void Table::insert(Record *record, int defaultId) {
    if (record->c + 1 != c) throw invalid_argument("Record columns number doesn't match to Table columns number");
    for (int i = 1; i < c; i++) if (record->F[i - 1]->type != C[i]->type) throw invalid_argument("Record columns type doesn't match to Table columns type");
    Node *idNode = createNewID(defaultId);
    Node *prev = idNode;
    for (int i = 1; i < c; i++) {
        Node *next = C[i]->insertField(record->F[i - 1]);
        prev->nextField = next;
        prev = next;
    }
    prev->nextField = idNode;
    n++;
}

vector<Record *>
Table::selectOneCondition(vector<string> columns, Condition* condition) {
    Field *f = new Field(condition->value, getColumn(condition->column)->type);
    vector<int> columnsIdx;
    for (string title : columns) columnsIdx.push_back(getColumnIdx(title));
    vector<Node*> nodes = getColumn(condition->column)->selectNodes(condition->op, f);
    vector<Record*> records;
    int conditionColumnIdx = getColumnIdx(condition->column);
    for (Node* node : nodes) records.push_back(getRecordFromNode(conditionColumnIdx, node));
    sort(records.begin(), records.end(), [] (const Record* l, const Record* r) { return l->F[0]->getHash() < r->F[0]->getHash(); });
    vector<Record*> sortedCustomRecords;
    for (Record* record : records) sortedCustomRecords.push_back(getCustomRecordFromRecord(columnsIdx, record));
    return records;
}

vector<Record *>
Table::selectTwoCondition(vector<string> columns, Condition *condition1, Condition *condition2, bool isAnd) {
    vector<string> allColumns = getColumnTitles();
    vector<Record *> part1 = selectOneCondition(allColumns, condition1);
    vector<Record *> part2 = selectOneCondition(allColumns, condition2);
    vector<Record *> unionPart, intersectionPart;
    int i = 0, j = 0;
    while (i < (int)part1.size() && j < (int)part2.size()) {
        if (part1[i]->F[0]->getHash() < part2[j]->F[0]->getHash()) unionPart.push_back(part1[i++]);
        else if (part1[i]->F[0]->getHash() > part2[j]->F[0]->getHash()) unionPart.push_back(part2[j++]);
        else unionPart.push_back(part1[i]), intersectionPart.push_back(part1[i]), i++, j++;
    }
    while (i < (int)part1.size()) unionPart.push_back(part1[i++]);
    while (j < (int)part2.size()) unionPart.push_back(part2[j++]);
    vector<Record *> results;
    if (isAnd) results = intersectionPart;
    else results = unionPart;
    vector<Record*> customRecords;
    vector<int> columnsIdx;
    for (string title : columns) columnsIdx.push_back(getColumnIdx(title));
    for (Record* record : results) customRecords.push_back(getCustomRecordFromRecord(columnsIdx, record));
    return customRecords;
}

vector<int> Table::remove(vector<Record *> fullRecords) {
    vector<int> ids;
    for (Record* record : fullRecords) {
        if (record->c != c) throw invalid_argument("Removing records shouldn't be custom and have to be complete.");
        int id = record->F[0]->getHash();
        Node* node = C[0]->getNode(Operator::Equal, record->F[0]);
        for (int i = 0; i < c; i++) {
            C[i]->removeNode(node);
            Node *next = node->nextField;
            delete node;
            node = next;
        }
        n--;
        ids.push_back(id);
        removedIds.insert(id);
    }
    return ids;
}

int Table::findNewID() {
    if (removedIds.empty()) return id++;
    auto it = removedIds.begin();
    int i = *it;
    removedIds.erase(it);
    return i;
}

Node* Table::createNewID(int defaultId) {
    int i = 0;
    if (defaultId == 0) i = findNewID();
    else if (id > 0) i = defaultId;
    else throw invalid_argument("ID can't be negative");
    Field *idField = new Field(i);
    if (C[0]->getNode(Operator::Equal, idField) != nullptr) throw invalid_argument("Duplicate ID not allowed");
    Node *idNode = C[0]->insertField(idField);
    return idNode;
}

int Table::getColumnIdx(string title) {
    if (CTitles.find(title) == CTitles.end()) throw invalid_argument("Column with this title doesn't exists");
    return CTitles[title];
}

Column* Table::getColumn(string title) {
    return C[getColumnIdx(title)];
}

Type* Table::getColumnsTypes() {
    Type* types = new Type[c - 1];
    for (int i = 1; i < c; i++) types[i - 1] = C[i]->type;
    return types;
}

vector<string> Table::getColumnTitles() {
    vector<string> v;
    for (int i = 0; i < c; i++) v.push_back(C[i]->title);
    return v;
}

Record* Table::getRecordFromNode(int nodeColumnIdx, Node *node) {
    Field** fields = new Field* [c];
    for (int i = 0; i < c; i++) {
        fields[(i + nodeColumnIdx) % c] = C[(i + nodeColumnIdx) % c]->getField(node);
        node = node->nextField;
    }
    return new Record(c, fields);
}

Record* Table::getCustomRecordFromRecord(vector<int> columnsIdx, Record *record) {
    int m = columnsIdx.size();
    Field** fields = new Field* [m];
    for (int i = 0; i < m; i++) fields[i] = record->F[columnsIdx[i]];
    delete record;
    return new Record(m, fields);
}

void Table::print() {
    vector<Node *> v = C[0]->getNodes();
    for (int i = 0; i < n; i++) getRecordFromNode(0, v[i])->print();
}