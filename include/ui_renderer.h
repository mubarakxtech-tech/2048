#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "raylib.h"
#include "game_types.h"
#include "game_logic.h"
#include "file_manager.h"


Color GetTileColor(int value);


void DrawMenuScreen(GameScreen *currentScreen, bool hasSaveGame, GameState *game);


void DrawConfigScreen(GameScreen *currentScreen, GameConfig *config, GameState *game);


void DrawGameplayScreen(GameScreen *currentScreen, GameState *game);


void DrawPauseOverlay(GameScreen *currentScreen, GameState *game);


void DrawGameOverScreen(GameScreen *currentScreen, GameState *game, bool *scoreSaved);


void DrawLeaderboardScreen(GameScreen *currentScreen, const ScoreRecord leaderboard[], int recordCount);

#endif // UI_RENDERER_H
