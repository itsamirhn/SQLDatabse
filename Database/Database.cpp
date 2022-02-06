//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "Database.h"
#include "Utils/Utils.h"
#include "Condition/Condition.h"
#include <regex>

Table *Database::getTable(string name) {
    if (T.find(name) == T.end()) throw invalid_argument("Table with this name doesn't exists");
    return T[name];
}

void Database::createTable(string name, int column, string* titles, Type* types)  {
    if (T.find(name) != T.end()) throw invalid_argument("Table with this name already exists");
    Table* t = new Table(name, column, titles, types);
    T[name] = t;
}

void Database::insertToTable(string tableName, string *data) {
    Table *t = getTable(tableName);
    Record *r = new Record(t->c - 1, data, t->getColumnsTypes());
    t->insert(r);
}

vector<Record *> Database::selectOneFromTable(string tableName, vector<string> columns, string column, Operator op, string value) {
    Table *t = getTable(tableName);
    Condition* condition = new Condition(column, op, value);
    return t->selectOneCondition(columns, condition);
}

vector<Record *>
Database::selectTwoFromTable(string tableName, vector<string> columns, string column1, Operator op1, string value1,
                             string column2, Operator op2, string value2, char mergeOp) {
    Table *t = getTable(tableName);
    Condition* condition1 = new Condition(column1, op1, value1);
    Condition* condition2 = new Condition(column2, op2, value2);
    bool isAnd = false;
    if (mergeOp == '&') isAnd = true;
    else if (mergeOp == '|') isAnd = false;
    else throw invalid_argument("Invalid merging operator (& or |)");
    return t->selectTwoCondition(columns, condition1, condition2, isAnd);
}

void Database::deleteFromTable(string tableName, string column, Operator op, string value) {
    Table *t = getTable(tableName);
    Condition* condition = new Condition(column, op, value);
    t->remove(condition);
}

void Database::updateTable(string tableName, string *data, string column, Operator op, string value) {
    Table *t = getTable(tableName);
    Record *r = new Record(t->c - 1, data, t->getColumnsTypes());
    Condition* condition = new Condition(column, op, value);
    vector<int> ids = t->remove(condition);
    for (int id : ids) t->insert(r, id);
}

vector<string> Database::extractDataVector(string q) {
    smatch matches;
    regex commaSepRgx("\\s*(.+?)(?:,|$)");
    vector<string> v;
    while (regex_search(q, matches, commaSepRgx)) {
        v.push_back(matches[1].str());
        q = matches.suffix();
    }
    return v;
}

pair<string, string> Database::splitDataAndType(string q) {
    auto spacePos = q.find(' ');
    string data = q.substr(0, spacePos);
    q.erase(0, spacePos + 1);
    string type = q;
    return make_pair(data, type);
}

void Database::query(string q) {
    smatch matches;
    regex createRgx("^CREATE\\s+TABLE\\s+([a-zA-Z0-9]+)\\s+\\((.*)\\)$");
    if (regex_search(q, matches, createRgx)) {
        string tableName = matches[1].str();
        vector<string> v = extractDataVector(matches[2].str());
        int c = v.size();
        string *titles = new string [c];
        Type* types = new Type [c];
        for (int i = 0; i < c; i++) {
            pair<string, string> p = splitDataAndType(v[i]);
            titles[i] = p.first;
            types[i] = convertStringToType(p.second);
        }
        createTable(tableName, c, titles, types);
        return;
    }
    regex insertRgx("^INSERT\\s+INTO\\s+([a-zA-Z0-9]+)\\s+VALUES\\s+\\((.*)\\)$");
    if (regex_search(q, matches, insertRgx)) {
        string tableName = matches[1].str();
        vector<string> v = extractDataVector(matches[2].str());
        int c = v.size();
        string *data = new string [c];
        for (int i = 0; i < c; i++) data[i] = v[i];
        insertToTable(tableName, data);
        return;
    }
    regex select1Rgx("^SELECT\\s+(?:\\((.*)\\)|(\\*))\\s+FROM\\s+([a-zA-Z0-9]+)\\s+WHERE\\s+(\\w+)\\s*(==|>=|<=|>|<)\\s*(\\w+)$");
    if (regex_search(q, matches, select1Rgx)) {
        string tableName = matches[3].str();
        string conditionColumn = matches[4].str();
        string conditionOperator = matches[5].str();
        string conditionValue = matches[6].str();
        vector<string> columns;
        if (matches[2].str() == "*") columns = getTable(tableName)->getColumnTitles();
        else columns = extractDataVector(matches[1].str());
        vector<Record *> result = selectOneFromTable(tableName, columns, conditionColumn,
                                                     convertStringToOperator(conditionOperator), conditionValue);
        for (Record* record : result) record->print();
        return;
    }
    regex select2Rgx("^SELECT\\s+(?:\\((.*)\\)|(\\*))\\s+FROM\\s+([a-zA-Z0-9]+)\\s+WHERE\\s+(\\w+)\\s*(==|>=|<=|>|<)\\s*(.+)\\s+([&|])\\s+(\\w+)\\s*(==|>=|<=|>|<)\\s*(\\w+)$");
    if (regex_search(q, matches, select2Rgx)) {
        string tableName = matches[3].str();
        string conditionColumn1 = matches[4].str();
        string conditionOperator1 = matches[5].str();
        string conditionValue1 = matches[6].str();
        char mergeOperator = matches[7].str()[0];
        string conditionColumn2 = matches[8].str();
        string conditionOperator2 = matches[9].str();
        string conditionValue2 = matches[10].str();
        vector<string> columns;
        if (matches[2].str() == "*") columns = getTable(tableName)->getColumnTitles();
        else columns = extractDataVector(matches[1].str());
        vector<Record *> result = selectTwoFromTable(tableName, columns,
                                                     conditionColumn1, convertStringToOperator(conditionOperator1), conditionValue1,
                                                     conditionColumn2, convertStringToOperator(conditionOperator2), conditionValue2,
                                                     mergeOperator);
        for (Record* record : result) record->print();
        return;
    }
    regex deleteRgx("^DELETE\\s+FROM\\s+([a-zA-Z0-9]+)\\s+WHERE\\s+(\\w+)\\s*(==|>=|<=|>|<)\\s*(\\w+)$");
    if (regex_search(q, matches, deleteRgx)) {
        string tableName = matches[1].str();
        string conditionColumn = matches[2].str();
        string conditionOperator = matches[3].str();
        string conditionValue = matches[4].str();
        deleteFromTable(tableName, conditionColumn, convertStringToOperator(conditionOperator), conditionValue);
        return;
    }
    regex updateRgx("^UPDATE\\s+([a-zA-Z0-9]+)\\s+SET\\s+\\((.*)\\)\\s+WHERE\\s+(\\w+)\\s*(==|>=|<=|>|<)\\s*(\\w+)$");
    if (regex_search(q, matches, updateRgx)) {
        string tableName = matches[1].str();
        vector<string> v = extractDataVector(matches[2].str());
        string conditionColumn = matches[3].str();
        string conditionOperator = matches[4].str();
        string conditionValue = matches[5].str();
        int c = v.size();
        string *data = new string [c];
        for (int i = 0; i < c; i++) data[i] = v[i];
        updateTable(tableName, data, conditionColumn, convertStringToOperator(conditionOperator), conditionValue);
        return;
    }
    regex printRgx("PRINT\\s+([a-zA-Z0-9]+)");
    if (regex_search(q, matches, printRgx)) {
        string tableName = matches[1].str();
        getTable(tableName)->print();
        return;
    }
    throw invalid_argument("Invalid Query!");
}