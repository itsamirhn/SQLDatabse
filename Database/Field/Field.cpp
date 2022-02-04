//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "Field.h"
#include <regex>

int Field::getHash() {
    if (type == Type::Integer) return stoi(data);
    int h = 0;
    for (int i = 0; i < (int)(data.size()); i++) h = (1LL * h * HASH_BASE % HASH_MOD + (int)(data[i])) % HASH_MOD;
    return h;
}

void Field::validateField() {
    regex rgx;
    if (type == Type::String) rgx = regex("^(\"|').*\\1+");
    if (type == Type::Integer) rgx = regex("^-?\\d+$");
    if (type == Type::Timestamp) rgx = regex("^\\d{4}\\/(0?[1-9]|1[012])\\/(0?[1-9]|[12][0-9]|3[01])$");
    if (!regex_match(data, rgx)) throw invalid_argument("Data doesn't match with type");
}