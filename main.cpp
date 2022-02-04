#include <iostream>
#include "Database/Database.h"
using namespace std;

int main() {

    Database db;
    int n; cin >> n;
    string q; getline(cin, q);
    while (n--) {
        getline(cin, q);
        db.query(q);
    }

    return 0;
}
