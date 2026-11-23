#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "raylib.h"
#include "game_types.h"
#include "game_logic.h"
#include "file_manager.h"

// Function prototypes for UI screens
void DrawMenuScreen(GameScreen *currentScreen, bool hasSavedGame, GameState *game);
void DrawConfigScreen(GameScreen *currentScreen, GameConfig *config, GameState *game);
void DrawGameplayScreen(GameScreen *currentScreen, GameState *game);
void DrawPauseOverlay(GameScreen *currentScreen, GameState *game);
void DrawGameOverScreen(GameScreen *currentScreen, GameState *game, bool *scoreSaved);
void DrawLeaderboardScreen(GameScreen *currentScreen, ScoreRecord leaderboard[], int recordCount);

// Helper function for tile colors
Color GetTileColor(int value);

#endif // UI_RENDERER_H
