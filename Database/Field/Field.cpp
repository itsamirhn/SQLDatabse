//
// Created by AmirMohammad Hosseini Nasab on 2022/1/28.
//

#include "Field.h"
#include <regex>

int Field::getHash() {
    if (type == Type::Integer) return stoi(data);
    if (type == Type::Timestamp) {
        smatch matches;
        regex timeRgx("^(\\d{4})\\/(0?[1-9]|1[012])\\/(0?[1-9]|[12][0-9]|3[01])$");
        if (regex_search(data, matches, timeRgx)) {
            int year = stoi(matches[1].str());
            int month = stoi(matches[2].str());
            int day = stoi(matches[3].str());
            return 372 * year + 31 * month + day;
        }
        throw invalid_argument("Invalid Date.");
    }
    if (type == Type::String) {
        int h = 0;
        for (int i = 1; i + 1 < (int)(data.size()); i++) h = (1LL * h * HASH_BASE % HASH_MOD + (int)(data[i])) % HASH_MOD;
        return h;
    }
    throw invalid_argument("Invalid Type.");
}

void Field::validateField() {
    regex rgx;
    if (type == Type::String) rgx = regex("^(\"|').*\\1+");
    if (type == Type::Integer) rgx = regex("^-?\\d+$");
    if (type == Type::Timestamp) rgx = regex("^\\d{4}\\/(0?[1-9]|1[012])\\/(0?[1-9]|[12][0-9]|3[01])$");
    if (!regex_match(data, rgx)) throw invalid_argument("Data doesn't match with type");
}

string Field::represent() {
    if (type == Type::Integer) return data;
    if (type == Type::Timestamp) return data;
    if (type == Type::String) {
        string tmp = "";
        for (int i = 1; i + 1 < (int)(data.size()); i++) tmp += data[i];
        return tmp;
    }
    throw invalid_argument("Invalid Type.");
}