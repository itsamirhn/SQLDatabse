//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#ifndef SQL_RECORD_H
#define SQL_RECORD_H

#include "../Field/Field.h"

class Table;

class Record {
public:
    int c;
    Field** F;
    Record(int _c, Field** _F) {
        c = _c;
        F = _F;
    }
    Record(int _c, string* _data, Type* _types) {
        c = _c;
        F = new Field* [c];
        for (int i = 0; i < c; i++) F[i] = new Field(_data[i], _types[i]);
    }
    void print();
};


#endif //SQL_RECORD_H
