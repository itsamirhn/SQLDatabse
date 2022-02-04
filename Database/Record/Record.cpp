//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "Record.h"

void Record::print() {
    for (int i = 0; i < c; i++) cout << F[i]->data << ' ';
    cout << endl;
}