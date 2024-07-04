#ifndef LAB3_ELECTIONS_H
#define LAB3_ELECTIONS_H

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include "OutputHelper.h"

using namespace std;

class Elections {
private:
    int minDistrict, maxDistrict;
    int getRand(int min, int max);
    void fillVotersMatrix(vector<vector<int>> *votersMatrix, int numberOfCandidates);
    void printMatrix(const std::vector<std::vector<int>>& matrix, const std::vector<std::vector<int>>& matrix2);
    void floodFill(std::vector<std::vector<int>>& matrix, int i, int j, int color);
    void fillZeroes(std::vector<std::vector<int>>& matrix, int i, int j, int neighborValue);
    int colorIslands(std::vector<std::vector<int>>& matrix);
    unordered_map<int, unordered_map<int, int>> calculateVote(const std::vector<std::vector<int>>& matrix,
                                                              const std::vector<std::vector<int>>& matrix2,
                                                              int numberOfDistricts,
                                                              int numberOfCandidates);
    void printVotesResult(unordered_map<int, unordered_map<int, int>> votesResult);
    unordered_map<int, int> calculateCandidateWinPerDistrict(unordered_map<int, unordered_map<int, int>> votesMap,
                                                             int minDistrict,
                                                             int maxDistrict,
                                                             int candidatesNumber);
    void printWinsPerDistrict(unordered_map<int, int> winsPerDistrictMap);
    int calculateAbsoluteWinner(unordered_map<int, int> winsPerDistrictMap, int numberOfCandidates);
public:
    void makeElections(int M, int N, int numberOfCandidates);

};


#endif //LAB3_ELECTIONS_H
