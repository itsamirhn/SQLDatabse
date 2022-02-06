//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#ifndef SQL_DATABASE_H
#define SQL_DATABASE_H

#include "Table/Table.h"
#include "Operator/Operator.h"
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

class Database {
    int n;
    unordered_map<string, Table*> T;
    pair<string, string> splitDataAndType(string q);
    vector<string> extractDataVector(string q);
    Table *getTable(string name);
    void createTable(string name, int column, string* titles, Type* types);
    void insertToTable(string tableName, string *data);
    vector<Record *> selectOneFromTable(string tableName, vector<string> columns, string column, Operator op, string value);
    vector<Record *> selectTwoFromTable(string tableName, vector<string> columns,
                                        string column1, Operator op1, string value1,
                                        string column2, Operator op2, string value2,
                                        char mergeOp);
    vector<int> deleteFromTable(string tableName, string column, Operator op, string value);
    void updateTable(string tableName, string *data, string column, Operator op, string value);
public:
    void query(string q);
};


#endif //SQL_DATABASE_H
