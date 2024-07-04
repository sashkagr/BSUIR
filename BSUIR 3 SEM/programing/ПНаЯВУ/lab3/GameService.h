#ifndef LAB3_GAMESERVICE_H
#define LAB3_GAMESERVICE_H


#include "Item.h"
#include "vector"

using namespace std;

class GameService {
private:
    vector<Item*> items;
    int money = 0;
    int buyCount = 0;
public:
    GameService();
    void showInventory();
    void showPoisons();
    void showWeapons();
    void showResources();
    void useItemById(int itemId);
    void craftWeapon(int weaponId);
    void sellWeapon(int weaponId);
    void buyResources();
};


#endif //LAB3_GAMESERVICE_H
