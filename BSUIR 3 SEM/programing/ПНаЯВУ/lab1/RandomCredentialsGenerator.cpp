#include "RandomCredentialsGenerator.h"
#include <ctime>
#include <cstdlib>

RandomCredentialsGenerator::RandomCredentialsGenerator() {
    allowedCharacters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
    userNumber = 1;
    srand(time(0));  // Seed the random number generator with the current time
}

std::string RandomCredentialsGenerator::generateLogin() {
    std::string login = "user" + std::to_string(userNumber);
    userNumber++;  // Increment userNumber for the next user
    return login;
}

std::string RandomCredentialsGenerator::generatePassword(int length) {
    std::string password;
    for (int i = 0; i < length; ++i) {
        int randomIndex = rand() % allowedCharacters.size();
        password += allowedCharacters[randomIndex];
    }
    return password;
}
