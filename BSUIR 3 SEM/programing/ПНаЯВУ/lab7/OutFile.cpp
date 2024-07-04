//
// Created by sashk on 20.11.2023.
//

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "OutFile.h"

void OutFile::changeById(int id) {
    std::ifstream inFile("output_file.txt");


    if (!inFile.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    // Чтение строк из файла и сохранение в векторе
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }

    inFile.close();


    std::ofstream outFile("output_file.txt");

    if (!outFile.is_open()) {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
        return;
    }

    // Запись вектора строк обратно в файл
    for ( std::string line : lines) {
        std::string num;
        num=line[5];
        int i=6;
        while(line[i]!=' ')
            num+=line[i];
        if(id==std::atoi(num.c_str()))
        {
            num="";
            std::cout<<"Enter new data: "<<std::endl;
            std::cin>>num;
            line="{id: " + std::to_string(id) + " | info: " + num + "}";
        }
        outFile << line<<std::endl;
    }

    outFile.close();


}
