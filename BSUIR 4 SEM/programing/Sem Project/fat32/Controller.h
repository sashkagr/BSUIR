#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include <regex>

class Controller {

public:
    static std::vector<std::string> find_usbflash() {
        std::vector<std::string> tmp_paths;
        TCHAR buf[100];
        DWORD len = GetLogicalDriveStrings(sizeof(buf) / sizeof(TCHAR), buf);
        for (TCHAR* s = buf; *s; s += lstrlen(s) + 1) {
            TCHAR sDrivePath[MAX_PATH];
            lstrcpy(sDrivePath, s);
            UINT uDriveType = GetDriveType(sDrivePath);
            if (uDriveType == DRIVE_REMOVABLE) {
                std::string strPath(sDrivePath, sDrivePath + lstrlen(sDrivePath));
                tmp_paths.push_back(strPath);
            }
        }
        return tmp_paths;
    }

    static int Enter_IntNumber(int firstcheck, int secondcheck, const char* str) {
        int number;
        char buffer;
        do {
            printf("%s", str);
            rewind(stdin);
        } while (!(scanf_s("%d%c", &number, &buffer, 1)) || number < firstcheck || number > secondcheck);
        return number;
    }

    static std::string get_NewName() {
        bool check = true;
        std::string newName;
        do {
            check = true;
            printf("Enter new device name ( max 11 symbols)\nName: ");
            std::cin >> newName;
            if (newName.size() > 11) {
                check = false;
                std::cout << "You've enterded more than 11 symbols.\n\n";
                continue;
            }
            if (!std::regex_match(newName, std::regex("[A-Za-z0-9_-]*"))) {
                check = false;
                std::cout << "Banned symbols were found.\n\n";
            }
        } while (!check);
        return newName;
    }

    static bool verify_choice(int choice, std::vector<std::string> USBs) {
        if (choice < 0 || choice >= USBs.size()) {
            std::cout << "Wrong choice." << std::endl;
            return false;
        }
        std::vector<std::string> new_USBs = find_usbflash();
        for (int j = 0; j < new_USBs.size(); j++) {
            if (new_USBs.at(j) == USBs.at(choice)) {
                return true;
            }
        }
        std::cout << "Can't format that USB, return it to PC.\nOr any symbol to continue." << std::endl;
        system("pause");
        return false;
    }

    char get_USBPath() {
        std::vector<std::string> USBs;
        int choice = 0;
        std::cout << "Make sure that you inserted your USB-drive." << std::endl;
        system("pause");
        do {
            USBs = find_usbflash();
            while (USBs.empty()) {
                std::cout << "Can't find any USB." << std::endl;
                std::cout << "Insert your USB to PC and press any symbol." << std::endl;
                system("pause");
                USBs = find_usbflash();
            }
            std::cout << "Choose USB to format (write id of USB):" << std::endl;
            for (int i = 0; i < USBs.size(); i++) {
                std::cout << "USB [" << i << "] path: " + USBs.at(i) << std::endl;
            }
            choice = Enter_IntNumber(0, USBs.size() - 1, "Choice: ");
        } while (!verify_choice(choice, USBs));
        return get_PathLetter(USBs.at(choice));
    }

    static char get_PathLetter(std::string path_usb) {
        for (int i = 0; i < path_usb.size(); i++) {
            if (path_usb.at(i) > 'A' && path_usb.at(i) < 'Z') {
                return path_usb.at(i);
            }
        }
        return '\0';
    }
};
