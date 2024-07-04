
#include "User.h"
#include <iostream>

void User::setLogin(string login) {
    this->login = login;
}

void User::setPassword(string password) {
    this->password = password;
}

ostream &operator<<(ostream &output, const User &D) {
    output << "User: " << D.login << " Password: " << D.password;
    return output;
}

void User::setNewLogin(string newLogin) {
    this->newLogin = newLogin;
}

void User::setNewPassword(string newPassword) {
    this->newPassword = newPassword;
}

string User::getLogin() {
    return this->login;
}

string User::getPassword() {
    return this->password;
}

string User::getNewLogin() {
    return this->newLogin;
}

string User::getNewPassword() {
    return this->newPassword;
}
