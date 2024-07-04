#ifndef LAB3_DECRYPTION_H
#define LAB3_DECRYPTION_H


#include "Cryption.h"

class Decryption : public Cryption {
    string crypt(int n, string str) override;
};


#endif //LAB3_DECRYPTION_H
