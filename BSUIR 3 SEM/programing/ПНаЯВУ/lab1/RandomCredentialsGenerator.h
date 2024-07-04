
#ifndef LAB1_RANDOMCREDENTIALSGENERATOR_H
#define LAB1_RANDOMCREDENTIALSGENERATOR_H

#include <string>

using namespace std;

class RandomCredentialsGenerator {
public:
    RandomCredentialsGenerator();  // Constructor
    string generateLogin();
    string generatePassword(int length);

private:
    string allowedCharacters;
    int userNumber;
};


#endif //LAB1_RANDOMCREDENTIALSGENERATOR_H
