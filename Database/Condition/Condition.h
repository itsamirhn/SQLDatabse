//
// Created by AmirMohammad Hosseini Nasab on 2022/2/4.
//

#ifndef SQL_CONDITION_H
#define SQL_CONDITION_H
#include "../Operator/Operator.h"
#include <iostream>
using namespace std;

class Condition {
public:
    string column;
    Operator op;
    string value;
    Condition(string _column, Operator _op, string _value) {
        column = _column;
        op = _op;
        value = _value;
    }
};


#endif //SQL_CONDITION_H
