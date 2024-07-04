#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include <windows.h>
#include <limits.h>

HashTable *createHashTable(int size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->entries = (struct HashTableEntry **)calloc(size, sizeof(struct HashTableEntry *));
    return table;
}

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

struct HashTableEntry *getHashTableEntry(HashTable *table, const char *key) {
    unsigned long hashValue = hash(key) % table->size;
    struct HashTableEntry *entry = table->entries[hashValue];
    printf("%lu\n",hashValue);
    while (entry) {
        if (strcmp(entry->key, key) == 0)
            return entry;
        entry = entry->next;
    }

    return NULL;
}

/**
 * Метод цепочек заключается в том, что в каждой ячейке хеш-таблицы хранится не один, а несколько элементов,
 * которые являются членами связанного списка. При добавлении элемента, если он попадает в ячейку, которая уже
 * занята, новый элемент просто добавляется в конец списка. При поиске элемента необходимо просмотреть все
 * элементы списка в соответствующей ячейке хеш-таблицы.
 *
 * Для реализации метода цепочек в функции insert() необходимо добавить следующую логику:
 * 1. Вычислить хеш ключа.
 * 2. Проверить, есть ли элемент с таким же хешем в ячейке таблицы.
 * 3. Если элемента нет, создать новый элемент типа HashTableElement, инициализировать его ключом и значением и добавить его в ячейку таблицы.
 * 4.
 * Если элемент с таким хешем уже есть в ячейке таблицы, то добавить новый элемент в конец связанного списка.
 */

void putHashTableEntry(HashTable *table, char *key, char *value)
{
    int hash1 = hash(key) % table->size;
    struct HashTableEntry* new_element = (struct HashTableEntry*)malloc(sizeof(struct HashTableEntry));
    new_element->key = key;
    new_element->value = value;
    new_element->next = NULL;
    if (table->entries[hash1] == NULL) {
        table->entries[hash1] = new_element;
    } else {
        struct HashTableEntry* current_element = table->entries[hash1];
        while (current_element->next != NULL) {
            current_element = current_element->next;
        }
        current_element->next = new_element;
    }
}

void deleteHashTableEntry(HashTable *table, const char *key) {
    unsigned long hashValue = hash(key) % table->size;
    struct HashTableEntry *entry = table->entries[hashValue];
    int num,choose;
    printf("Choose: 1)delete 1 element    2)delete all elements with same name");
    scanf("%d",&choose);
    struct HashTableEntry *prev =NULL;
    struct HashTableEntry *head = table->entries[hashValue];
    if(choose==2) {
        while(entry!=NULL ){
            if (strcmp(entry->key, key) == 0) //1 0 0 1
            {
                if(head==entry) {
                    head = head->next;
                    prev=NULL;
                }
                else prev->next=entry->next;
            }
            else prev=entry;
            entry=entry->next;
        }
        table->entries[hashValue] = head;
    }
    else {
        while (strcmp(entry->key, key) != 0)
        {
            prev = entry;
            entry = entry->next;
        }
        if (prev != NULL)
            prev->next = entry->next;
        else
            table->entries[hashValue] = entry->next;
        table->count--;
    }
}

char *generateRandomIpAddress() {
    FILETIME ft;
    ULARGE_INTEGER li;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    unsigned long long seed = li.QuadPart;
    srand((unsigned int)(seed % UINT_MAX));
    char *ip = (char*)malloc(50 * sizeof(char));
    sprintf(ip, "%d.%d.%d.%d:%d", rand() % 256, rand() % 256, rand() % 256, rand() % 256, rand() % 65536);
    return ip;
}

void output(HashTable *table) {
    struct HashTableEntry *entry = NULL;
    int i = 0;
    while (i != 1000) {
        entry = table->entries[i];
        if (entry != NULL) {
//            printf("%s       \t\t\t\t|%s\t\t\t\t|%lu\n", entry->key,
//                   entry->value, hash(entry->key)% table->size);
            printf("%-20s | %-20s | %lu\n", entry->key,
                   entry->value, hash(entry->key) % table->size);

            while (entry->next != NULL) {
                entry = entry->next;
//                printf("%s       \t\t\t\t|%s\t\t\t\t|%lu\n", entry->key,
//                       entry->value, hash(entry->key)% table->size);
                printf("%-20s | %-20s | %lu\n", entry->key,
                       entry->value, hash(entry->key) % table->size);

            }
        }
        i++;
    }
}