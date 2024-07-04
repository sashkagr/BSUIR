#ifndef LAB3_CRYPTION_H
#define LAB3_CRYPTION_H

#include <string>

using namespace std;

class Cryption
{
public:
    virtual string crypt(int n, string str) = 0;
};

#endif //LAB3_CRYPTION_H
