#include "MySet.h"

template <typename T>
MySet<T>::MySet() {
    static_assert(std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, std::string>::value, "Unsupported data type");
    capacity = INITIAL_CAPACITY;
    data = new T[capacity];
    len = 0;
}

template <typename T>
MySet<T>::~MySet() {
    delete[] data;
}

template <typename T>
void MySet<T>::push_back(const T& element) {
    if (len == capacity) {
        capacity *= 2;
        T* new_data = new T[capacity];
        for (int i = 0; i < len; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

    // Проверка на дубликаты
    for (int i = 0; i < len; ++i) {
        if (data[i] == element) {
            return; // Элемент уже существует, не добавляем его снова
        }
    }

    data[len++] = element;
}

template <typename T>
T* MySet<T>::erase(T* position) {
    if (position < data || position >= data + len) {
        return position; // Если позиция некорректна, вернем ее без изменений
    }

    for (T* it = position; it < data + len - 1; ++it) {
        *it = *(it + 1);
    }

    --len;
    return position;
}

template <typename T>
T& MySet<T>::at(int index) {
    if (index < 0 || index >= len) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template <typename T>
int MySet<T>::size() const {
    return len;
}

template <typename T>
T* MySet<T>::begin() {
    return data;
}

template <typename T>
T* MySet<T>::end() {
    return data + len;
}

template class MySet<std::string>;
template class MySet<int>;
template class MySet<float>;
