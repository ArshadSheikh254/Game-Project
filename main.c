#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For getpid()

#define CELL_SIZE 28  // Size (in pixels) of each maze cell

// Enum for difficulty levels
typedef enum {
    EASY = 15,
    MEDIUM = 21,
    HARD = 25,
    QUIT = -1
} Difficulty;

// Struct to hold player coordinates in the maze grid
typedef struct {
    int x, y;
} Player;

// Function declarations
int** AllocateMaze(int rows, int cols);
void FreeMaze(int** maze, int rows);
void GenerateMaze(int** maze, int row, int col, int rows, int cols);
void AddDecoyPaths(int** maze, int rows, int cols, int count);
void DrawMaze(int** maze, int rows, int cols);
int CanMove(int** maze, int newX, int newY, int rows, int cols);
Difficulty ShowDifficultyMenu();

int main() {
    while (1) {
        Difficulty difficulty = ShowDifficultyMenu();
        if (difficulty == QUIT) break;

        int ROWS = difficulty;
        int COLS = difficulty;

        InitWindow(COLS * CELL_SIZE, ROWS * CELL_SIZE, "Maze Escape - Choose Your Challenge");
        SetTargetFPS(60);

        srand((unsigned int)time(NULL) ^ getpid());

        // Allocate and initialize maze
        int** maze = AllocateMaze(ROWS, COLS);
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                maze[i][j] = 1;

        // Generate base maze
        GenerateMaze(maze, 1, 1, ROWS, COLS);

        // Add fake paths for challenge (in medium and hard)
        if (difficulty >= MEDIUM) {
            AddDecoyPaths(maze, ROWS, COLS, difficulty == MEDIUM ? 20 : 40);
        }

        // Set the exit point
        maze[ROWS - 2][COLS - 2] = 2;

        // Player setup
        Player player = {1, 1};
        int gameWon = 0;

        // Gameplay loop
        while (!WindowShouldClose()) {
            if (!gameWon) {
                if (IsKeyPressed(KEY_RIGHT) && CanMove(maze, player.x + 1, player.y, ROWS, COLS)) player.x++;
                if (IsKeyPressed(KEY_LEFT)  && CanMove(maze, player.x - 1, player.y, ROWS, COLS)) player.x--;
                if (IsKeyPressed(KEY_DOWN)  && CanMove(maze, player.x, player.y + 1, ROWS, COLS)) player.y++;
                if (IsKeyPressed(KEY_UP)    && CanMove(maze, player.x, player.y - 1, ROWS, COLS)) player.y--;

                if (maze[player.y][player.x] == 2) gameWon = 1;  // Reached exit
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawMaze(maze, ROWS, COLS);
            DrawCircle(player.x * CELL_SIZE + CELL_SIZE / 2, player.y * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 3, BLUE);

            if (gameWon) {
                DrawText("You Escaped!", 120, (ROWS * CELL_SIZE) / 2 - 20, 40, RED);
                DrawText("Press [Enter] to return to Menu", 80, (ROWS * CELL_SIZE) / 2 + 30, 20, GRAY);
                if (IsKeyPressed(KEY_ENTER)) break;
            }

            EndDrawing();
        }

        FreeMaze(maze, ROWS);
        CloseWindow();
    }

    return 0;
}

// Dynamically allocate a 2D maze array
int** AllocateMaze(int rows, int cols) {
    int** maze = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        maze[i] = (int*)malloc(cols * sizeof(int));
    }
    return maze;
}

// Free memory used by maze
void FreeMaze(int** maze, int rows) {
    for (int i = 0; i < rows; i++) free(maze[i]);
    free(maze);
}

// Function: GenerateMaze
// Description:
// Uses recursive backtracking to carve a path through the maze.
// It ensures the maze is solvable by removing walls between cells.
//
// Acknowledgment:
// This logic was inspired by a maze generation algorithm from:
// https://stackoverflow.com/questions/22747974/random-maze-generator-in-c
// The recursive backtracking structure and directional shuffling
// are adapted and customized to fit this game's mechanics.

void GenerateMaze(int** maze, int row, int col, int rows, int cols) {
    maze[row][col] = 0;

    int directions[4][2] = {{0,2}, {0,-2}, {2,0}, {-2,0}};  // Directions: right, left, down, up

    // Shuffle directions
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int tmpX = directions[i][0], tmpY = directions[i][1];
        directions[i][0] = directions[r][0]; directions[i][1] = directions[r][1];
        directions[r][0] = tmpX; directions[r][1] = tmpY;
    }

    // Try each direction
    for (int i = 0; i < 4; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (newRow > 0 && newRow < rows - 1 && newCol > 0 && newCol < cols - 1 && maze[newRow][newCol] == 1) {
            maze[row + directions[i][0] / 2][col + directions[i][1] / 2] = 0; // Break wall
            GenerateMaze(maze, newRow, newCol, rows, cols);
        }
    }
}

// Adds false/dead-end paths to increase maze complexity
void AddDecoyPaths(int** maze, int rows, int cols, int count) {
    for (int i = 0; i < count; i++) {
        int r = rand() % (rows - 2) + 1;
        int c = rand() % (cols - 2) + 1;
        if (maze[r][c] == 1 && maze[r+1][c] == 1 && maze[r-1][c] == 1 && maze[r][c+1] == 1 && maze[r][c-1] == 1) {
            maze[r][c] = 0;
            if (rand() % 2) maze[r + ((rand() % 2) * 2 - 1)][c] = 0;
            else maze[r][c + ((rand() % 2) * 2 - 1)] = 0;
        }
    }
}

// Renders the maze on screen
void DrawMaze(int** maze, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (maze[row][col] == 1)
                DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
            else if (maze[row][col] == 2) {
                DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
                DrawRectangleLines(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            }
        }
    }
}

// Check if a player can move to the given coordinates
int CanMove(int** maze, int newX, int newY, int rows, int cols) {
    return (newX >= 0 && newX < cols && newY >= 0 && newY < rows && maze[newY][newX] != 1);
}

// Shows difficulty selection menu and returns user's choice
Difficulty ShowDifficultyMenu() {
    InitWindow(500, 350, "Select Difficulty");
    SetTargetFPS(60);

    Difficulty selected = EASY;

    while (!WindowShouldClose()) {
        int key = GetKeyPressed();

        if (key == KEY_ONE) { selected = EASY; break; }
        if (key == KEY_TWO) { selected = MEDIUM; break; }
        if (key == KEY_THREE) { selected = HARD; break; }
        if (key == 'q' || key == 'Q') { selected = QUIT; break; }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Select Maze Difficulty:", 100, 60, 25, BLACK);
        DrawText("1 - Easy", 180, 110, 20, DARKGRAY);
        DrawText("2 - Medium", 180, 150, 20, DARKGRAY);
        DrawText("3 - Hard", 180, 190, 20, DARKGRAY);
        DrawText("Q - Quit", 180, 230, 20, RED);
        EndDrawing();
    }

    CloseWindow();
    return selected;
}
