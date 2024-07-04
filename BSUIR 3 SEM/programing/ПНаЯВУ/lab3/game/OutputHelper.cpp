#include "OutputHelper.h"

using namespace std;

string OutputHelper::underline(string str) {
    return "\033[4m" + str + "\033[0m\t";
}

string OutputHelper::blinkingColor(int color, string str) {
    return "\033[5;" + to_string(color) + "m" + str + "\033[0m\t";
}

string OutputHelper::colorizeText(int color, string str) {
    return "\033[" + to_string(color) + "m" + str + "\033[0m\t";
}

string OutputHelper::intToStr(int value, int leadingZeros) {
    std::string str = std::to_string(value);
    if (str.length() < leadingZeros) {
        int numZeros = leadingZeros - str.length();
        std::string zeros(numZeros, '0');
        str = zeros + str;
    }
    return str;
}
