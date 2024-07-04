#include "func.hpp"

void createRecords(const char* filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return;
    }

    record_s records[15];
    for (int i = 0; i < 15; i++) {
        sprintf(records[i].name, "Student: %d", i + 1);
        sprintf(records[i].address, "Address: %d", i + 1);
        records[i].semester = i + 1;
    }

    struct flock lock{};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;


    struct flock getLock{};

    if (fcntl(fd, F_GETLK, &getLock) == -1) {
        perror("fcntl");
    }

    while (getLock.l_type != F_UNLCK) {
        sleep(1);

        if (fcntl(fd, F_GETLK, &getLock) == -1) {
            perror("fcntl");
        }
    }
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
    }

    if (write(fd, records, sizeof(records)) == -1) {
        perror("failed write");
        return;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
    }

    close(fd);
}


std::vector<record_s> showAllRecords(const int& fd, int& counter) {
    std::vector<record_s> records;

    lseek(fd, 0, SEEK_SET);

    struct flock lock{};
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    struct flock getLock{};

    if (fcntl(fd, F_GETLK, &getLock) == -1) {
        perror("fcntl");
        return records;
    }

    while (getLock.l_type != F_UNLCK) {
        sleep(1);

        if (fcntl(fd, F_GETLK, &getLock) == -1) {
            perror("fcntl");
            return records;
        }
    }

    lock.l_type = F_RDLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        return records;
    }

    while (true) {
        record_s recordS{};
        ssize_t bytesRead = read(fd, &recordS, sizeof(recordS));
        if (bytesRead == -1) {
            perror("failed read");
            break;
        } else if (bytesRead == 0) {
            break;
        }

        records.push_back(recordS);
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
    }

   counter = (int) records.size();

    return records;
}

record_s getRecord(const int& fd, const size_t& recNo, int & counter) {
    record_s record{};
    if(recNo > counter) {
        perror("too high number of num_rec");
        return record;
    }
    struct flock lock{};
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (recNo - 1) * sizeof(record_s);;
    lock.l_len = sizeof(record_s);

    struct flock getLock{};

    if (fcntl(fd, F_GETLK, &getLock) == -1) {
        perror("fcntl");
        return record;
    }

    while (getLock.l_type != F_UNLCK) {
        sleep(1);

        if (fcntl(fd, F_GETLK, &getLock) == -1) {
            perror("fcntl");
            return record;
        }
    }

    lock.l_type = F_RDLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        return record;
    }

    lseek(fd, (recNo - 1) * sizeof(record_s), SEEK_SET);
    read(fd, &record, sizeof(record_s));
    lseek(fd, 0, SEEK_SET);

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
    }

    return record;

}

void modifyRecord (record_s & record) {

    while(1) {
        printf("1.Edit name.\n");
        printf("2.Edit address.\n");
        printf("3.Edit num of semester.\n");
        printf("4.Display edit record.\n");
        printf("q - Exit.\n");

        char choice = getchar();
        char BUFFER[80];
        switch(choice) {
            case '1': {
                std::cout << "Enter the new name" << std::endl;
                scanf("%s", BUFFER);
                strncpy(record.name, BUFFER, 80);
                rewind(stdin);
                break;
            }
            case '2': {
                printf("Enter the new address: ");
                scanf("%s", BUFFER);
                strncpy(record.address, BUFFER, 80);
                break;
            }
            case '3': {
                u_int8_t sem;
                printf("Enter the new num of semester: ");
                scanf("%hhu", &sem);
                record.semester = sem;
                break;
            }
            case '4' : {
                std::cout << record.address << std::endl;
                std::cout << record.name << std::endl;
                std::cout << "Semester: ";
                std::cout << (int) record.semester << std::endl;
                break;
            }
            case 'q' : {
                return;
            }
            default: {
                break;
            }
        }
        getchar();
    }

}

void saveChanges(const int& fd, const int& recNo, const record_s& record, int & counter) {
    if(recNo > counter) {
        perror("too high number of num_rec");
        return;
    }

    struct flock lock{};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (recNo - 1) * sizeof(record_s);
    lock.l_len = sizeof(record_s);

    struct flock getLock{};

    if (fcntl(fd, F_GETLK, &getLock) == -1) {
        perror("fcntl");
        return;
    }

    while (getLock.l_type != F_UNLCK) {
        sleep(1);
        if (fcntl(fd, F_GETLK, &getLock) == -1) {
            perror("fcntl");
            return;
        }
    }

    lock.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        return;
    }

    lseek(fd, (recNo - 1) * sizeof(record_s), SEEK_SET);
    write(fd, &record, sizeof(record_s));
    lseek(fd, 0, SEEK_SET);

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
    }

}