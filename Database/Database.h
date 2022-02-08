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
    unordered_map<string, Table*> T;
    pair<string, string> splitDataAndType(string q);
    vector<string> extractDataVector(string q);
    Table *getTable(string name);
    void createTable(string name, int column, string* titles, Type* types);
    void insertToTable(string tableName, string *data, int c);
    vector<Record *> selectOneFromTable(string tableName, vector<string> columns, string column, Operator op, string value);
    vector<Record *> selectTwoFromTable(string tableName, vector<string> columns,
                                        string column1, Operator op1, string value1,
                                        string column2, Operator op2, string value2,
                                        char mergeOp);
    vector<int> deleteOneFromTable(string tableName, string column, Operator op, string value);
    vector<int> deleteTwoFromTable(string tableName,
                                   string column1, Operator op1, string value1,
                                   string column2, Operator op2, string value2,
                                   char mergeOp);
    void updateOneFromTable(string tableName, string *data, int c, string column, Operator op, string value);
    void updateTwoFromTable(string tableName, string *data, int c,
                     string column1, Operator op1, string value1,
                     string column2, Operator op2, string value2,
                     char mergeOp);
public:
    void query(string q);
};


#endif //SQL_DATABASE_H
