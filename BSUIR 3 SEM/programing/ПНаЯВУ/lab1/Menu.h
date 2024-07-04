//
// Created by sashk on 06.09.2023.
//

#ifndef LAB1_MENU_H
#define LAB1_MENU_H

#include <memory>
#include "UserService.h"
#include "RandomCredentialsGenerator.h"
#include "InputHelper.h"

class Menu {
public:
    void run();
private:
    void showInnerMethod();
   void logIn();
   void signUp();
   void logOut();
   void changePassword();
   void changeLogin();
   int getUserSelect(int layer);
   void exit();
   void generateRandomUsers();
   void showAllUsers();

    unique_ptr<UserService >userService= std::make_unique<UserService>();
    unique_ptr<RandomCredentialsGenerator> randomCredentialsGenerator = std::make_unique<RandomCredentialsGenerator>();
    unique_ptr<InputHelper> inputHelper = std::make_unique<InputHelper>();
    string currentLogin;
};


#endif //LAB1_MENU_H
