
#include <iostream>
#include "Weapon.h"
#include "GameService.h"

int Weapon::useItem() {
    if (craftWood > woodAmount) return 0;
    if (craftWool > woolAmount) return 0;
    if (craftSand > sandAmount) return 0;
    if (craftStone > stoneAmount) return 0;
    return 1;
}

Weapon::Weapon(int id, string name, int maxAmount, int amount, float weight, int craftWool, int craftStone, int craftWood, int craftSand, int minPrice, int maxPrice) : Item(id, name, maxAmount, amount,
                                                                                           weight), craftWool(craftWool), craftStone(craftStone), craftWood(craftWood), craftSand(craftSand)
                                                                                           , minPrice(minPrice), maxPrice(maxPrice) {}

int Weapon::getWoolAmount() const {
    return woolAmount;
}

void Weapon::setWoolAmount(int woolAmount) {
    Weapon::woolAmount = woolAmount;
}

int Weapon::getStoneAmount() const {
    return stoneAmount;
}

void Weapon::setStoneAmount(int stoneAmount) {
    Weapon::stoneAmount = stoneAmount;
}

int Weapon::getWoodAmount() const {
    return woodAmount;
}

void Weapon::setWoodAmount(int woodAmount) {
    Weapon::woodAmount = woodAmount;
}

int Weapon::getSandAmount() const {
    return sandAmount;
}

void Weapon::setSandAmount(int sandAmount) {
    Weapon::sandAmount = sandAmount;
}

int Weapon::getMinPrice() const {
    return minPrice;
}

void Weapon::setMinPrice(int minPrice) {
    Weapon::minPrice = minPrice;
}

int Weapon::getMaxPrice() const {
    return maxPrice;
}

void Weapon::setMaxPrice(int maxPrice) {
    Weapon::maxPrice = maxPrice;
}
