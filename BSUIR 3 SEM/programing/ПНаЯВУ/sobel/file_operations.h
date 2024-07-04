
#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include "macros.h"

void readFile(char *file_name, byte **buffer, int buffer_size);
void writeFile(char *file_name,std::string str, int buffer_size);

#endif