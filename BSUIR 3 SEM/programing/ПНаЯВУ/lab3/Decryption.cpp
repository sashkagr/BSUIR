#include "Decryption.h"

string Decryption::crypt(int numRows, string ciphertext) {

    int len = ciphertext.length();
    string plaintext(len, ' ');

    int cycle = 2 * numRows - 2;
    int index = 0;

    for (int i = 0; i < numRows; i++) {
        int step = cycle - 2 * i;
        for (int j = i; j < len; j += cycle) {
            plaintext[j] = ciphertext[index++];
            if (step > 0 && step < cycle && j + step < len)
                plaintext[j + step] = ciphertext[index++];
        }
    }

    return plaintext;
}