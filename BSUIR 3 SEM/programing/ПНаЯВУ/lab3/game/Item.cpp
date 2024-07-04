
#include "Item.h"

Item::Item(int id, string name, int maxAmount, int amount, float weight) : id(id), name(name),
                                                                                  maxAmount(maxAmount), amount(amount),
                                                                                  weight(weight) {}

int Item::getId()  {
    return id;
}

void Item::setId(int id) {
    Item::id = id;
}

 string Item::getName()  {
    return name;
}

void Item::setName( string name) {
    Item::name = name;
}

int Item::getMaxAmount()  {
    return maxAmount;
}

void Item::setMaxAmount(int maxAmount) {
    Item::maxAmount = maxAmount;
}

int Item::getAmount()  {
    return amount;
}

void Item::setAmount(int amount) {
    Item::amount = amount;
}

float Item::getWeight()  {
    return weight;
}

void Item::setWeight(float weight) {
    Item::weight = weight;
}
