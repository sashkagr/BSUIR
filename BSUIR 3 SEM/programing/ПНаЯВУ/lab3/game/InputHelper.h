#ifndef LAB1_INPUTHELPER_H
#define LAB1_INPUTHELPER_H

#include <string>

using namespace std;

class InputHelper {
public :
    static int inputInt(int min, int max);
    static string inputString(string allowedCharacters, int minLength, int maxLength);
private:
    static void clearCin(int min, int max);
    static bool containsOnlyAllowedCharacters(string input, string allowedCharacters);
};


#endif //LAB1_INPUTHELPER_H
