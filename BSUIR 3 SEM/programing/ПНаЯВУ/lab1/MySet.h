#ifndef MySet_H
#define MySet_H

#include <iostream>

template <typename T>
class MySet {
public:
    MySet();
    ~MySet();

    void push_back(const T& element);
    T* erase(T* position);
    T& at(int index);
    int size() const;

    T* begin();
    T* end();

    friend std::ostream& operator<<(std::ostream& os, const MySet<T>& set) {
        os << "[";
        for (int i = 0; i < set.len; ++i) {
            os << set.data[i];
            if (i < set.len - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, MySet<T>& set) {
        T element;
        is >> element;
        set.push_back(element);
        return is;
    }

private:
    static const int INITIAL_CAPACITY = 10;
    T* data;
    int capacity;
    int len;

};



#endif  // MySet_H
