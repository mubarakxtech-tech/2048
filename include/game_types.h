#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdbool.h>

#define MAX_NAME_LEN 32
#define MAX_LEADERBOARD_ENTRIES 10

typedef enum {
    SCREEN_MENU = 0,    // Main menu screen
    SCREEN_CONFIG,      // Configuration / Settings screen
    SCREEN_GAMEPLAY,    // Active 2048 grid gameplay screen
    SCREEN_PAUSED,      // Pause overlay over active gameplay
    SCREEN_GAMEOVER,    // Game Over screen (win / loss summary)
    SCREEN_LEADERBOARD  // Leaderboard screen displaying top CSV records
} GameScreen;


typedef enum {
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} MoveDir;


typedef struct {
    char playerName[MAX_NAME_LEN];
    int gridSize;       // 3 to 8
    int baseValue;      // 2, 4, etc.
    int multiplier;     // 2, 3, etc.
    float timeLimit;    // In seconds (0.0f = infinite)
    bool isEndless;     // True = don't stop at 2048
} GameConfig;


typedef struct {
    int gridSize;
    int baseValue;
    int multiplier;
    float timeLimit;
    float timeRemaining;
    bool isEndless;

    int score;
    int prevScore;
    int maxTile;

    bool canUndo;
    bool isGameOver;
    bool targetReached;

    int *grid;         // Dynamic 1D array representing 2D board
    int *prevGrid;     // Dynamic 1D array backup for 1-step undo
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

#endif // GAME_TYPES_H
