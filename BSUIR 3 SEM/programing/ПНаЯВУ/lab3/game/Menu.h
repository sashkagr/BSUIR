#ifndef LAB3_MENU_H
#define LAB3_MENU_H


#include "GameService.h"
#include "InputHelper.h"
#include "Cryption.h"

class Menu {
private:
    GameService gameService;
    InputHelper inputHelper;
    Cryption* cryption;

    void usePotionMenu();
    void craftWeaponMenu();
    void sellWeaponsMenu();
    void buyResourcesMenu();
    void encryptionMenu();
    void makeElectionsMenu();
public:
    void mainMenu();
};


#endif
