
#ifndef LAB7_STUDENT_H
#define LAB7_STUDENT_H

#include <cstddef>
#include <string>
#include <memory>

using namespace std;
class Student {
private:
    int id;
    std::string info;

public:
    Student();
    int getId() const;
    void setId(int id);
    const std::string &getInfo() const;
    void setInfo(const std::string &info);

    friend std::ostream &operator<<(ostream &os, const Student &student);

    // Перегрузка оператора ввода для чтения из файла
    friend std::istream& operator>>(std::istream& is, Student& student);


    // Конструктор для создания студента

};


#endif //LAB7_STUDENT_H
