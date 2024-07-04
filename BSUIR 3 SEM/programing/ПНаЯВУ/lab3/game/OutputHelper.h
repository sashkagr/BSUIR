#ifndef LAB3_OUTPUTHELPER_H
#define LAB3_OUTPUTHELPER_H

#include <string>

using namespace std;

class OutputHelper {
public:
    static string underline(string str);
    static string blinkingColor(int color, string str);
    static string colorizeText(int color, string str);
    static string intToStr(int value, int leadingZeros);
};


#endif //LAB3_OUTPUTHELPER_H
