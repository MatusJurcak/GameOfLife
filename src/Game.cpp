#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "raylib.h"
#include "Game.h"

using namespace std;

int underPopulationVal = DEFAULT_UNDERPOPULATION_VALUE;
int overPopulationVal = DEFAULT_OVERPOPULATION_VALUE;
int reproductionVal = DEFAULT_REPRODUCTION_VALUE;

void setTimeout(int milisecs) {
    this_thread::sleep_for(chrono::milliseconds(milisecs));
}

int nextGenerationCellCheck(vector<vector<int>> &oldMatrix, unsigned int &posY, unsigned int &posX) {
    int neighboursPosY[8]{1, 1, 1, 0, 0, -1, -1, -1};
    int neighboursPosX[8]{-1, 0, 1, -1, 1, -1, 0, 1};
    unsigned int sumOfAliveNeighbours = 0;

    for (int i = 0; i < 8; i++) {
        int neighbourPosY = posY + neighboursPosY[i];
        int neighbourPosX = posX + neighboursPosX[i];
        if (neighbourPosY > -1 && neighbourPosY < oldMatrix.size() &&
            neighbourPosX > -1 && neighbourPosX < oldMatrix[0].size()) {
            if (oldMatrix[posY + neighboursPosY[i]][posX + neighboursPosX[i]] == 1) {
                sumOfAliveNeighbours++;
            }
        }
    }

    if (oldMatrix[posY][posX] == 1) {
        if (sumOfAliveNeighbours > overPopulationVal || sumOfAliveNeighbours < underPopulationVal) {
            return 0;
        }
        return 1;
    } else {
        if (sumOfAliveNeighbours == reproductionVal) {
            return 1;
        }
    }
    return 0;
}

void updateMatrixForNextGeneration(vector<vector<int>> &oldMatrix, vector<vector<int>> &newMatrix) {
    for (unsigned int i = 0; i < oldMatrix.size(); i++) {
        for (unsigned int j = 0; j < oldMatrix[i].size(); j++) {
            newMatrix[i][j] = nextGenerationCellCheck(oldMatrix, i, j);
        }
    }
}

void setAllZerosToArray(vector<vector<int>> &matrix) {
    for (unsigned int i = 0; i < matrix.size(); i++) {
        for (unsigned int j = 0; j < matrix[i].size(); j++) {
            matrix[i][j] = 0;
        }
    }
}

vector<vector<int>> getInitialMatrixOnRandomBasis(int numInACollumn, int numInARow) {
    vector<vector<int>> matrix(numInACollumn, vector<int>(numInARow, 0));
    for (int i = 0; i < numInACollumn; i++) {
        for (int j = 0; j < numInARow; j++) {
            matrix[i][j] = rand() % 2 == 0;
        }
    }
    return matrix;
}

void DrawCells(vector<vector<int>> &matrix) {
    for (int i = 0; i < NUM_OF_CELLS_IN_COLLUMN; i++) {
        for (int j = 0; j < NUM_OF_CELLS_IN_ROW; j++) {
            DrawRectangle(j * CELL_SIZE, i * CELL_SIZE + 100, CELL_SIZE, CELL_SIZE, matrix[i][j] == 1 ? BLACK : WHITE);
            DrawRectangleLines(j * CELL_SIZE, i * CELL_SIZE + 100, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
        }
    }
}

void loop() {
    GameState gameState = RULES;
    bool shouldQuit = false;
    bool firstInputFocused = false;
    bool secondInputFocused = false;
    bool thirdInputFocused = false;
    unsigned int generation = 0;

    vector<vector<int>> secondMatrix(NUM_OF_CELLS_IN_COLLUMN, vector<int>(NUM_OF_CELLS_IN_ROW, 0));
    vector<vector<int>> mainMatrix(NUM_OF_CELLS_IN_COLLUMN, vector<int>(NUM_OF_CELLS_IN_ROW, 0));

    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(60);

    Rectangle firstInput = {(screenWidth / 2.0f) + 120, 120, 40, 40};
    Rectangle secondInput = {(screenWidth / 2.0f) + 120, 180, 40, 40};
    Rectangle thirdInput = {(screenWidth / 2.0f) + 120, 240, 40, 40};
    char underPopulationValue[2];
    underPopulationValue[0] = '0' + DEFAULT_UNDERPOPULATION_VALUE;
    char overPopulationValue[2];
    overPopulationValue[0] = '0' + DEFAULT_OVERPOPULATION_VALUE;
    char reproductionValue[2];
    reproductionValue[0] = '0' + DEFAULT_REPRODUCTION_VALUE;

    int letterCount1 = 1;
    int letterCount2 = 1;
    int letterCount3 = 1;
    Rectangle restartButton = {20, 25, 120, 50};
    Rectangle startButton = {150, 25, 120, 50};
    Rectangle playButton = {(screenWidth / 2.0f) - 60, (screenHeight - 140), 120, 50};
    Rectangle quitButton = {(screenWidth / 2.0f - 60), (screenHeight - 80), 120, 50};
    Rectangle randomizeButton = {screenWidth - 300, 25, 150, 50};
    Rectangle toRulesButton = {screenWidth - 140, 25, 120, 50};

    char *startButtonText = "START";
    bool startButtonState = false;  // Play button state: false-PAUSED, true-STARTED;

    Vector2 mousePoint = {0.0f, 0.0f};


    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();

        if (shouldQuit) {
            break;
        }

        switch (gameState) {
            case RULES: {

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    if (CheckCollisionPointRec(mousePoint, playButton)) {
                        gameState = GAME;
                    } else if (CheckCollisionPointRec(mousePoint, quitButton)) {
                        shouldQuit = true;
                    } else if (CheckCollisionPointRec(mousePoint, firstInput)) {
                        firstInputFocused = true;
                        secondInputFocused = false;
                        thirdInputFocused = false;
                    } else if (CheckCollisionPointRec(mousePoint, secondInput)) {
                        firstInputFocused = false;
                        secondInputFocused = true;
                        thirdInputFocused = false;
                    } else if (CheckCollisionPointRec(mousePoint, thirdInput)) {
                        firstInputFocused = false;
                        secondInputFocused = false;
                        thirdInputFocused = true;
                    } else {
                        firstInputFocused = false;
                        secondInputFocused = false;
                        thirdInputFocused = false;
                    }
                }

                if (firstInputFocused || secondInputFocused || thirdInputFocused) {
                    int key = GetCharPressed();
                    if ((key >= 48) && (key <= 56)) {
                        if (firstInputFocused) {
                            underPopulationValue[0] = (char) key;
                            underPopulationVal = key - 48;
                            if(underPopulationVal > overPopulationVal){
                                overPopulationValue[0] = underPopulationValue[0];
                                overPopulationVal = underPopulationVal;
                            }
                            letterCount1++;
                        }
                        if (secondInputFocused) {
                            overPopulationValue[0] = (char) key;
                            overPopulationVal = key - 48;
                            if(overPopulationVal < underPopulationVal){
                                underPopulationValue[0] = overPopulationValue[0];
                                underPopulationVal = overPopulationVal;
                            }
                            letterCount2++;
                        }
                        if (thirdInputFocused) {
                            reproductionValue[0] = (char) key;
                            reproductionVal = key - 48;
                            letterCount3++;
                        }
                    }
                }
                break;
            }
            case GAME: {

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    if (CheckCollisionPointRec(mousePoint, startButton)) {
                        if (startButtonState) {
                            startButtonText = "START";
                        } else {
                            startButtonText = "PAUSE";
                        }
                        startButtonState = !startButtonState;
                    }
                    if (CheckCollisionPointRec(mousePoint, restartButton)) {
                        startButtonState = false;
                        startButtonText = "START";
                        generation = 0;
                        setAllZerosToArray(mainMatrix);
                    }
                    if (CheckCollisionPointRec(mousePoint, randomizeButton)) {
                        startButtonState = false;
                        startButtonText = "START";
                        generation = 0;
                        mainMatrix = getInitialMatrixOnRandomBasis(NUM_OF_CELLS_IN_COLLUMN,
                                                                   NUM_OF_CELLS_IN_ROW);
                    }
                    if (CheckCollisionPointRec(mousePoint, toRulesButton)) {
                        gameState = RULES;
                    }
                }

                if (mousePoint.y > 100 && (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))) {
                    int cellPosX = mousePoint.x / CELL_SIZE;
                    int cellPosY = (mousePoint.y - 100) / CELL_SIZE;
                    mainMatrix[cellPosY][cellPosX] = !mainMatrix[cellPosY][cellPosX];
                }

                break;
            }
            default:
                break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        switch (gameState) {
            case RULES: {
                ClearBackground(WHITE);

                DrawText("RULES", (screenWidth / 2) - (MeasureText("RULES", 36) / 2), (int) 50, 36, RED);

                DrawText("Underpopulation value: ", (int) 400, (int) firstInput.y + 8, 25, BLACK);
                DrawText("Overpopulation value: ", (int) 400, (int) secondInput.y + 8, 25, BLACK);
                DrawText("Reproduction value: ", (int) 400, (int) thirdInput.y + 8, 25, BLACK);
                DrawRectangleRec(firstInput, LIGHTGRAY);
                DrawText(underPopulationValue, (int) firstInput.x + 20 - (MeasureText(underPopulationValue, 30) / 2),
                         (int) firstInput.y + 6, 30, MAROON);
                if (firstInputFocused) {
                    DrawRectangleLines((int) firstInput.x, (int) firstInput.y, (int) firstInput.width,
                                       (int) firstInput.height,
                                       RED);
                }
                DrawRectangleRec(secondInput, LIGHTGRAY);
                DrawText(overPopulationValue, (int) secondInput.x + 20 - (MeasureText(overPopulationValue, 30) / 2),
                         (int) secondInput.y + 6, 30, MAROON);
                if (secondInputFocused) {
                    DrawRectangleLines((int) secondInput.x, (int) secondInput.y, (int) secondInput.width,
                                       (int) secondInput.height,
                                       RED);
                }
                DrawRectangleRec(thirdInput, LIGHTGRAY);
                DrawText(reproductionValue, (int) thirdInput.x + 20 - (MeasureText(reproductionValue, 30) / 2),
                         (int) thirdInput.y + 6, 30, MAROON);
                if (thirdInputFocused) {
                    DrawRectangleLines((int) thirdInput.x, (int) thirdInput.y, (int) thirdInput.width,
                                       (int) thirdInput.height,
                                       RED);
                }

                DrawText(
                        TextFormat("Any alive cell with fewer than %i alive neighbours dies, as if by underpopulation.",
                                   underPopulationVal), (screenWidth / 2) - (MeasureText(TextFormat(
                                "Any alive cell with fewer than %i alive neighbours dies, as if by underpopulation.",
                                underPopulationVal), 20) / 2), 350, 20, BLACK);
                DrawText(TextFormat("Any alive cell with more than %i alive neighbours dies, as if by overpopulation.",
                                    overPopulationVal), (screenWidth / 2 - (MeasureText(
                        TextFormat("Any alive cell with more than %i alive neighbours dies, as if by overpopulation.", overPopulationVal),
                        20) / 2)), 390, 20, BLACK);
                DrawText(TextFormat("Any alive cell with %i or %i alive neighbours lives on to the next generation.",
                                    underPopulationVal, overPopulationVal), (screenWidth / 2 -
                                                                                                   (MeasureText(
                                                                                                           TextFormat(
                                                                                                                   "Any alive cell with %i or %i alive neighbours lives on to the next generation.",
                                                                                                                   underPopulationVal,
                                                                                                                   overPopulationVal),
                                                                                                           20) / 2)),
                         430, 20, BLACK);
                DrawText(TextFormat(
                        "Any dead cell with exactly %i alive neighbours becomes an alive cell, as if by reproduction.",
                        reproductionVal), (screenWidth / 2) - (MeasureText(TextFormat(
                        "Any dead cell with exactly %i alive neighbours becomes an alive cell, as if by reproduction.",
                        reproductionVal), 20) / 2), 470, 20, BLACK);

                // Draw PLAY button
                DrawRectangleRec(playButton, LIGHTGRAY);
                DrawText("PLAY", (int) playButton.x + 35, (int) playButton.y + 16, 20, RED);

                // Draw QUIT button
                DrawRectangleRec(quitButton, LIGHTGRAY);
                DrawText("QUIT", (int) quitButton.x + 35, (int) quitButton.y + 16, 20, RED);
                break;

            }
            case GAME: {
                ClearBackground(WHITE);

                // Draw RESTART button
                DrawRectangleRec(restartButton, LIGHTGRAY);
                DrawText("RESTART", (int) restartButton.x + 10, (int) restartButton.y + 16, 20, RED);

                // Draw PAUSE button
                DrawRectangleRec(startButton, LIGHTGRAY);
                DrawText(startButtonText, (int) startButton.x + 25,
                         (int) startButton.y + 16,
                         20,
                         RED);

                // Draw text with generation info
                DrawText(TextFormat("GENERATION: %i", generation), 500, 40, 25, RED);

                // Draw RANDOMIZE box
                DrawRectangleRec(randomizeButton, LIGHTGRAY);
                DrawText("RANDOMIZE", (int) randomizeButton.x + 15, (int) randomizeButton.y + 16, 20,
                         BLACK);

                // Draw TO SETTINGS button
                DrawRectangleRec(toRulesButton, LIGHTGRAY);
                DrawText("RULES", (int) toRulesButton.x + 25, (int) toRulesButton.y + 16, 20, RED);


                // Draw line that seperates "menu section" from the grid
                DrawLineV((Vector2) {0, 100}, (Vector2) {(float) screenWidth, 100},
                          DARKGRAY);

                DrawCells(mainMatrix);
                break;
            }
            default:
                break;
        }
        EndDrawing();


        if (gameState == GAME && startButtonState) {
            setTimeout(60);
            updateMatrixForNextGeneration(mainMatrix, secondMatrix);
            mainMatrix = secondMatrix;
            generation++;
        }
    }

    CloseWindow();

}
