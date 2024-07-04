
#ifndef LAB7_BINFILE_H
#define LAB7_BINFILE_H
#include <fstream>
#include <vector>
#include "Student.h"

using namespace std;
class BinFile {

public:
    BinFile(vector<Student*> students);
    void printBinaryFileContents(const string &filename);
};


#endif //LAB7_BINFILE_H
