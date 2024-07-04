#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "Student.h"
#include "BinFile.h"
#include "TxtFile.h"
#include "OutFile.h"
#include <string>

bool uniq(vector<Student*> students,int id )
{
    for (const auto& student : students) {
        if(student->getId()==id)
            return false;
    }
    return true;

}
std::vector<int> inputMenu()
{
    std::vector<Student*> students;
    std::vector<int> IDs;
    char choice;
    cout<< "Enter ID|INFO ABOUT STUDENT"<<endl;
    do {
        bool flag=true;
        Student* student=new Student();
        int id;
        do{

            std::cout << "Enter ID: ";
            std::cin >> id;
            std::cin.ignore();
            cout<< "----------------------------------"<<endl;
            if(id!=(int)id||id<=0||!uniq(students,id)) {
                flag = false;
                cout<< "ERROR: PLEASE, ENTER CORRECT DATA:"<<endl;
            }
        }while(!flag);


        student->setId(id);
        std::string info;
        std::cout << "Enter info: ";
        std::getline(std::cin, info);
        student->setInfo(info);
        students.push_back(student);
        cout<< "----------------------------------"<<endl;
        std::cout << "Do u want to add ? (y-yes): ";
        std::cin >> choice;
    } while (choice == 'y' || choice == 'Y');
    BinFile* bf=new BinFile(students);
    for (const auto& student : students)
        IDs.push_back(student->getId());

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(IDs.begin(),IDs.end(),g);

    TxtFile* tf=new TxtFile(IDs);
    return IDs;
}
void changeMenu(std::vector<int> IDs)
{
    int id;
    char choice;
    bool flag=true;
    cout<< "Do u want to change info?"<<endl;
    cin >> choice;
    if((choice == 'y' || choice == 'Y')) {
        do {
            do{

                std::cout << "Enter ID: ";
                std::cin >> id;
                std::cin.ignore();
                cout<< "----------------------------------"<<endl;
                // Используем std::find для проверки наличия значения в векторе
                auto it = std::find(IDs.begin(), IDs.end(), id);

                if (it == IDs.end()&&id!=IDs.at(IDs.size()-1)) {
                    std::cout << "ERROR: PLEASE, ENTER CORRECT DATA:" << std::endl;
                    flag=false;
                }
            }while(!flag);
            OutFile of;
            of.changeById(id);
            cout<< "Do u want to change info?"<<endl;
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }
}
void suffle() {
    std::ifstream textFile("txt_file.txt");
    std::ifstream binaryFile("binary_file.bin");
    std::ofstream outputFile("output_file.txt");

    int id1, id2 = -1;
    Student* student = new Student();

    while (textFile >> id1) {
        binaryFile.seekg(0, std::ios::beg);
        while (id1 != id2 ) {

            binaryFile.read(reinterpret_cast<char*>(&id2), sizeof(int));
            int infoLength;
            binaryFile.read(reinterpret_cast<char*>(&infoLength), sizeof(int));
            char* buffer = new char[infoLength + 1];
            binaryFile.read(buffer, infoLength);
            buffer[infoLength] = '\0';

            student->setId(id2);
            student->setInfo(buffer);
        }
        outputFile << *student;
    }

    delete student;

    textFile.close();
    binaryFile.close();
    outputFile.close();
}

int main() {
    std::vector<int> IDs=inputMenu();
suffle();
    changeMenu(IDs);





    return 0;
}
