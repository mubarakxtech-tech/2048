#include "raylib.h"
#include "game_types.h"
#include "game_logic.h"
#include "file_manager.h"
#include "ui_renderer.h"
#include <string.h>

#define SAVE_FILE        "data/savegame.dat"
#define LEADERBOARD_FILE "data/leaderboard.csv"
#define SETTINGS_FILE    "data/settings.txt"

int main(void) {
    const int screenWidth = 900;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Dynamic 2048 - Programming Fundamentals");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // Prevent ESC from closing the window abruptly; use it to Pause

    GameScreen currentScreen = SCREEN_MENU;
    GameState game = {0};

    // Load saved user preferences or fall back to standard 2048 defaults
    GameConfig config = {"Player", 4, 2, 2, 0.0f, false};
    LoadDefaultConfig(&config, SETTINGS_FILE);

    ScoreRecord leaderboard[MAX_LEADERBOARD_ENTRIES];
    int leaderboardCount = LoadLeaderboard(leaderboard, MAX_LEADERBOARD_ENTRIES, LEADERBOARD_FILE);
    bool scoreSaved = false;

    while (!WindowShouldClose()) {
        // --------------------------------------------------------------------
        // 1. UPDATE LOGIC & INPUTS
        // --------------------------------------------------------------------
        if (currentScreen == SCREEN_GAMEPLAY) {
            scoreSaved = false;

            // Frame-independent countdown timer
            if (game.timeLimit > 0.0f && !game.isGameOver) {
                game.timeRemaining -= GetFrameTime();
                if (game.timeRemaining <= 0.0f) {
                    game.timeRemaining = 0.0f;
                    game.isGameOver = true;
                }
            }

            // Keyboard movement & shortcuts
            if (!game.isGameOver) {
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))       MoveTiles(&game, DIR_UP);
                else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))  MoveTiles(&game, DIR_DOWN);
                else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))  MoveTiles(&game, DIR_LEFT);
                else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) MoveTiles(&game, DIR_RIGHT);
                else if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_U))     UndoLastMove(&game);
                else if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
                    currentScreen = SCREEN_PAUSED;
                }
            }

            // Transition to Game Over when board locks or timer expires
            if (game.isGameOver) {
                DeleteSavedGame(SAVE_FILE);
                if (!scoreSaved) {
                    ScoreRecord rec;
                    strcpy(rec.playerName, config.playerName);
                    rec.gridSize = game.gridSize;
                    rec.baseValue = game.baseValue;
                    rec.isEndless = game.isEndless;
                    rec.score = game.score;
                    rec.maxTile = game.maxTile;
                    rec.timeLimit = game.timeLimit;

                    SaveScoreToLeaderboard(rec, LEADERBOARD_FILE);
                    leaderboardCount = LoadLeaderboard(leaderboard, MAX_LEADERBOARD_ENTRIES, LEADERBOARD_FILE);
                    scoreSaved = true;
                }
                currentScreen = SCREEN_GAMEOVER;
            }
        }

        // --------------------------------------------------------------------
        // 2. RENDER CURRENT SCREEN
        // --------------------------------------------------------------------
        BeginDrawing();
        ClearBackground((Color){250, 248, 239, 255}); // Classic 2048 warm cream background

        switch (currentScreen) {
            case SCREEN_MENU:
                DrawMenuScreen(&currentScreen, HasSavedGame(SAVE_FILE), &game);
                break;

            case SCREEN_CONFIG:
                DrawConfigScreen(&currentScreen, &config, &game);
                break;

            case SCREEN_GAMEPLAY:
                DrawGameplayScreen(&currentScreen, &game);
                break;

            case SCREEN_PAUSED:
                DrawGameplayScreen(&currentScreen, &game); // Draw frozen board underneath
                DrawPauseOverlay(&currentScreen, &game);
                break;

            case SCREEN_GAMEOVER:
                DrawGameOverScreen(&currentScreen, &game, &scoreSaved);
                break;

            case SCREEN_LEADERBOARD:
                DrawLeaderboardScreen(&currentScreen, leaderboard, leaderboardCount);
                break;
        }

        EndDrawing();
    }

    // Clean up heap memory and close Raylib window cleanly
    FreeGame(&game);
    CloseWindow();
    return 0;
}