//
// Created by sashk on 06.09.2023.
//

#ifndef LAB1_USERSERVICE_H
#define LAB1_USERSERVICE_H
#include <string>
#include "HashTable.h"
#include <memory>

using namespace std;

class UserService {
public:
bool logIn(string login,string password);
bool signUp(string login,string password);
bool changeLogin(string oldLogin, string newLogin);
void changePassword(string login,string password);
void showAllUsers();
private:
    unique_ptr<HashTable> hashTable = make_unique<HashTable>();
};


#endif //LAB1_USERSERVICE_H
