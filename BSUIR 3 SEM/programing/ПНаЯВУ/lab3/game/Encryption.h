#ifndef LAB3_ENCRYPTION_H
#define LAB3_ENCRYPTION_H

#include "Cryption.h"

class Encryption : public Cryption {
    string crypt(int n, string str) override;
};


#endif //LAB3_ENCRYPTION_H
