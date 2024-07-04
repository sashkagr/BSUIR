#include "Elections.h"

int Elections::getRand(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void Elections::fillVotersMatrix(vector<vector<int>> *votersMatrix, int numberOfCandidates) {
    for (int i = 0; i < votersMatrix->size(); i++)
    {
        for (int j = 0; j < (*votersMatrix)[i].size(); j++)
        {
            (*votersMatrix)[i][j] = getRand(1, numberOfCandidates);
        }
    }
}

void Elections::printMatrix(const vector<std::vector<int>> &matrix, const vector<std::vector<int>> &matrix2) {
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

            int colorID = 0;
            switch (temp) {
                case 2:
                    colorID = 31;
                    break;
                case 3:
                    colorID = 32;
                    break;
                case 4:
                    colorID = 33;
                    break;
                case 5:
                    colorID = 34;
                    break;
                case 6:
                    colorID = 35;
                    break;
                case 7:
                    colorID = 36;
                    break;
                case 8:
                    colorID = 42;
                    break;
                case 9:
                    colorID = 43;
                    break;
                case 10:
                    colorID = 44;
                    break;
            }
            string outputStr = OutputHelper::intToStr(value, 2) + "(" + OutputHelper::intToStr(value2, 2) + ")";
            cout << OutputHelper::colorizeText(colorID, outputStr);
        }
        std::cout << std::endl;
    }
}

void Elections::floodFill(vector<std::vector<int>> &matrix, int i, int j, int color) {
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

void Elections::fillZeroes(vector<std::vector<int>> &matrix, int i, int j, int neighborValue) {
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

int Elections::colorIslands(vector<std::vector<int>> &matrix) {
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

    if (color < 7)
    {
            for (size_t i = 0; i < matrix.size() && color != 7; i++) {
                for (size_t j = 0; j < matrix[0].size() && color != 7; j++) {
                    if (
                            (j+1 != matrix[0].size() && matrix[i][j] != matrix[i][j+1]) &&
                            ((j-1 > 0 && matrix[i][j-1] == matrix[i][j]) || (i+1 != matrix.size() && matrix[i+1][j] == matrix[i][j]) ||
                                    (i-1 > 0 && matrix[i-1][j] == matrix[i][j])))
                    {
                        color++;
                        matrix[i][j] = color;
                    }
                }
            }
    }
    return color;
}

unordered_map<int, unordered_map<int, int>>
Elections::calculateVote(const vector<std::vector<int>> &matrix, const vector<std::vector<int>> &matrix2,
                         int numberOfDistricts, int numberOfCandidates) {
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

void Elections::printVotesResult(unordered_map<int, unordered_map<int, int>> votesResult) {
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

unordered_map<int, int>
Elections::calculateCandidateWinPerDistrict(unordered_map<int, unordered_map<int, int>> votesMap, int minDistrict,
                                            int maxDistrict, int candidatesNumber) {
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

void Elections::printWinsPerDistrict(unordered_map<int, int> winsPerDistrictMap) {
    for (const auto& outerPair : winsPerDistrictMap) {
        int candidateID = outerPair.first;
        int numberOfDisctricts = outerPair.second;
        cout << "Candidate: " << candidateID << " Number of districts: " << numberOfDisctricts << endl;
    }
}

int Elections::calculateAbsoluteWinner(unordered_map<int, int> winsPerDistrictMap, int numberOfCandidates) {
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

void Elections::makeElections(int M, int N, int numberOfCandidates) {
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
}
