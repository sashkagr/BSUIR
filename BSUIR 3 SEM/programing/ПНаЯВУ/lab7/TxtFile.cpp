//
// Created by sashk on 19.11.2023.
//

#include <iostream>
#include "TxtFile.h"

TxtFile::TxtFile(vector<int> IDs) {

        std::ofstream outputFile("txt_file.txt");
        if (outputFile.is_open()) {
            for (const auto& id : IDs) {
                // Запись данных о студентах в текстовый файл
                outputFile << id << "\n";
            }
            outputFile.close();
            std::cout << "Текстовый файл успешно создан." << std::endl;
        } else {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
        }
    }


void TxtFile::printBinaryFileContents(const string &filename) {

}
