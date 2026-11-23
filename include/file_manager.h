#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "game_types.h"

// Dev 1: Binary save/resume (data/savegame.dat)
bool SaveGameBinary(const GameState *state, const char *filepath);
bool LoadGameBinary(GameState *state, const char *filepath);
bool HasSavedGame(const char *filepath);
void DeleteSavedGame(const char *filepath);

// Dev 2: CSV leaderboard & settings (data/leaderboard.csv, data/settings.txt)
int LoadLeaderboard(ScoreRecord records[], int maxRecords, const char *filepath);
void SaveScoreToLeaderboard(ScoreRecord newRecord, const char *filepath);
void SaveDefaultConfig(const GameConfig *config, const char *filepath);
bool LoadDefaultConfig(GameConfig *config, const char *filepath);

#endif