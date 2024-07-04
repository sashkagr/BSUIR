#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include <iostream>

void readFile(char *file_name, byte **buffer, int buffer_size) {
    std::string str="";
    FILE *file = fopen(file_name, "rb"); // Открываем файл в режиме чтения бинарных данных

    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    *buffer = (byte*)malloc(sizeof(byte) * buffer_size);

    if (*buffer == NULL) {
        fclose(file);
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < buffer_size; i++) {
        int ch = fgetc(file);
        if (ch == EOF) {
            fprintf(stderr, "Error reading the file");
            fclose(file);
            free(*buffer);
            exit(EXIT_FAILURE);
        }
        (*buffer)[i] = (byte)ch;
        str+=(byte)ch;
        std::cout<<(*buffer)[i];
    }
    writeFile("new.png", str,buffer_size);
    if (fclose(file) != 0) {
        perror("Error closing the file");
        exit(EXIT_FAILURE);
    }

}

void writeFile(char *file_name, std::string str, int buffer_size) {
    FILE *file = fopen(file_name, "wb"); // Открываем файл в режиме записи бинарных данных

    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < buffer_size; i++) {
        if (fputc(str[i], file) == EOF) {
            perror("Error writing to the file");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    if (fclose(file) != 0) {
        perror("Error closing the file");
        exit(EXIT_FAILURE);
    }
}
