#include "file_manager.h"
#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// DEV 1 SECTION: Binary Save & Resume (data/savegame.dat)
// ============================================================================

bool SaveGameBinary(const GameState *state, const char *filepath) {
    if (state == NULL || state->grid == NULL || state->prevGrid == NULL || filepath == NULL) {
        return false;
    }

    FILE *fp = fopen(filepath, "wb");
    if (fp == NULL) {
        return false; // Fails if data/ directory does not exist
    }

    // 1. Copy struct metadata and clear heap pointers before writing to disk
    GameState meta = *state;
    meta.grid = NULL;
    meta.prevGrid = NULL;

    if (fwrite(&meta, sizeof(GameState), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    // 2. Write the dynamic grid and prevGrid arrays separately
    int totalCells = state->gridSize * state->gridSize;
    size_t writtenGrid = fwrite(state->grid, sizeof(int), totalCells, fp);
    size_t writtenPrev = fwrite(state->prevGrid, sizeof(int), totalCells, fp);

    fclose(fp);
    return (writtenGrid == (size_t)totalCells && writtenPrev == (size_t)totalCells);
}

bool LoadGameBinary(GameState *state, const char *filepath) {
    if (state == NULL || filepath == NULL) return false;

    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL) return false;

    GameState meta;
    if (fread(&meta, sizeof(GameState), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    // Validate gridSize to guard against corrupted or empty binary files
    if (meta.gridSize < 3 || meta.gridSize > 8) {
        fclose(fp);
        return false;
    }

    // Release any active board already in memory before allocating the loaded one
    FreeGame(state);

    *state = meta;
    state->grid = NULL;
    state->prevGrid = NULL;

    int totalCells = state->gridSize * state->gridSize;
    state->grid = (int *)calloc(totalCells, sizeof(int));
    state->prevGrid = (int *)calloc(totalCells, sizeof(int));

    if (state->grid == NULL || state->prevGrid == NULL) {
        FreeGame(state);
        fclose(fp);
        return false;
    }

    size_t readGrid = fread(state->grid, sizeof(int), totalCells, fp);
    size_t readPrev = fread(state->prevGrid, sizeof(int), totalCells, fp);

    fclose(fp);

    if (readGrid != (size_t)totalCells || readPrev != (size_t)totalCells) {
        FreeGame(state);
        return false;
    }

    return true;
}

bool HasSavedGame(const char *filepath) {
    if (filepath == NULL) return false;

    FILE *fp = fopen(filepath, "rb");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}

void DeleteSavedGame(const char *filepath) {
    if (filepath != NULL) {
        remove(filepath);
    }
}

// ============================================================================
// DEV 2 SECTION: CSV Leaderboard & Text Settings (Add below this line)
// ============================================================================
#include <string.h>

// ============================================================================
// DEV 2 SECTION: CSV Leaderboard & Text Settings
// ============================================================================

int LoadLeaderboard(ScoreRecord records[], int maxRecords, const char *filepath) {
    if (records == NULL || maxRecords <= 0 || filepath == NULL) return 0;

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) return 0; // File does not exist yet on first run

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && count < maxRecords) {
        // Skip CSV header row if present
        if (strncmp(line, "PlayerName,", 11) == 0) continue;

        int endlessInt = 0;
        int parsed = sscanf(line, "%31[^,],%d,%d,%d,%d,%d,%f",
                            records[count].playerName,
                            &records[count].gridSize,
                            &records[count].baseValue,
                            &endlessInt,
                            &records[count].score,
                            &records[count].maxTile,
                            &records[count].timeLimit);

        if (parsed == 7) {
            records[count].isEndless = (endlessInt != 0);
            count++;
        }
    }

    fclose(fp);
    return count;
}

void SaveScoreToLeaderboard(ScoreRecord newRecord, const char *filepath) {
    if (filepath == NULL) return;

    // Sanitize player name: replace commas or empty names so CSV columns never break
    if (strlen(newRecord.playerName) == 0) {
        strcpy(newRecord.playerName, "Player");
    }
    for (int i = 0; newRecord.playerName[i] != '\0'; i++) {
        if (newRecord.playerName[i] == ',') newRecord.playerName[i] = '_';
    }

    ScoreRecord allRecords[MAX_LEADERBOARD_ENTRIES + 1];
    int count = LoadLeaderboard(allRecords, MAX_LEADERBOARD_ENTRIES, filepath);

    // Append the newly finished game's score
    allRecords[count++] = newRecord;

    // Bubble Sort descending by Score (Core PF sorting algorithm)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (allRecords[j].score < allRecords[j + 1].score) {
                ScoreRecord temp = allRecords[j];
                allRecords[j] = allRecords[j + 1];
                allRecords[j + 1] = temp;
            }
        }
    }

    int limit = (count > MAX_LEADERBOARD_ENTRIES) ? MAX_LEADERBOARD_ENTRIES : count;

    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) return;

    fprintf(fp, "PlayerName,GridSize,BaseValue,IsEndless,Score,MaxTile,TimeLimit\n");
    for (int i = 0; i < limit; i++) {
        fprintf(fp, "%s,%d,%d,%d,%d,%d,%.1f\n",
                allRecords[i].playerName,
                allRecords[i].gridSize,
                allRecords[i].baseValue,
                allRecords[i].isEndless ? 1 : 0,
                allRecords[i].score,
                allRecords[i].maxTile,
                allRecords[i].timeLimit);
    }

    fclose(fp);
}

void SaveDefaultConfig(const GameConfig *config, const char *filepath) {
    if (config == NULL || filepath == NULL) return;

    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) return;

    fprintf(fp, "%s\n%d %d %d %.1f %d\n",
            (strlen(config->playerName) > 0) ? config->playerName : "Player",
            config->gridSize,
            config->baseValue,
            config->multiplier,
            config->timeLimit,
            config->isEndless ? 1 : 0);

    fclose(fp);
}

bool LoadDefaultConfig(GameConfig *config, const char *filepath) {
    if (config == NULL || filepath == NULL) return false;

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) return false;

    char nameBuf[MAX_NAME_LEN];
    int gridSize, baseValue, multiplier, endlessInt;
    float timeLimit;

    if (fgets(nameBuf, sizeof(nameBuf), fp) == NULL) {
        fclose(fp);
        return false;
    }
    // Strip trailing newline from fgets
    nameBuf[strcspn(nameBuf, "\r\n")] = '\0';

    if (fscanf(fp, "%d %d %d %f %d", &gridSize, &baseValue, &multiplier, &timeLimit, &endlessInt) != 5) {
        fclose(fp);
        return false;
    }

    fclose(fp);

    strcpy(config->playerName, (strlen(nameBuf) > 0) ? nameBuf : "Player");
    config->gridSize = (gridSize >= 3 && gridSize <= 8) ? gridSize : 4;
    config->baseValue = (baseValue > 0) ? baseValue : 2;
    config->multiplier = (multiplier >= 2) ? multiplier : 2;
    config->timeLimit = (timeLimit >= 0.0f) ? timeLimit : 0.0f;
    config->isEndless = (endlessInt != 0);

    return true;
}