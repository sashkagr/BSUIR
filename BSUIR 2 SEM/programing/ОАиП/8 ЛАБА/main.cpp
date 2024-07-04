#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"
#include <chrono>
#include <iostream>
#include <unordered_map>

int main() {
    HashTable *table = createHashTable(1000);
    std::unordered_map<std::string, std::string> myMap;
    FILE *file = fopen("file.txt", "r");

    while (1) {
        printf("\nEnter the command:\n 1 - load\n 2 - add\n 3 - delete\n 4 - find\n 5 - printf\n 6 - out\nCommand:  ");
        int command;
        fflush(stdin);
        scanf("%d", &command);

        if (command == 1) {
            int n;
            printf("Amount of rows ");
            scanf("%d", &n);

            if (file == NULL) {
                printf("Error with file \n");
                continue;
            }

            char key[50];
            int count = 0;
            while (count < n && fscanf(file, "%s", key) != EOF) {
                if (key[strlen(key) - 1] == '\n') {
                    key[strlen(key) - 1] = '\0';
                }
                char *value = generateRandomIpAddress();

                char* copiedKey = (char*) malloc(sizeof(char) * (strlen(key) + 1));
                strcpy(copiedKey, key);
                putHashTableEntry(table, copiedKey, value);
                myMap.insert({copiedKey, value});
                count++;
            }

            printf("Download was successful.\n");
        }
        else if (command == 2) {
            auto start_time1 = std::chrono::high_resolution_clock::now();

            char key[50];
            printf("Enter the row: ");
            scanf("%s", key);

            char *value = generateRandomIpAddress();
            char* copiedKey = (char*) malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(copiedKey, key);
            putHashTableEntry(table, copiedKey, value);
            printf("The element was added.\n");

            auto end_time1 = std::chrono::high_resolution_clock::now();
            auto duration_ns1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
            std::cout << "Duration of my implementation: " << duration_ns1 << " ns" << std::endl;

            auto start_time2 = std::chrono::high_resolution_clock::now();

            myMap.insert({copiedKey, value});

            auto end_time2 = std::chrono::high_resolution_clock::now();
            auto duration_ns2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();
            std::cout << "Duration of embedded implementation: " << duration_ns2 << " ns" << std::endl;
        }
        else if (command == 3) {
            auto start_time1 = std::chrono::high_resolution_clock::now();

            char key[50];
            printf("Enter the row: ");
            scanf("%s", key);

            deleteHashTableEntry(table, key);

            printf("The element was deleted.\n");

            auto end_time1 = std::chrono::high_resolution_clock::now();
            auto duration_ns1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
            std::cout << "Duration of my implementation: " << duration_ns1 << " ns" << std::endl;

            auto start_time2 = std::chrono::high_resolution_clock::now();
            char* copiedKey = (char*) malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(copiedKey, key);
            myMap.erase(copiedKey);

            auto end_time2 = std::chrono::high_resolution_clock::now();
            auto duration_ns2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();
            std::cout << "Duration of embedded implementation: " << duration_ns2 << " ns" << std::endl;
        }
        else if (command == 4) {
            auto start_time1 = std::chrono::high_resolution_clock::now();

            char key[50];
            printf("Enter the row: ");
            scanf("%s", key);

            struct HashTableEntry *entry = getHashTableEntry(table, key);

            if (entry != NULL) {
                if (strcmp(entry->key, key) == 0)
                    printf("Ip address: %s\n", entry->value);
                while (entry->next != NULL) {
                    entry = entry->next;
                    if (strcmp(entry->key, key) == 0)
                        printf("Ip address: %s\n", entry->value);
                }
            } else {
                printf("The element was not found.\n");
            }

            auto end_time1 = std::chrono::high_resolution_clock::now();
            auto duration_ns1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
            std::cout << "Duration of my implementation: " << duration_ns1 << " ns" << std::endl;

            auto start_time2 = std::chrono::high_resolution_clock::now();

            char* copiedKey = (char*) malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(copiedKey, key);
            if (myMap.find(copiedKey) != myMap.end()) myMap.at(copiedKey);

            auto end_time2 = std::chrono::high_resolution_clock::now();
            auto duration_ns2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();
            std::cout << "Duration of embedded implementation: " << duration_ns2 << " ns" << std::endl;
        }
        else if (command == 5)output(table);
        else if (command == 6) {
            break;
        }
        else {
            printf("Error: you enter incorrect data!.\n");
        }
    }
    fclose(file);

    return 0;
}
