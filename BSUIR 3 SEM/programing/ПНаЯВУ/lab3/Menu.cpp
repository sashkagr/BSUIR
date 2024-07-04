#include <iostream>
#include <limits>

#include "Menu.h"
#include "OutputHelper.h"
#include "Encryption.h"
#include "Decryption.h"
#include "Elections.h"


void Menu::mainMenu() {
    system("cls");
    gameService.showInventory();
    cout << "Menu: " << endl;
    cout << OutputHelper::underline("1. Buy resources") << endl;
    cout << OutputHelper::underline("2. Sell weapons") << endl;
    cout << OutputHelper::underline("3. Use potion") << endl;
    cout << OutputHelper::underline("4. Craft weapon") << endl;
    cout << OutputHelper::underline("5. Encryption/decryption") << endl;
    cout << OutputHelper::underline("6. Elections") << endl;
    cout << endl;
    cout << "Option: ";
    int option = inputHelper.inputInt(1, 6);
    switch (option) {
        case 1: buyResourcesMenu(); break;
        case 2: sellWeaponsMenu(); break;
        case 3: usePotionMenu(); break;
        case 4: craftWeaponMenu(); break;
        case 5: encryptionMenu(); break;
        case 6: makeElectionsMenu(); break;
    }
}

void Menu::usePotionMenu() {
    system("cls");
    gameService.showPoisons();
    cout << "13 Back" << endl;
    cout << "Enter potion ID: ";
    int potionId = inputHelper.inputInt(9, 13);
    if (potionId == 13) mainMenu();
    gameService.useItemById(potionId);
    mainMenu();
}

void Menu::craftWeaponMenu() {
    system("cls");
    cout << "  INFORMATION " << endl;
    cout << "| sword = x3 stone x1 wood \t|" << endl;
    cout << "| axe = x1 wood x1 stone \t|" << endl;
    cout << "| crossbow = x2 wood x1 wool \t|" << endl;
    cout << "| trident = x3 sand \t|" << endl;
    cout << endl;
    gameService.showResources();
    cout << endl;
    gameService.showWeapons();
    cout << "5 Back" << endl;
    cout << "Enter weapon id: ";
    int weaponId = InputHelper::inputInt(1,5);
    if (weaponId == 5) mainMenu();
    gameService.craftWeapon(weaponId - 1);
    system("pause");
    mainMenu();
}

void Menu::sellWeaponsMenu() {
    system("cls");
    gameService.showWeapons();
    cout << "5. Back to main menu" << endl;
    cout << "Enter weapon id: ";
    int weaponId = InputHelper::inputInt(1,5);
    if (weaponId == 5) mainMenu();
    gameService.sellWeapon(weaponId - 1);
    mainMenu();
}

void Menu::buyResourcesMenu() {
    system("cls");
    gameService.buyResources();
    mainMenu();
}

void Menu::encryptionMenu() {
    cryption = new Encryption();
    system("cls");
    cout << "n: ";
    int n = InputHelper::inputInt(2, 1000);
    string str;
    cout << "string: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, str);
    cout << "result: ";
    string encrypted = cryption->crypt(n, str);
    cout << encrypted << endl;
    cout << "decrypted result: ";
    cryption = new Decryption();
    cout << cryption->crypt(n, encrypted) << endl;
    system("pause");
    mainMenu();
}

void Menu::makeElectionsMenu() {
    system("cls");
    cout << "Enter M: ";
    int M = InputHelper::inputInt(6, 20);
    cout << "Enter N: ";
    int N = InputHelper::inputInt(6, 20);
    cout << "Enter number of candidates: ";
    int numberOfCandidates = InputHelper::inputInt(2, M*N);
    (new Elections())->makeElections(M, N, numberOfCandidates);
    system("pause");
    mainMenu();
}
