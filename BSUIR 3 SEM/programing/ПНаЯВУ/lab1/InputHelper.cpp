#include "InputHelper.h"
#include <iostream>
#include <limits>

using namespace std;

int InputHelper::inputInt(int min, int max) {
    int integer;
    while (true)
    {
        if (cin >> integer)
        {
            if (!(min == 0 && max == 0))
            {
                if (integer >= min && integer<=max) break;
                else {
                    clearCin(min, max);
                }
            }
            else break;
        }
        else clearCin(min, max);
    }

    return integer;
}

void InputHelper::clearCin(int min, int max) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (min == 0 && max == 0)
        cout << "Invalid input. Please enter a new one." << endl;
    else cout << "Invalid input. Please enter a new one. In range/length from " << min << " to " << max << endl;
}

string InputHelper::inputString(string allowedCharacters, int minLength, int maxLength) {
    string str;
    while (true)
    {
        if (cin >> str)
        {
            if (cin.peek() == ' ') clearCin(minLength, maxLength);
            else if (minLength != 0 && maxLength != 0 && (minLength > str.size() || maxLength < str.size()))
            {
                clearCin(minLength, maxLength);
            }
            else if (allowedCharacters != "")
            {
                if (containsOnlyAllowedCharacters(str, allowedCharacters))
                    break;
                else clearCin(0, 0);
            }
            else break;
        }
        else clearCin(0, 0);
    }

    return str;
}

bool InputHelper::containsOnlyAllowedCharacters(string input, string allowedCharacters) {
    for (char c : input) {
        if (allowedCharacters.find(c) == string::npos) {
            return false;
        }
    }
    return true;
}
