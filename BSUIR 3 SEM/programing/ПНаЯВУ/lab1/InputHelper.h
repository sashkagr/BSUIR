#ifndef LAB1_INPUTHELPER_H
#define LAB1_INPUTHELPER_H

#include <string>

using namespace std;

class InputHelper {
public :
    int inputInt(int min, int max);
    string inputString(string allowedCharacters, int minLength, int maxLength);
private:
    void clearCin(int min, int max);
    bool containsOnlyAllowedCharacters(string input, string allowedCharacters);
};


#endif //LAB1_INPUTHELPER_H
