#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct HashTableEntry{
    char *key;
    char *value;
    struct HashTableEntry *next;
} ;
typedef struct {
    int size;
    int count;
    struct HashTableEntry **entries;
} HashTable;
HashTable *createHashTable(int size);

unsigned long hash(const char *str);
struct HashTableEntry *getHashTableEntry(HashTable *table, const char *key);
void putHashTableEntry(HashTable *table, char *key, char *value);
void deleteHashTableEntry(HashTable *table, const char *key);
char *generateRandomIpAddress();
void output(HashTable *table);
#endif /* HASH_TABLE_H */
