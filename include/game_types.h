#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdbool.h>

#define MAX_NAME_LEN 32
#define MAX_LEADERBOARD_ENTRIES 10

typedef enum {
    SCREEN_MENU,
    SCREEN_CONFIG,
    SCREEN_GAMEPLAY,
    SCREEN_PAUSED,
    SCREEN_GAMEOVER,
    SCREEN_LEADERBOARD
} GameScreen;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} MoveDir;

typedef struct {
    char playerName[MAX_NAME_LEN];
    int gridSize;       // 3 to 8 (for NxN grid)
    int baseValue;      // Starting tile value (e.g., 2, 3, or 5)
    int multiplier;     // Merge factor (default: 2)
    float timeLimit;    // Seconds allowed (0.0f = no timer)
    bool isEndless;     // true = No Limit mode
} GameConfig;

typedef struct {
    int *grid;          // Dynamic 1D array: grid[row * gridSize + col]
    int *prevGrid;      // Previous board snapshot for Undo
    int gridSize;
    int score;
    int prevScore;
    int maxTile;
    int baseValue;
    int multiplier;
    float timeLimit;
    float timeRemaining;
    bool isEndless;
    bool canUndo;
    bool isGameOver;
    bool targetReached;
} GameState;

typedef struct {
    char playerName[MAX_NAME_LEN];
    int gridSize;
    int baseValue;
    bool isEndless;
    int score;
    int maxTile;
    float timeLimit;
} ScoreRecord;

#endif