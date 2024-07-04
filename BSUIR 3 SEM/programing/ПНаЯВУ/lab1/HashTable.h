#include <memory>
#ifndef LAB1_HASHTABLE_H
#define LAB1_HASHTABLE_H
#include <map>
#include <cstddef>
#include <string>
#include <vector>
using namespace std;
class HashTable {
private:
    unique_ptr<map<size_t, string>> map = make_unique<::map<size_t, string>>();
    unique_ptr<vector<string>> keys = make_unique<vector<string>>();

public:
    bool add(string key,string value);
    string search(string key);
    bool ubdateKey(string oldKey,string newKey);
    void ubdateValue(string Key,string newValue);
    void printUnhashedKeyAndValue();
};


#endif //LAB1_HASHTABLE_H
