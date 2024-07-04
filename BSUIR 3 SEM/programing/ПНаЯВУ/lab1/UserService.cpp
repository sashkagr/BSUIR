#include "UserService.h"

bool UserService::logIn(string login, string password)
{
    string userPassword= hashTable->search(login);
    if(userPassword.empty())
    return false;
    else
    {
        if(password==userPassword)
            return true;
    }
    return false;
}

bool UserService::signUp(string login, string password) {
    string userPassword= hashTable->search(login);
    if(!userPassword.empty())
        return false;
    else{
      hashTable->add(login,password);
        return true;
    }
}

bool UserService::changeLogin(string oldLogin, string newLogin) {
    string userPassword= hashTable->search(newLogin);
    if(!userPassword.empty())
        return false;
    else {
        hashTable->ubdateKey(oldLogin, newLogin);
        return true;
    }
}

void UserService::changePassword(string login, string password) {
   hashTable->ubdateValue(login,password);
}

void UserService::showAllUsers() {
    hashTable->printUnhashedKeyAndValue();
}
