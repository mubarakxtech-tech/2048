#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "game_types.h"

void DrawMenuScreen(GameScreen *currentScreen, bool hasSave, GameState *state);
void DrawConfigScreen(GameScreen *currentScreen, GameConfig *config, GameState *state);
void DrawGameplayScreen(GameScreen *currentScreen, GameState *state);
void DrawPauseOverlay(GameScreen *currentScreen, GameState *state);
void DrawGameOverScreen(GameScreen *currentScreen, GameState *state, bool *scoreSaved);
void DrawLeaderboardScreen(GameScreen *currentScreen, const ScoreRecord records[], int count);

#endif