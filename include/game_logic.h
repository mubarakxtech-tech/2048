#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_types.h"

// Memory & lifecycle
bool InitGame(GameState *state, GameConfig config);
void ResetGame(GameState *state);
void FreeGame(GameState *state);

// Core mechanics
bool MoveTiles(GameState *state, MoveDir dir);
void SpawnRandomTile(GameState *state);
bool UndoLastMove(GameState *state);

// Board queries
int GetTileAt(const GameState *state, int row, int col);
int GetHighestTile(const GameState *state);
bool HasValidMoves(const GameState *state);

#endif