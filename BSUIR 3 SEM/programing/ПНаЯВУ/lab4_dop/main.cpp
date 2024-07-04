#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <unordered_map>

using namespace std;

int minDistrict, maxDistrict;

int getRand(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

void fillVotersMatrix(vector<vector<int>> *votersMatrix, int numberOfCandidates)
{
    for (int i = 0; i < votersMatrix->size(); i++)
    {
        for (int j = 0; j < (*votersMatrix)[i].size(); j++)
        {
            (*votersMatrix)[i][j] = getRand(1, numberOfCandidates);
        }
    }
}

void textcolor(int color) {
    static int __BACKGROUND;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            color + (__BACKGROUND << 4));
}

void printMatrix(const std::vector<std::vector<int>>& matrix, const std::vector<std::vector<int>>& matrix2) {
    std::cout << "Matrix:\n";
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            int value = matrix[i][j];
            int value2 = matrix2[i][j];
            int temp = value;
            if (temp > 10) {
                while (temp > 10)
                    temp -= 10;
            }

            switch (temp) {
                case 0:
                    textcolor(3);
                    break;
                case 1:
                    textcolor(4);
                    break;
                case 2:
                    textcolor(14);
                    break;
                case 3:
                    textcolor(5);
                    break;
                case 4:
                    textcolor(6);
                    break;
                case 5:
                    textcolor(13);
                    break;
                case 6:
                    textcolor(8);
                    break;
                case 7:
                    textcolor(7);
                    break;
                case 8:
                    textcolor(10);
                    break;
                case 9:
                    textcolor(15);
                    break;
                case 10:
                    textcolor(12);
                    break;
            }
            std::cout << std::setw(2) << std::setfill('0') << value << "(" << std::setw(2) << std::setfill('0') << value2 << ") ";
        }
        std::cout << std::endl;
    }
    textcolor(15);
}

void floodFill(std::vector<std::vector<int>>& matrix, int i, int j, int color) {
    if (i < 0 || i >= matrix.size() || j < 0 || j >= matrix[0].size())
        return;
    if (matrix[i][j] != 1)
        return;

    matrix[i][j] = color;

    floodFill(matrix, i + 1, j, color);
    floodFill(matrix, i - 1, j, color);
    floodFill(matrix, i, j + 1, color);
    floodFill(matrix, i, j - 1, color);
}

void fillZeroes(std::vector<std::vector<int>>& matrix, int i, int j, int neighborValue) {
    if (i < 0 || i >= matrix.size() || j < 0 || j >= matrix[0].size())
        return;

    if (matrix[i][j] == 0) {
        matrix[i][j] = neighborValue;
        fillZeroes(matrix, i + 1, j, neighborValue);
        fillZeroes(matrix, i - 1, j, neighborValue);
        fillZeroes(matrix, i, j + 1, neighborValue);
        fillZeroes(matrix, i, j - 1, neighborValue);
    }
}

int colorIslands(std::vector<std::vector<int>>& matrix) {
    int color = 2;
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j] == 1) {
                floodFill(matrix, i, j, color);
                color++;
            }
        }
    }

    // Заполнение нулей соседними значениями
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j] == 0) {
                int neighborValue = 1; // По умолчанию соседнее значение равно 1
                // Поиск первого ненулевого соседнего значения
                if (i > 0 && matrix[i - 1][j] != 0)
                    neighborValue = matrix[i - 1][j];
                else if (i < matrix.size() - 1 && matrix[i + 1][j] != 0)
                    neighborValue = matrix[i + 1][j];
                else if (j > 0 && matrix[i][j - 1] != 0)
                    neighborValue = matrix[i][j - 1];
                else if (j < matrix[0].size() - 1 && matrix[i][j + 1] != 0)
                    neighborValue = matrix[i][j + 1];

                fillZeroes(matrix, i, j, neighborValue);
            }
        }
    }
    return color;
}

unordered_map<int, unordered_map<int, int>> calculateVote(const std::vector<std::vector<int>>& matrix,
                                                          const std::vector<std::vector<int>>& matrix2,
                                                          int numberOfDistricts,
                                                          int numberOfCandidates)
{
    minDistrict = numberOfDistricts;
    maxDistrict = numberOfDistricts;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (minDistrict > matrix[i][j]) minDistrict = matrix[i][j];
            if (maxDistrict < matrix[i][j]) maxDistrict = matrix[i][j];
        }
    }
    unordered_map<int, unordered_map<int, int>> resultMap;
    for (int i = minDistrict; i <= maxDistrict; i++)
    {
        unordered_map<int, int> candidatesPerDistrictMap;
        for (int j = 1; j <= numberOfCandidates; j++)
        {
            candidatesPerDistrictMap.insert(make_pair(j, 0));
        }
        resultMap.insert(make_pair(i, candidatesPerDistrictMap));
    }

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            int districtID = matrix[i][j];
            int candidateID = matrix2[i][j];
            resultMap[districtID][candidateID]++;
        }
    }

    return resultMap;
}

void printVotesResult(unordered_map<int, unordered_map<int, int>> votesResult)
{
    for (const auto& outerPair : votesResult) {
        int outerKey = outerPair.first;
        const std::unordered_map<int, int>& innerMap = outerPair.second;

        std::cout << "District: " << outerKey << std::endl;

        // Iterate through the inner map
        for (const auto& innerPair : innerMap) {
            int innerKey = innerPair.first;
            int innerValue = innerPair.second;

            std::cout << "Candidate: " << innerKey << ", Votes: " << innerValue << std::endl;
        }
    }
}

unordered_map<int, int> calculateCandidateWinPerDistrict(unordered_map<int, unordered_map<int, int>> votesMap,
                                                         int minDistrict,
                                                         int maxDistrict,
                                                         int candidatesNumber)
{
    unordered_map<int, int> resultMap;
    for (int candidateID = 1 ; candidateID <= candidatesNumber; candidateID++)
    {
        int numberOfVotedDistricts = 0;
        for (int districtID = minDistrict; districtID <= maxDistrict; districtID++)
        {
            bool isWin = true;
            int candidateVotes = votesMap[districtID][candidateID];
            for (int anotherCandidateID = 1; anotherCandidateID <= candidatesNumber; anotherCandidateID++)
            {
                if (anotherCandidateID == candidateID) continue;
                int anotherCandidateVotes = votesMap[districtID][anotherCandidateID];
                if (anotherCandidateVotes >= candidateVotes) isWin = false;
            }
            if (isWin) numberOfVotedDistricts++;
        }
        resultMap.insert(make_pair(candidateID, numberOfVotedDistricts));
    }
    return resultMap;
}

void printWinsPerDistrict(unordered_map<int, int> winsPerDistrictMap)
{
    for (const auto& outerPair : winsPerDistrictMap) {
        int candidateID = outerPair.first;
        int numberOfDisctricts = outerPair.second;
        cout << "Candidate: " << candidateID << " Number of districts: " << numberOfDisctricts << endl;
    }
}

int calculateAbsoluteWinner(unordered_map<int, int> winsPerDistrictMap, int numberOfCandidates)
{
    int winnerID = 1;
    for (int candidateID = winnerID+1; candidateID <= numberOfCandidates; candidateID++)
    {
        if (winsPerDistrictMap[winnerID] < winsPerDistrictMap[candidateID]) winnerID = candidateID;
    }
    for (int candidateID = 1; candidateID <= numberOfCandidates; candidateID++)
    {
        if (candidateID == winnerID) continue;
        if (winsPerDistrictMap[winnerID] == winsPerDistrictMap[candidateID]) winnerID = 0;
    }
    return winnerID;
}

int main() {
    int M, N;
    std::cout << "Enter M: ";
    std::cin >> M;
    while (std::cin.fail() || M > 40 || M < 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: please enter correct data:\n";
        std::cin >> M;
    }

    std::cout << "Enter N: ";
    std::cin >> N;
    while (std::cin.fail() || N > 40 || N < 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: please enter correct data:\n";
        std::cin >> N;
    }
    int numberOfCandidates;
    std::cout << "Enter number of canditates: ";
    std::cin >> numberOfCandidates;

    srand(static_cast<unsigned>(time(NULL)));
    std::vector<std::vector<int>> matrix(M, std::vector<int>(N));
    std::vector<std::vector<int>> votersMatrix(M, std::vector<int>(N));
    fillVotersMatrix(&votersMatrix, numberOfCandidates);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 2;
        }
    }

    int numberOfDistricts = colorIslands(matrix);
    numberOfDistricts--;
    printMatrix(matrix, votersMatrix);

    unordered_map<int, unordered_map<int, int>> votesResult = calculateVote(matrix, votersMatrix, numberOfDistricts, numberOfCandidates);
    printVotesResult(votesResult);
    unordered_map<int, int> winsPerDistrict = calculateCandidateWinPerDistrict(votesResult, minDistrict, maxDistrict, numberOfCandidates);
    printWinsPerDistrict(winsPerDistrict);
    int winnerID = calculateAbsoluteWinner(winsPerDistrict, numberOfCandidates);

    if (winnerID == 0)
    {
        cout << "No one is winner" << endl;
    }
    else
    {
        cout << "Winner is " << winnerID << endl;
    }
    system("pause");
    return 0;
}
