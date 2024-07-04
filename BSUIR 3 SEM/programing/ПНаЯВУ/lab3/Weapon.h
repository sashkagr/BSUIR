
#ifndef LAB3_WEAPON_H
#define LAB3_WEAPON_H


#include "Item.h"

class Weapon: public Item {
private:
    int woolAmount;
    int stoneAmount;
    int woodAmount;
    int sandAmount;
    int minPrice;
public:
    int getMinPrice() const;

    void setMinPrice(int minPrice);

    int getMaxPrice() const;

    void setMaxPrice(int maxPrice);

private:
    int maxPrice;
public:
    int getWoolAmount() const;

    void setWoolAmount(int woolAmount);

    int getStoneAmount() const;

    void setStoneAmount(int stoneAmount);

    int getWoodAmount() const;

    void setWoodAmount(int woodAmount);

    int getSandAmount() const;

    void setSandAmount(int sandAmount);

public:
    int useItem() override;
    Weapon(int id, string name, int maxAmount, int amount, float weight, int craftWool, int craftStone, int craftWood, int craftSand, int minPrice, int maxPrice);
    int craftWool;
    int craftStone;
    int craftWood;
    int craftSand;



};


#endif //LAB3_WEAPON_H
