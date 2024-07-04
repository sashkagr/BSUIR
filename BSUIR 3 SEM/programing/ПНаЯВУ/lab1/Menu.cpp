#include "Menu.h"
#include <iostream>
#include <limits>

using namespace std;

void Menu::logOut() {
    run();
}
void Menu::run() {
    system("pause");
    system("cls");
        int choose=getUserSelect(1);
        switch (choose) {
            case 0:
                generateRandomUsers();
                break;
            case 1:
                logIn();
                break;

            case 2:
                 signUp();
                 break;
            case 3:
                showAllUsers();
                break;
            case 4:
                exit();
                break;
        }
    system("cls");
}
void Menu::logIn() {

    string login,password;
    cout<<"Enter login:\n";
    cin>>login;
    cout<<"Enter password:\n";
    cin>>password;
    if(userService->logIn(login,password)) {
        cout<<"Log in was successful.\n";
        currentLogin =login;
        showInnerMethod();
    }
    else
    {
        cout<<"Log in failed.\n";
        run();
    }

}
void Menu::signUp() {
    string login,password;
    cout<<"Enter login (a-z A-Z 0-9 min 3 max 20):\n";
    login = inputHelper->inputString("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890", 3, 20);
    cout<<"Enter password (a-z A-Z 0-9 min 8 max 16):\n";
    password = inputHelper->inputString("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890", 8, 16);
    if(userService->signUp(login,password)) {
        cout<<"Sign up was successful.\n";
        currentLogin =login;
        showInnerMethod();
    }
    else
    {
        cout<<"Sign up failed.\n";
        run();
    }
}
void Menu::changePassword() {
    string newPassword;
    cout<<"Enter password (a-z A-Z 0-9 min 8 max 16):\n";
    newPassword = inputHelper->inputString("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890", 8, 16);
    userService->changePassword(currentLogin,newPassword);
    cout<<"Password was changed:\n";
    showInnerMethod();
}
void Menu::changeLogin() {
    string newLogin;
    cout<<"Enter login (a-z A-Z 0-9 min 3 max 20):\n";
    newLogin = inputHelper->inputString("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890", 3, 20);
    if(userService->changeLogin(currentLogin,newLogin)) {
        currentLogin = newLogin;
        cout<<"Login change was successful.\n";
    }
    else
    {
        cout<<"Login change failed.\n";
    }
    showInnerMethod();
}
int Menu::getUserSelect(int layer) {
    if(layer==1)
    cout<<"Enter choose:\n0)generate random users\n1)log in\n2)sign up\n3)show all users\n4)exit\n";
    else
    cout<<"Enter choose:\n1)change password\n2)change login\n3)log out\n";
     int choose=-1;
     if (layer==1)
     {
             choose = inputHelper->inputInt(0, 4);
     }
     else {
             choose = inputHelper->inputInt(1, 3);
     }
    return choose;
}
void Menu::exit() {
  ::exit(0);
}
void Menu::showInnerMethod() {
    system("pause");
    system("cls");
    int choose=getUserSelect(2);
    switch (choose) {
        case 1: {
            changePassword();
            break;
        }
        case 2:{
            changeLogin();
            break;
        }
        case 3: {
            logOut();
            break;
        }
    }
    system("cls");
}

void Menu::generateRandomUsers() {
    system("cls");
    cout << "enter number of users: \n";
    int usersNo = inputHelper->inputInt(1, 100);
    for (int i = 0; i < usersNo; i++)
    {
        string login, password;
        do {
            login = randomCredentialsGenerator->generateLogin();
            password = randomCredentialsGenerator->generatePassword(16);
        } while (userService->signUp(login, password) != true);
        cout << "login: " << login << " password: " << password << endl;
    }
    cout<<"Sign up was successful.\n";
    run();
}

void Menu::showAllUsers() {
    system("cls");
    userService->showAllUsers();
    run();
}
