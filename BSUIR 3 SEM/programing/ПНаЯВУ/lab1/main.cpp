#include <iostream>
#include "Menu.h"
#include <memory>
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    Menu menu;
    unique_ptr<Menu> menuPtr = make_unique<Menu>();
    menuPtr->run();
    return 0;
}
