#include "GameService.h"
#include "Weapon.h"
#include "Resource.h"
#include "Potion.h"
#include <iostream>
#include <unistd.h>
#include "InputHelper.h"
#include "OutputHelper.h"
#include <ctime>

using namespace std;

GameService::GameService() {
    items.push_back(new Weapon(1, "Sword", 64, 0, 2, 0, 3, 1, 0, 20, 60));
    items.push_back(new Weapon(2, "Axe", 64, 0, 1, 0, 1, 1, 0, 15, 25));
    items.push_back(new Weapon(3, "Trident", 64, 0, 2, 0, 0, 0, 3, 10, 20));
    items.push_back(new Weapon(4, "Crossbow", 64, 0, 4, 1, 0, 2, 0, 18 , 30));
    items.push_back(new Resource(5, "Wool", 64, rand() % 64, 5, 0, 7));
    sleep(1);
    items.push_back(new Resource(6, "Stone", 64, rand() % 64, 30, 0, 10));
    sleep(1);
    items.push_back(new Resource(7, "Wood", 64, rand() % 64, 9, 0, 8));
    sleep(1);
    items.push_back(new Resource(8, "Sand", 64, rand() % 64, 15, 0, 5));
    items.push_back(new Potion(9, "Potion of Money", 1, 1, 0.1, "You will receive random amount of coins between 100 and 500"));
    items.push_back(new Potion(10, "Potion of Restart", 1, 1, 0.1, "You will wake up with random inheritance and will start a new life"));
    items.push_back(new Potion(11, "Potion of Random item", 1, 1, 0.1, "You will receive one random item in your inventory"));
    items.push_back(new Potion(12, "Potion of Suicide", 1, 1, 0.1, "You will commit suicide game will be over"));
    srand(time(0));
    money = (rand() % 100) + 100;
}

void GameService::showInventory() {


    cout << "\t\t\t\t\t\033[1;5;32mCOINS " << money << " $\033[0m\t" << endl;
    cout << "Inventory: " << endl << endl;
    float totalWeight = 0;
    for (int i = 0; i < items.size(); i++)
    {
        if (i == 4 || i == 8) cout << "---------------------------------------" << endl;
        cout << items.at(i)->getId() << " "
             << items.at(i)->getName() << " ("
             << items.at(i)->getAmount() << "/"
             << items.at(i)->getMaxAmount() << ")"
             << endl;
        float itemWeight = items.at(i)->getWeight() * items.at(i)->getAmount();
        totalWeight += itemWeight;
    }
    cout << endl;
    cout << "Weight: " << totalWeight << " kg" << endl;
    cout << endl;

}

void GameService::useItemById(int itemId) {
    int result = items.at(itemId-1)->useItem();
    if (itemId == 9)
    {
        money += result;
    }
    if (itemId == 10 && result == 1)
    {
        system("cls");
        cout << "Hello, you have received an inheritance!" << endl;
        cout << "Now you are a merchant! Buy resources, make weapons and sell them." << endl;
        cout << "Have a good business!" << endl;
        system("pause");
        srand(time(0));
        money = (rand() % 100) + 100;
        for (int i = 0; i <= 3; i++)
        {
            items.at(i)->setAmount(0);
        }
        for (int i = 4; i <= 7; i++)
        {
            sleep(1);
            items.at(i)->setAmount(rand() % 64);
        }
    }
    if (itemId == 11)
    {
        srand(time(0));
        int maxRandAmount = 64 - items.at(result)->getAmount();
        int randAmount = rand() % maxRandAmount;
        items.at(result)->setAmount(items.at(result)->getAmount() + randAmount);
    }
}

void GameService::showPoisons() {
    cout << "Potions: " << endl;
    for (int i = 8; i < items.size(); i++)
    {
        string str = to_string(items.at(i)->getId()) + " "
                   + items.at(i)->getName() + " ("
                   + to_string(items.at(i)->getAmount()) + "/"
                   + to_string(items.at(i)->getMaxAmount()) + ")";
        cout << OutputHelper::blinkingColor(23+i, str) << endl;
//        cout << items.at(i)->getId() << " "
//             << items.at(i)->getName() << " ("
//             << items.at(i)->getAmount() << "/"
//             << items.at(i)->getMaxAmount() << ")"
//             << endl;
    }
}

void GameService::showWeapons() {
    cout << "Weapons: " << endl;
    for (int i = 0; i <= 3; i++)
    {
        cout << items.at(i)->getId() << " "
             << items.at(i)->getName() << " ("
             << items.at(i)->getAmount() << "/"
             << items.at(i)->getMaxAmount() << ")"
             << endl;
    }
}

void GameService::craftWeapon(int weaponId) {
    Weapon* weapon = (Weapon*) items.at(weaponId);
    cout << "Enter amount: ";
    int craftAmount = InputHelper::inputInt(1, 64 - weapon->getAmount());
    system("cls");
    for (int i = 1; i <= craftAmount; i++) {
        weapon->setSandAmount(items.at(7)->getAmount());
        weapon->setStoneAmount(items.at(5)->getAmount());
        weapon->setWoodAmount(items.at(6)->getAmount());
        weapon->setWoolAmount(items.at(4)->getAmount());
        int isCrafted = weapon->useItem();
        if (isCrafted == 1 && weapon->getAmount() != 64) {
            items.at(4)->setAmount(items.at(4)->getAmount() - weapon->craftWool);
            items.at(5)->setAmount(items.at(5)->getAmount() - weapon->craftStone);
            items.at(6)->setAmount(items.at(6)->getAmount() - weapon->craftWood);
            items.at(7)->setAmount(items.at(7)->getAmount() - weapon->craftSand);
            weapon->setAmount(weapon->getAmount() + 1);
            cout << "Weapon" << i << " crafted" << endl;
        } else {
            cout << "Weapon " << i << " : Not enough resources or inventory is full" << endl;
        }
    }
}

void GameService::sellWeapon(int weaponId) {
    Weapon* weapon = (Weapon*) items.at(weaponId);
    if (weapon->getAmount() == 0)
    {
        cout << "Not enough weapons to sell" << endl;
        system("pause");
        return;
    }
    cout << "Enter amount (0 to back)";
    int sellAmount = InputHelper::inputInt(0, weapon->getAmount());
    if (sellAmount == 0) return;
    srand(time(0));
    for (int i = 0; i < 3; i++)
    {
        int sellPrice = (rand() % (weapon->getMaxPrice() - weapon->getMinPrice())) + weapon->getMinPrice();
        int totalPrice = sellPrice * sellAmount;
        cout << "You can sell weapon for " << sellPrice << " coins (total: " << totalPrice << " coins)" << endl;
        cout << "Are you agree?" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        int option = InputHelper::inputInt(1, 2);
        if (option == 1 || i == 2)
        {
            weapon->setAmount(weapon->getAmount() - sellAmount);
            money += totalPrice;
            cout << "Congratulations! Weapon sold" << endl;
            system("pause");
            break;
        }
    }
}

void GameService::buyResources() {
    srand(time(0));

    if (buyCount == 0)
    {
        for (int i = 4; i <= 7; i++)
        {
            sleep(1);
            ((Resource*) items.at(i))->getRandomOptAmount();
            ((Resource*) items.at(i))->getRandomPrice();
        }
        buyCount = 2;
    }

    for (int i = 4; i <= 7; i++)
    {

            cout << items.at(i)->getId() << " "
                 << items.at(i)->getName() << " \t\topt amount: "
                 << ((Resource *) items.at(i))->getOptAmount() << " \t\tprice for one: "
                 << ((Resource *) items.at(i))->getPrice() << " \t\ttotal price: "
                 << ((Resource *) items.at(i))->getPrice() * ((Resource *) items.at(i))->getOptAmount()
                 << endl;
    }
    cout << "9 Back" << endl;
    cout << "Enter option: ";
    int resourceId = InputHelper::inputInt(4, 9);
    if (resourceId == 9) return;
    resourceId--;
    Resource* resource = ((Resource *) items.at(resourceId));
    int totalPrice = resource->getPrice() * resource->getOptAmount();
    if (totalPrice <= money && resource->getOptAmount() > 0)
    {
        resource->setAmount(resource->getAmount() + resource->getOptAmount());
        resource->setOptAmount(0);
        money-=totalPrice;
        buyCount--;
    }
    else
    {
        cout << "Not enough money or resource" << endl;
        system("pause");
    }
}

void GameService::showResources() {
    cout << "Resources: " << endl;
    for (int i = 4; i <= 7; i++)
    {
        cout << items.at(i)->getId() << " "
             << items.at(i)->getName() << " ("
             << items.at(i)->getAmount() << "/"
             << items.at(i)->getMaxAmount() << ")"
             << endl;
    }
}
