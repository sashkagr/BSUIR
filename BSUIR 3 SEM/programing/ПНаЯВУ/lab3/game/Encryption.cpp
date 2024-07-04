#include <vector>
#include "Encryption.h"

string Encryption::crypt(int key, string msg) {
    if (key <= 1 || msg.empty()) {
        return msg;
    }

    vector<std::string> rows(key, "");
    int direction = 1;
    int row = 0;

    for (char c : msg) {
        rows[row] += c;

        if (row == 0) {
            direction = 1;
        } else if (row == key - 1) {
            direction = -1;
        }
        row += direction;
    }

    std::string result;
    for (const std::string &row : rows) {
        result += row;
    }

    return result;
}
