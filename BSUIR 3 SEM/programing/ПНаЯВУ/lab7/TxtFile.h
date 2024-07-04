//
// Created by sashk on 19.11.2023.
//

#ifndef LAB7_TXTFILE_H
#define LAB7_TXTFILE_H
#include <fstream>

#include <vector>
#include "Student.h"

using namespace std;
class TxtFile {
public:
    TxtFile(vector<int> IDs);
    void printBinaryFileContents(const string &filename);
};


#endif //LAB7_TXTFILE_H
