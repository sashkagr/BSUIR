//
// Created by sashk on 30.09.2023.
//

#ifndef LAB3_RESOURCE_H
#define LAB3_RESOURCE_H


#include "Item.h"

class Resource: public Item {
public:
    Resource(int id, string name, int maxAmount, int amount, float weight, int minPrice, int maxPrice);

private:
    int useItem() override;
    int minPrice;
    int maxPrice;
    int price;
    int optAmount;
public:
    int getPrice();
    int getRandomPrice();
    int getOptAmount();
    int getRandomOptAmount();
    void setOptAmount(int amount);

};


#endif //LAB3_RESOURCE_H
