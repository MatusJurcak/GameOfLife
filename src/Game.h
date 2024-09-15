
#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include <iostream>
#include <thread>
#include <vector>
#include "raylib.h"

using namespace std;

constexpr int screenWidth = 1200;
constexpr int screenHeight = 700;
constexpr int CELL_SIZE = 8;
constexpr int NUM_OF_CELLS_IN_ROW = screenWidth / CELL_SIZE;
constexpr int NUM_OF_CELLS_IN_COLLUMN = (screenHeight - 100) / CELL_SIZE;
constexpr int DEFAULT_UNDERPOPULATION_VALUE = 2;
constexpr int DEFAULT_OVERPOPULATION_VALUE = 3;
constexpr int DEFAULT_REPRODUCTION_VALUE = 3;

enum GameState {
    RULES,
    GAME
};

void setTimeout(int milisecs);

int nextGenerationCellCheck(vector<vector<int>> &oldMatrix, unsigned int &posY, unsigned int &posX);

void updateMatrixForNextGeneration(vector<vector<int>> &oldMatrix, vector<vector<int>> &newMatrix);

void setAllZerosToArray(vector<vector<int>> &matrix);

vector<vector<int>> getInitialMatrixOnRandomBasis(int numInACollumn, int numInARow);

void DrawCells(vector<vector<int>> &matrix);

void loop();

#endif //GAMEOFLIFE_GAME_H
