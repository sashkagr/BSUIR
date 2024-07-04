#include "func.hpp"


void print_menu() {

    printf("  1  - LST(check all file)                       \n");
    printf("  2  - GET Rec_No (get field with number Rec_No) \n");
    printf("  3  - Modification of field                     \n");
    printf("  4  - Save modification of field                \n");
    printf("  q  - Exit                                      \n");
}

int main() {
    createRecords("students.bin");
    int counter = 0;
    char input;
    int fd = open("students.bin", O_RDWR);
    if (fd == -1) {
        perror("failed open");
    }
    std::vector records = showAllRecords(fd, counter);
    record_s record{};
    record_s modifiedRec{};
    int num = 0;
    int modifiedNum = 0;

    while (1) {
        print_menu();
        std::cout << "Enter your choice: \n";
        input= getchar();
        switch(input) {
            case '1':
            {
                records= showAllRecords(fd, counter);
                for(auto i : records) {
                    std::cout << i.name << std::endl;
                    std::cout << i.address << std::endl;
                    std::cout << "Semester: "<< (int) i.semester << std::endl;
                    std::cout << "..............................."<< std::endl;
                }
                break;
            }

            case '2':
            {
                std::cout << "Enter a number of record [from 1 to 15]\n>";
                std::cin >> num;
                while(num > 15 || num < 1) {
                    std::cout << "Wrong num! Enter a number of record [from 1 to 15]\n>";
                    std::cin >> num;
                }
                record = getRecord(fd, num, counter);
                std::cout << "Result:" << std::endl;
                std::cout << record.name << std::endl;
                std::cout << record.address << std::endl;
                std::cout << "Semester: "<<(int) record.semester << std::endl;
                break;
            }

            case '3':
            {
                std::cout << "Enter a number of record [from 1 to 15]\n>";
                std::cin >> num;
                while(num > 15 || num < 1) {
                    std::cout << "Wrong num! Enter a number of record [from 1 to 15]\n>";
                    std::cin >> num;
                }
                modifiedRec = getRecord(fd, num, counter);
                getchar();
                modifyRecord(modifiedRec);
                modifiedNum = num;
                break;
            }

            case '4':
            {
                saveChanges(fd, modifiedNum, modifiedRec, counter);
                std::cout << "Saved last modified record!" << std::endl;
                break;
            }
            case 'q':
            {
                close(fd);
                return 0;
            }

            default:
                std::cout << "Wrong input!\n>";
                break;


        }
        getchar();
    }


}
