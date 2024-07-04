#include "Menu.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Hello, you have received an inheritance!" << endl;
    cout << "Now you are a merchant! Buy resources, make weapons and sell them." << endl;
    cout << "Have a good business!" << endl;
    system("pause");

    Menu menu;

    menu.mainMenu();

    system("pause");
    return 0;
}
