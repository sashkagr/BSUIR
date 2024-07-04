#include "Student.h"
using namespace std;
#include <ostream>
#include <iostream>
int Student::getId() const {
    return this->id;
}

void Student::setId(int id) {
    this->id=id;
}

const std::string &Student::getInfo() const {
    return this->info;
}

void Student::setInfo(const string &info) {
    this->info=info;
}

Student::Student() {

}
 ostream &operator<<(ostream &os, const Student &student) {
    int id=student.getId();
    string ids= to_string(id);
    string info=student.getInfo();
    string out="{id: " + ids + " | info: " +info + "}\n";
    os << out;
    return os;
}

istream& operator>>(std::istream& is, Student& student) {
    char c;
    is >> c; // Считываем '{'
    is >> c; // Считываем 'id'
    is >> c; // Считываем ':'
    is >> student.id; // Считываем id
    is >> c; // Считываем '|'
    is >> c; // Считываем 'info'
    is >> c; // Считываем ':'
    is.ignore(); // Пропускаем пробел после ':'
    std::getline(is, student.info, '}'); // Считываем информацию о студенте
    return is;
}