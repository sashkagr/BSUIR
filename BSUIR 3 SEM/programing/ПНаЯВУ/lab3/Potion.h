#ifndef LAB3_POTION_H
#define LAB3_POTION_H


#include "Item.h"
#include <string>

class Potion: public Item{
private:
    string description;

    void suicidePotion();
    int giveMoney();
    int giveRandomItem();
public:
    int useItem() override;
    Potion(int id, string name, int maxAmount, int amount, float weight, string description);

};


#endif //LAB3_POTION_H
