
#include <iostream>
#include "BinFile.h"

BinFile::BinFile(vector<Student *> students) {
    std::ofstream outputFile("binary_file.bin", std::ios::binary);
    if (outputFile.is_open()) {
        for (const auto& student : students) {
            // Запись данных о студентах в бинарный файл
            int id = student->getId();
            outputFile.write(reinterpret_cast<const char*>(&id), sizeof(int));

            std::string info = student->getInfo();
            int infoLength = info.size();
            outputFile.write(reinterpret_cast<const char*>(&infoLength), sizeof(int));
            outputFile.write(info.c_str(), infoLength);
        }

        outputFile.close();
    } else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
    }
}


void BinFile::printBinaryFileContents(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << " для чтения." << std::endl;
        return;
    }

    std::cout << "Содержимое файла " << filename << ":" << std::endl;

    while (!inputFile.eof()) {
        int id;
        inputFile.read(reinterpret_cast<char*>(&id), sizeof(int));

        if (inputFile.eof()) {
            break;
        }

        int infoLength;
        inputFile.read(reinterpret_cast<char*>(&infoLength), sizeof(int));

        char* buffer = new char[infoLength + 1];
        inputFile.read(buffer, infoLength);
        buffer[infoLength] = '\0';

        std::cout << "{id: " << id << " | info: " << buffer << "}" << std::endl;

        delete[] buffer;
    }

    inputFile.close();

}
