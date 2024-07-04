
//
// Created by sashk on 30.09.2023.
//

#include <ctime>
#include "Resource.h"

int Resource::useItem() {
    return 0;
}

Resource::Resource(int id, string name, int maxAmount, int amount, float weight, int minPrice, int maxPrice) : Item(id, name, maxAmount,
                                                                                               amount, weight), minPrice(minPrice), maxPrice(maxPrice) {}

int Resource::getRandomPrice() {
    srand(time(0));
    int price = (rand() % (maxPrice - minPrice)) + minPrice;
    this->price = price;
    return price;
}

int Resource::getPrice() {
    return this->price;
}

int Resource::getOptAmount() {
    return this->optAmount;
}

int Resource::getRandomOptAmount() {
    srand(time(0));
    int maxOptAmount = 64 - this->getAmount();
    int randomOptAmount = rand() % maxOptAmount;
    this->optAmount = randomOptAmount;
    return randomOptAmount;
}

void Resource::setOptAmount(int amount) {
    this->optAmount = amount;
}
