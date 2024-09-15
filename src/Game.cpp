#include <iostream>
#include <thread>
#include <vector>
#include "raylib.h"
#include "Game.h"

using namespace std;

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
        if (sumOfAliveNeighbours > 3 || sumOfAliveNeighbours < 2) {
            return 0;
        }
        return 1;
    } else {
        if (sumOfAliveNeighbours == 3) {
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
    unsigned int generation = 0;

    vector<vector<int>> secondMatrix(NUM_OF_CELLS_IN_COLLUMN, vector<int>(NUM_OF_CELLS_IN_ROW, 0));
    vector<vector<int>> mainMatrix(NUM_OF_CELLS_IN_COLLUMN, vector<int>(NUM_OF_CELLS_IN_ROW, 0));

    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(60);

    Rectangle restartButton = {20, 25, 120, 50};
    Rectangle startButton = {150, 25, 120, 50};
    Rectangle playButton = {(screenWidth / 2) - 60, (screenHeight - 140), 120, 50};
    Rectangle quitButton = {(screenWidth / 2) - 60, (screenHeight - 80), 120, 50};
    Rectangle randomizeButton = {screenWidth - 300, 25, 150, 50};
    Rectangle toRulesButton = {screenWidth - 140, 25, 120, 50};

    char *startButtonText = "START";
    bool startButtonState = false;  // Play button state: false-PAUSED, true-STARTED;

    unsigned short startBtnState = 0;    // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool startButtonAction = false;

    unsigned short restartBtnState = 0;    // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool restartButtonAction = false;

    unsigned short quitBtnState = 0;    // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool quitButtonAction = false;

    unsigned short toRulesButtonState = 0;    // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool toRulesButtonAction = false;

    unsigned short randomizeBtnState = 0;    // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool randomizeButtonAction = false;

    unsigned short playBtnState = 0;    // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool playButtonAction = false;


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
                // Check play button state
                if (CheckCollisionPointRec(mousePoint, playButton)) {

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        playBtnState = 2;
                    } else playBtnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) playButtonAction = true;
                } else playBtnState = 0;

                // Check quit button state
                if (CheckCollisionPointRec(mousePoint, quitButton)) {

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        quitBtnState = 2;
                    } else quitBtnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) quitButtonAction = true;
                } else quitBtnState = 0;


                // Handle PLAY button click
                if (playButtonAction) {
                    gameState = GAME;
                    playButtonAction = false;
                }

                // Handle QUIT button click
                if (quitButtonAction) {
                    shouldQuit = true;
                }
                break;
            }
            case GAME: {

                // Check start/pause button state
                if (CheckCollisionPointRec(mousePoint, startButton)) {

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        startBtnState = 2;
                    } else startBtnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) startButtonAction = true;
                } else startBtnState = 0;

                // Check restart button state
                if (CheckCollisionPointRec(mousePoint, restartButton)) {

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        restartBtnState = 2;
                    } else restartBtnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) restartButtonAction = true;
                } else restartBtnState = 0;


                // Check randomize button state
                if (CheckCollisionPointRec(mousePoint, randomizeButton)) {

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        randomizeBtnState = 2;
                    } else randomizeBtnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) randomizeButtonAction = true;
                } else randomizeBtnState = 0;

                if (mousePoint.y > 100 && (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))) {
                    int cellPosX = mousePoint.x / CELL_SIZE;
                    int cellPosY = (mousePoint.y - 100) / CELL_SIZE;
                    mainMatrix[cellPosY][cellPosX] = !mainMatrix[cellPosY][cellPosX];
                }

                // Check 'to settings' button state
                if (CheckCollisionPointRec(mousePoint, toRulesButton)) {

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        toRulesButtonState = 2;
                    } else toRulesButtonState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) toRulesButtonAction = true;
                } else toRulesButtonAction = 0;

                // Handle PLAY button click
                if (startButtonAction) {
                    if (startButtonState) {
                        startButtonText = "START";
                    } else {
                        startButtonText = "PAUSE";
                    }
                    startButtonState = !startButtonState;
                    startButtonAction = false;
                }

                // Handle RESTART button click
                if (restartButtonAction) {
                    startButtonState = false;
                    startButtonText = "START";
                    generation = 0;
                    setAllZerosToArray(mainMatrix);
                    restartButtonAction = false;
                }

                // Handle RANDOMIZE button click
                if (randomizeButtonAction) {
                    startButtonState = false;
                    startButtonText = "START";
                    generation = 0;
                    mainMatrix = getInitialMatrixOnRandomBasis(NUM_OF_CELLS_IN_COLLUMN,
                                                               NUM_OF_CELLS_IN_ROW);
                    randomizeButtonAction = false;
                }

                // Handle TO SETTINGS button click
                if (toRulesButtonAction) {
                    gameState = RULES;
                    toRulesButtonAction = false;
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

                DrawText("RULES", (screenWidth / 2) - 60, (int) 50, 36, RED);
                DrawText("Any live cell with fewer than two live neighbours dies, as if by underpopulation.", 200, 300, 20, BLACK);
                DrawText("Any live cell with two or three live neighbours lives on to the next generation.", 200, 340, 20, BLACK);
                DrawText("Any live cell with more than three live neighbours dies, as if by overpopulation.", 200, 380, 20, BLACK);
                DrawText("Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.", 160, 420, 20, BLACK);

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
                DrawText(TextFormat("RANDOMIZE"), (int) randomizeButton.x + 15, (int) randomizeButton.y + 16, 20,
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
