#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdbool.h>

#define MAX_NAME_LEN 32
#define MAX_LEADERBOARD_ENTRIES 10

/**
 * @brief Active UI screen state in the Raylib state machine.
 */
typedef enum {
    SCREEN_MENU = 0,       // Main menu screen
    SCREEN_CONFIG,         // Settings / config screen
    SCREEN_GAMEPLAY,       // Active 2048 grid gameplay screen
    SCREEN_PAUSED,         // Pause overlay screen
    SCREEN_GAMEOVER,       // Game Over summary screen
    SCREEN_LEADERBOARD     // High scores leaderboard
} GameScreen;

/**
 * @brief Directional inputs for tile movement.
 */
typedef enum {
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} MoveDir;

/**
 * @brief User preferences and initial game configuration.
 */
typedef struct {
    char playerName[MAX_NAME_LEN];
    int gridSize;          // 3x3 to 8x8
    int baseValue;         // Starting tile value (e.g. 2)
    int multiplier;        // Merge multiplier (e.g. 2)
    float timeLimit;       // Seconds (0.0f = no timer)
    bool isEndless;        // True = no win tile limit
} GameConfig;

/**
 * @brief Runtime state of an active 2048 play session.
 */
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

    int *grid;             // Dynamic 1D array (gridSize x gridSize)
    int *prevGrid;         // Snapshot for single-step undo
} GameState;

/**
 * @brief Leaderboard entry record for CSV storage.
 */
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
