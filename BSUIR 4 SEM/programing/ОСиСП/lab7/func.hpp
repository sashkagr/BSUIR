#ifndef LAB7_CONCURRENT_FILE_FUNC_H
#define LAB7_CONCURRENT_FILE_FUNC_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <cstdint>

struct record_s {
    char name[80];
    char address[80];
    uint8_t semester;
};

void saveChanges(const int& fd, const int& num_rec, const record_s& record, int & counter);
void modifyRecord (record_s & record);
record_s getRecord(const int& fd, const size_t& num_rec, int & counter);
std::vector<record_s> showAllRecords(const int& fd, int& counter);
void createRecords(const char* filename);

#endif //LAB7_CONCURRENT_FILE_FUNC_H
