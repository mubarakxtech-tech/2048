#include "game_logic.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool InitGame(GameState *state, GameConfig config) {
    if (state == NULL) return false;

    if (config.gridSize < 3) config.gridSize = 3;
    if (config.gridSize > 8) config.gridSize = 8;

    state->gridSize = config.gridSize;
    state->baseValue = (config.baseValue > 0) ? config.baseValue : 2;
    state->multiplier = (config.multiplier >= 2) ? config.multiplier : 2;
    state->timeLimit = config.timeLimit;
    state->timeRemaining = config.timeLimit;
    state->isEndless = config.isEndless;

    state->score = 0;
    state->prevScore = 0;
    state->maxTile = 0;
    state->canUndo = false;
    state->isGameOver = false;
    state->targetReached = false;

    int totalCells = state->gridSize * state->gridSize;
    state->grid = (int *)calloc(totalCells, sizeof(int));
    state->prevGrid = (int *)calloc(totalCells, sizeof(int));

    if (state->grid == NULL || state->prevGrid == NULL) {
        FreeGame(state);
        return false;
    }

    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    SpawnRandomTile(state);
    SpawnRandomTile(state);

    return true;
}

void FreeGame(GameState *state) {
    if (state == NULL) return;

    if (state->grid != NULL) {
        free(state->grid);
        state->grid = NULL;
    }
    if (state->prevGrid != NULL) {
        free(state->prevGrid);
        state->prevGrid = NULL;
    }
}

int GetTileAt(const GameState *state, int row, int col) {
    if (state == NULL || state->grid == NULL) return -1;
    if (row < 0 || row >= state->gridSize || col < 0 || col >= state->gridSize) {
        return -1;
    }
    return state->grid[row * state->gridSize + col];
}

void SpawnRandomTile(GameState *state) {
    if (state == NULL || state->grid == NULL) return;

    int totalCells = state->gridSize * state->gridSize;
    int emptyIndices[64];
    int emptyCount = 0;

    for (int i = 0; i < totalCells; i++) {
        if (state->grid[i] == 0) {
            emptyIndices[emptyCount++] = i;
        }
    }

    if (emptyCount == 0) return;

    int randomIndex = emptyIndices[rand() % emptyCount];
    int newValue = (rand() % 10 == 0)
                   ? (state->baseValue * state->multiplier)
                   : state->baseValue;

    state->grid[randomIndex] = newValue;

    if (newValue > state->maxTile) {
        state->maxTile = newValue;
    }
}
// Internal helper: slides and merges a single 1D row or column toward index 0
static bool SlideAndMergeLine(int line[], int size, int multiplier, int *scoreGained, int *maxTile) {
    int original[8];
    int temp[8] = {0};
    int pos = 0;

    for (int i = 0; i < size; i++) {
        original[i] = line[i];
        // Step 1: Compress non-zero tiles toward the front
        if (line[i] != 0) {
            temp[pos++] = line[i];
        }
    }

    // Step 2: Merge adjacent equal tiles once per move
    for (int i = 0; i < size - 1; i++) {
        if (temp[i] != 0 && temp[i] == temp[i + 1]) {
            temp[i] *= multiplier;
            *scoreGained += temp[i];
            if (temp[i] > *maxTile) {
                *maxTile = temp[i];
            }
            temp[i + 1] = 0;
            i++; // Skip next tile so [2, 2, 4] becomes [4, 4], not [8]
        }
    }

    // Step 3: Compress again to close gaps created by merges
    pos = 0;
    for (int i = 0; i < size; i++) {
        if (temp[i] != 0) {
            line[pos++] = temp[i];
        }
    }
    while (pos < size) {
        line[pos++] = 0;
    }

    // Check if anything actually changed in this row/column
    for (int i = 0; i < size; i++) {
        if (original[i] != line[i]) return true;
    }
    return false;
}

bool MoveTiles(GameState *state, MoveDir dir) {
    if (state == NULL || state->grid == NULL || state->isGameOver) return false;

    int n = state->gridSize;
    int totalCells = n * n;
    int backupGrid[64];
    int backupScore = state->score;

    // Save current state temporarily (only commit to prevGrid if move is valid)
    memcpy(backupGrid, state->grid, totalCells * sizeof(int));

    bool anyMoved = false;
    int line[8];

    for (int i = 0; i < n; i++) {
        // Extract row or column in the direction of movement
        for (int j = 0; j < n; j++) {
            switch (dir) {
                case DIR_LEFT:  line[j] = state->grid[i * n + j]; break;
                case DIR_RIGHT: line[j] = state->grid[i * n + (n - 1 - j)]; break;
                case DIR_UP:    line[j] = state->grid[j * n + i]; break;
                case DIR_DOWN:  line[j] = state->grid[(n - 1 - j) * n + i]; break;
            }
        }

        if (SlideAndMergeLine(line, n, state->multiplier, &state->score, &state->maxTile)) {
            anyMoved = true;
        }

        // Write processed line back into the 1D grid
        for (int j = 0; j < n; j++) {
            switch (dir) {
                case DIR_LEFT:  state->grid[i * n + j] = line[j]; break;
                case DIR_RIGHT: state->grid[i * n + (n - 1 - j)] = line[j]; break;
                case DIR_UP:    state->grid[j * n + i] = line[j]; break;
                case DIR_DOWN:  state->grid[(n - 1 - j) * n + i] = line[j]; break;
            }
        }
    }

    if (anyMoved) {
        // Commit Undo snapshot now that we know the board changed
        memcpy(state->prevGrid, backupGrid, totalCells * sizeof(int));
        state->prevScore = backupScore;
        state->canUndo = true;

        // Check win target if Endless ("No Limit") mode is OFF
        if (!state->isEndless && !state->targetReached) {
            int target = state->baseValue;
            for (int k = 0; k < 10; k++) target *= state->multiplier; // e.g., 2 * 2^10 = 2048
            if (state->maxTile >= target) {
                state->targetReached = true;
            }
        }

        SpawnRandomTile(state);

        if (!HasValidMoves(state)) {
            state->isGameOver = true;
        }
    }

    return anyMoved;
}

bool UndoLastMove(GameState *state) {
    if (state == NULL || !state->canUndo || state->grid == NULL || state->prevGrid == NULL) {
        return false;
    }

    int totalCells = state->gridSize * state->gridSize;
    memcpy(state->grid, state->prevGrid, totalCells * sizeof(int));
    state->score = state->prevScore;
    state->maxTile = GetHighestTile(state);
    state->canUndo = false; // Allow 1 undo per turn
    state->isGameOver = false;

    return true;
}

void ResetGame(GameState *state) {
    if (state == NULL || state->grid == NULL || state->prevGrid == NULL) return;

    int totalCells = state->gridSize * state->gridSize;
    memset(state->grid, 0, totalCells * sizeof(int));
    memset(state->prevGrid, 0, totalCells * sizeof(int));

    state->score = 0;
    state->prevScore = 0;
    state->maxTile = 0;
    state->timeRemaining = state->timeLimit;
    state->canUndo = false;
    state->isGameOver = false;
    state->targetReached = false;

    SpawnRandomTile(state);
    SpawnRandomTile(state);
}

int GetHighestTile(const GameState *state) {
    if (state == NULL || state->grid == NULL) return 0;

    int totalCells = state->gridSize * state->gridSize;
    int highest = 0;
    for (int i = 0; i < totalCells; i++) {
        if (state->grid[i] > highest) {
            highest = state->grid[i];
        }
    }
    return highest;
}

bool HasValidMoves(const GameState *state) {
    if (state == NULL || state->grid == NULL) return false;

    int n = state->gridSize;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            int current = GetTileAt(state, r, c);
            if (current == 0) return true; // Empty cell available
            if (c + 1 < n && current == GetTileAt(state, r, c + 1)) return true; // Right neighbor matches
            if (r + 1 < n && current == GetTileAt(state, r + 1, c)) return true; // Bottom neighbor matches
        }
    }
    return false;
}