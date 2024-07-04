
#ifndef LAB3_ITEM_H
#define LAB3_ITEM_H


#include <string>

using namespace std;
class Item {
private:
    int id;
    string name;
    int maxAmount;
    int amount;
    float weight;

public:
    Item(int id, string name, int maxAmount, int amount, float weight);

    int getId();

    void setId(int id);

    string getName();

    void setName( string name);

    int getMaxAmount() ;

    void setMaxAmount(int maxAmount);

    int getAmount();

    void setAmount(int amount);

    float getWeight();

    void setWeight(float weight);

    virtual int useItem() = 0;
};


#endif //LAB3_ITEM_H
