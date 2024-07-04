#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}
std::tm stringToDate(const std::string& str) {
    std::tm date = {};
    std::istringstream ss(str);
    ss >> std::get_time(&date, "%d.%m");
    return date;
}
bool isBefore(std::tm date1, std::tm date2) {
    if (date1.tm_mon == 0 && date1.tm_mday == 0) {
        return false;
    }
    if (date1.tm_mon == date2.tm_mon)
    {
        return date1.tm_mday <= date2.tm_mday;
    }
    else return date1.tm_mon < date2.tm_mon;
}

void processGroup(std::string filename)
{
    std::ifstream file(filename);
    std::vector<std::vector<std::tm>> labsData;
    std::vector<std::string> studentData;
    std::vector<std::tm> deadlineData;
    if (file.is_open()) {
        std::string line;
        int lineNo = 0;
        while (std::getline(file, line)) {
            if (lineNo == 2) {
                std::vector<std::string> fields = split(line, ',');
                for (int i = 0; i < fields.size() - 1; i++) {
                    if (!fields[i].empty()) {
                        std::string deadline = split(fields[i], ' ')[1];
                        deadlineData.push_back(stringToDate(deadline));
                    }
                }
                std::cout << std::endl;
            }

            if (lineNo >= 4 && lineNo <= 34) {
                std::vector<std::string> fields = split(line, ',');

                studentData.push_back(fields[1]);

                std::vector<std::tm> labsDone;
                labsDone.push_back(stringToDate(fields[6]));
                labsDone.push_back(stringToDate(fields[9]));
                labsDone.push_back(stringToDate(fields[12]));
                labsDone.push_back(stringToDate(fields[15]));
                labsDone.push_back(stringToDate(fields[18]));
                labsDone.push_back(stringToDate(fields[21]));
                labsDone.push_back(stringToDate(fields[24]));

                labsData.push_back(labsDone);
            }
            lineNo++;
        }
        file.close();
    } else {
        std::cout << "Error opening file" << std::endl;
    }
    for (int i = 0; i < 30; i++)
    {
        if (isBefore(labsData[i][0], deadlineData[0]) &&
            isBefore(labsData[i][1], deadlineData[1]) &&
            isBefore(labsData[i][2], deadlineData[2]) &&
            isBefore(labsData[i][3], deadlineData[3]) &&
            isBefore(labsData[i][4], deadlineData[4]) &&
            isBefore(labsData[i][5], deadlineData[5]) &&
            isBefore(labsData[i][6], deadlineData[6]))
        {
            std::cout << studentData[i] << std::endl;
        }
    }
}

int main() {
    system("chcp 65001");
    std::cout << std::endl << "group 250502" << std::endl;
    processGroup("input.csv");
    std::cout << std::endl << "group 250503" << std::endl;
    processGroup("input1.csv");
    system("pause");
    return 0;
}
