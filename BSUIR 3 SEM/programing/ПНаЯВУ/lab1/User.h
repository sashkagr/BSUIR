
#ifndef LAB1_USER_H
#define LAB1_USER_H

#include <string>

using namespace std;

class User {
private:
    string login;
    string password;
    string newLogin;
    string newPassword;
public:
    void setLogin(string login);
    void setPassword(string password);
    void setNewLogin(string newLogin);
    void setNewPassword(string newPassword);

    string getLogin();
    string getPassword();
    string getNewLogin();
    string getNewPassword();

    friend ostream &operator<<(ostream &output, const User &D);
};


#endif //LAB1_USER_H
