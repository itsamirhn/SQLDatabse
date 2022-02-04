//
// Created by AmirMohammad Hosseini Nasab on 2022/2/4.
//

#include "Utils.h"

Type convertStringToType(string s) {
    if (s == "string") return Type::String;
    if (s == "int") return Type::Integer;
    if (s == "timestamp") return Type::Timestamp;
    throw invalid_argument("Invalid Type");
}


Operator convertStringToOperator(string s) {
    if (s == "==" || s == "=") return Operator::Equal;
    if (s == ">=") return Operator::GreaterThanOrEqual;
    if (s == "<=") return Operator::LessThanOrEqual;
    if (s == ">") return Operator::GreaterThan;
    if (s == "<") return Operator::LessThan;
    throw invalid_argument("Invalid Operator");
}

Operator withEqual(Operator op) {
    if (op == Operator::LessThan) return Operator::LessThanOrEqual;
    if (op == Operator::GreaterThan) return Operator::GreaterThanOrEqual;
    return op;
}

bool checkOperator(Operator op, int a, int b) {
    if (op == Operator::Equal) return a == b;
    if (op == Operator::GreaterThanOrEqual) return a >= b;
    if (op == Operator::LessThan) return a < b;
    if (op == Operator::LessThanOrEqual) return a <= b;
    if (op == Operator::GreaterThan) return a > b;
    throw invalid_argument("Invalid Operator");
}

bool isAscending(Operator op) {
    return (op == Operator::GreaterThan || op == Operator::GreaterThanOrEqual);
}

bool isDescending(Operator op) {
    return (op == Operator::LessThan || op == Operator::LessThanOrEqual);
}