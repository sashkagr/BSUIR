#include "HashTable.h"
#include <functional>
#include <iostream>


using namespace std;

bool HashTable::add(string key, string value) {
    hash<string> stringHash;
    size_t hashedKey = stringHash(key);
    if(search(key).empty()) {
        map->insert(make_pair(hashedKey, value));
        if (find(keys->begin(), keys->end(), key) == keys->end()) keys->push_back(key);
        return true;
    } else
        return false;
}

string HashTable::search(string key) {
    hash<string> stringHash;
    size_t hashedKey = stringHash(key);
    auto result=map->find(hashedKey);
    if(result==map->end())
      return "";
      else
    return result->second;

}

bool HashTable::ubdateKey(string oldKey, string newKey) {
    hash<string> stringHash;
    size_t hashedKey = stringHash(newKey);
    auto result=map->find(hashedKey);
    if(result!=map->end())
        return false;

    auto keyIt = find(keys->begin(), keys->end(), oldKey);
    if (keyIt != keys->end()) keys->erase(keyIt);

    hashedKey = stringHash(oldKey);
    result=map->find(hashedKey);
    string password=result->second;
    map->erase(result);
    add(newKey,password);
    return true;
}

void HashTable::ubdateValue(string key, string newValue) {
    hash<string> stringHash;
    size_t hashedKey = stringHash(key);
    auto result=map->find(hashedKey);
    map->erase(result);
    add(key,newValue);
}

void HashTable::printUnhashedKeyAndValue() {
    for (int i = 0; i < keys->size(); i++)
    {
        string login = keys->at(i);
        string password = search(login);
        cout << login << " " << password << endl;
    }
}
