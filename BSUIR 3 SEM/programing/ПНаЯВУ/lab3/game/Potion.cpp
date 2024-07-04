//
// Created by sashk on 30.09.2023.
//

#include "Potion.h"
#include <iostream>
#include "InputHelper.h"
#include <ctime>

using namespace std;

int Potion::useItem() {
    system("cls");
    int result = 0;
    if (this->getAmount() == 0)
    {
        cout << "Bottle is empty" << endl;
        system("pause");
    }
    else {
        cout << description << endl << "Do you want to use it?" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        int option = InputHelper::inputInt(1, 2);
        if (option == 1) {
            switch (getId()) {
                case 9:
                    result = giveMoney(); break;
                case 10:
                    result = 1;
                    break;
                case 11: result = giveRandomItem();
                    break;
                case 12:
                    suicidePotion();
                    break;
            }
            this->setAmount(0);
        }
    }
    return result;
}

Potion::Potion(int id, string name, int maxAmount, int amount, float weight, string description) : Item(id, name, maxAmount, amount,
                                                                                           weight), description(description) {}



void Potion::suicidePotion() {
    cout << "GAME OVER" << endl;
    system("pause");
    exit(0);
}

int Potion::giveMoney() {
    srand(time(0));
    return (rand() % 400) + 100;
}

int Potion::giveRandomItem() {
    srand(time(0));
    return rand() % 8;
}
