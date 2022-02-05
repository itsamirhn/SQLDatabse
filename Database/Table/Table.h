//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#ifndef SQL_TABLE_H
#define SQL_TABLE_H
#include "../Record/Record.h"
#include "../Column/Column.h"
#include "../Condition/Condition.h"
#include <iostream>
#include <set>
using namespace std;

class Table {
    string name;
    int n, id;
    Column** C;
    unordered_map<string, int> CTitles;
    set<int> removedIds;
    Record* getRecordFromNode(int nodeColumnIdx, Node *node);
    Record* getCustomRecordFromRecord(vector<int> columnsIdx, Record *record);
public:
    int c;
    Table(string _name, int _c, string* columnTitles, Type* columnTypes) {
        name = _name;
        n = 0, id = 1, c = _c + 1;
        C = new Column* [c];
        C[0] = new Column("ID", Type::Integer);
        CTitles["ID"] = 0;
        for (int i = 0; i < c - 1; i++) {
            if (columnTitles[i] == "ID") throw invalid_argument("'ID' column title is reserved.");
            C[i + 1] = new Column(columnTitles[i], columnTypes[i]);
            CTitles[columnTitles[i]] = i + 1;
        }
    }
    int getColumnIdx(string title);
    Column* getColumn(string title);
    Type* getColumnsTypes();
    vector<string> getColumnTitles();
    void insert(Record *record, int defaultId = 0);
    vector<Record* > select(vector<string> columns, Condition* condition);
    vector<int> remove(Condition* condition);
    int findNewID();
    Node *createNewID(int defaultId = 0);
    void print();
};


#endif //SQL_TABLE_H
