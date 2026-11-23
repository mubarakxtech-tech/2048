#include "raylib.h"
#include "game_types.h"
#include "game_logic.h"
#include "file_manager.h"
#include "ui_renderer.h"
#include <stdio.h>
#include <string.h>

// Color palette matching classic 2048 styling
Color GetTileColor(int value) {
    switch (value) {
        case 2:    return (Color){238, 228, 218, 255};
        case 4:    return (Color){237, 224, 200, 255};
        case 8:    return (Color){242, 177, 121, 255};
        case 16:   return (Color){245, 149, 99, 255};
        case 32:   return (Color){246, 124, 95, 255};
        case 64:   return (Color){246, 94, 59, 255};
        case 128:  return (Color){237, 207, 114, 255};
        case 256:  return (Color){237, 204, 97, 255};
        case 512:  return (Color){237, 200, 80, 255};
        case 1024: return (Color){237, 197, 63, 255};
        case 2048: return (Color){237, 194, 46, 255};
        default:   return (value > 2048) ? (Color){60, 58, 50, 255} : (Color){205, 193, 180, 255};
    }
}

// 1. MAIN MENU SCREEN
void DrawMenuScreen(GameScreen *currentScreen, bool hasSavedGame, GameState *game) {
    int sw = GetScreenWidth();

    DrawText("DYNAMIC 2048", sw/2 - MeasureText("DYNAMIC 2048", 60)/2, 200, 60, (Color){119, 110, 101, 255});
    DrawText("Programming Fundamentals - C + Raylib", sw/2 - MeasureText("Programming Fundamentals - C + Raylib", 20)/2, 270, 20, (Color){143, 131, 118, 255});

    Rectangle btnNew      = {sw/2 - 150, 350, 300, 50};
    Rectangle btnResume   = {sw/2 - 150, 420, 300, 50};
    Rectangle btnLeader   = {sw/2 - 150, 490, 300, 50};
    Rectangle btnSettings = {sw/2 - 150, 560, 300, 50};
    Rectangle btnExit     = {sw/2 - 150, 630, 300, 50};

    Vector2 mouse = GetMousePosition();

    // New Game
    DrawRectangleRec(btnNew, CheckCollisionPointRec(mouse, btnNew) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnNew, 2, (Color){119, 110, 101, 255});
    DrawText("New Game", (int)(btnNew.x + 150 - MeasureText("New Game", 24)/2), (int)(btnNew.y + 13), 24, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnNew) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        GameConfig defaultConfig = {"Player", 4, 2, 2, 0.0f, false};
        LoadDefaultConfig(&defaultConfig, "data/settings.txt");
        InitGame(game, defaultConfig);
        *currentScreen = SCREEN_GAMEPLAY;
    }

    // Resume Game
    Color resumeColor = hasSavedGame ? ((CheckCollisionPointRec(mouse, btnResume)) ? LIGHTGRAY : RAYWHITE) : (Color){230, 230, 230, 255};
    DrawRectangleRec(btnResume, resumeColor);
    DrawRectangleLinesEx(btnResume, 2, hasSavedGame ? (Color){119, 110, 101, 255} : GRAY);
    DrawText("Resume Game", (int)(btnResume.x + 150 - MeasureText("Resume Game", 24)/2), (int)(btnResume.y + 13), 24, hasSavedGame ? (Color){119, 110, 101, 255} : GRAY);
    if (hasSavedGame && CheckCollisionPointRec(mouse, btnResume) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (LoadGameBinary(game, "data/savegame.dat")) {
            *currentScreen = SCREEN_GAMEPLAY;
        }
    }

    // Leaderboard
    DrawRectangleRec(btnLeader, CheckCollisionPointRec(mouse, btnLeader) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnLeader, 2, (Color){119, 110, 101, 255});
    DrawText("Leaderboard", (int)(btnLeader.x + 150 - MeasureText("Leaderboard", 24)/2), (int)(btnLeader.y + 13), 24, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnLeader) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_LEADERBOARD;
    }

    // Settings
    DrawRectangleRec(btnSettings, CheckCollisionPointRec(mouse, btnSettings) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnSettings, 2, (Color){119, 110, 101, 255});
    DrawText("Settings", (int)(btnSettings.x + 150 - MeasureText("Settings", 24)/2), (int)(btnSettings.y + 13), 24, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnSettings) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_CONFIG;
    }

    // Exit
    DrawRectangleRec(btnExit, CheckCollisionPointRec(mouse, btnExit) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnExit, 2, (Color){119, 110, 101, 255});
    DrawText("Exit", (int)(btnExit.x + 150 - MeasureText("Exit", 24)/2), (int)(btnExit.y + 13), 24, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnExit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        CloseWindow();
    }
}

// 2. CONFIG / SETTINGS SCREEN
void DrawConfigScreen(GameScreen *currentScreen, GameConfig *config, GameState *game) {
    int sw = GetScreenWidth();

    DrawText("SETTINGS", sw/2 - MeasureText("SETTINGS", 50)/2, 150, 50, (Color){119, 110, 101, 255});

    char gridStr[64];
    sprintf(gridStr, "Grid Size: %dx%d (Press UP/DOWN to adjust)", config->gridSize, config->gridSize);
    DrawText(gridStr, sw/2 - MeasureText(gridStr, 24)/2, 300, 24, (Color){119, 110, 101, 255});

    if (IsKeyPressed(KEY_UP) && config->gridSize < 8) config->gridSize++;
    if (IsKeyPressed(KEY_DOWN) && config->gridSize > 3) config->gridSize--;

    Rectangle btnSave = {sw/2 - 100, 500, 200, 50};
    Vector2 mouse = GetMousePosition();

    DrawRectangleRec(btnSave, CheckCollisionPointRec(mouse, btnSave) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnSave, 2, (Color){119, 110, 101, 255});
    DrawText("Save & Back", (int)(btnSave.x + 100 - MeasureText("Save & Back", 20)/2), (int)(btnSave.y + 15), 20, (Color){119, 110, 101, 255});

    if (CheckCollisionPointRec(mouse, btnSave) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SaveDefaultConfig(config, "data/settings.txt");
        *currentScreen = SCREEN_MENU;
    }
}

// 3. GAMEPLAY SCREEN
void DrawGameplayScreen(GameScreen *currentScreen, GameState *game) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Header Title & Scoreboard
    DrawText("2048", sw/2 - 350, 60, 70, (Color){119, 110, 101, 255});

    char scoreText[32], maxTileText[32];
    sprintf(scoreText, "SCORE: %d", game->score);
    sprintf(maxTileText, "BEST: %d", game->maxTile);

    DrawRectangle(sw/2 + 100, 60, 150, 60, (Color){187, 173, 160, 255});
    DrawText(scoreText, sw/2 + 110, 80, 20, RAYWHITE);

    DrawRectangle(sw/2 + 270, 60, 150, 60, (Color){187, 173, 160, 255});
    DrawText(maxTileText, sw/2 + 280, 80, 20, RAYWHITE);

    // Calculate dynamic centering
    int n = game->gridSize;
    int tileSize = (n <= 4) ? 110 : (450 / n);
    int gap = 12;
    int boardWidth = n * tileSize + (n + 1) * gap;

    int startX = sw/2 - boardWidth/2;
    int startY = sh/2 - boardWidth/2 + 20;

    // Background Board Container
    DrawRectangle(startX, startY, boardWidth, boardWidth, (Color){187, 173, 160, 255});

    // Render Tiles
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            int val = GetTileAt(game, r, c);
            int tileX = startX + gap + c * (tileSize + gap);
            int tileY = startY + gap + r * (tileSize + gap);

            DrawRectangle(tileX, tileY, tileSize, tileSize, GetTileColor(val));

            if (val > 0) {
                char valStr[16];
                sprintf(valStr, "%d", val);
                int fontSize = (tileSize > 80) ? 36 : 24;
                int textW = MeasureText(valStr, fontSize);
                Color fontColor = (val <= 4) ? (Color){119, 110, 101, 255} : RAYWHITE;
                DrawText(valStr, tileX + tileSize/2 - textW/2, tileY + tileSize/2 - fontSize/2, fontSize, fontColor);
            }
        }
    }

    // Instructions Footer
    DrawText("Controls: [W/A/S/D or Arrows] Move  |  [Z] Undo  |  [P / ESC] Pause", sw/2 - MeasureText("Controls: [W/A/S/D or Arrows] Move  |  [Z] Undo  |  [P / ESC] Pause", 20)/2, sh - 60, 20, (Color){119, 110, 101, 255});
}

// 4. PAUSE OVERLAY
void DrawPauseOverlay(GameScreen *currentScreen, GameState *game) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 150});

    DrawText("GAME PAUSED", sw/2 - MeasureText("GAME PAUSED", 50)/2, sh/2 - 180, 50, RAYWHITE);

    Rectangle btnResume = {sw/2 - 120, sh/2 - 60, 240, 50};
    Rectangle btnSave   = {sw/2 - 120, sh/2 + 10, 240, 50};
    Rectangle btnMenu   = {sw/2 - 120, sh/2 + 80, 240, 50};
    Vector2 mouse = GetMousePosition();

    // Resume
    DrawRectangleRec(btnResume, CheckCollisionPointRec(mouse, btnResume) ? LIGHTGRAY : RAYWHITE);
    DrawText("Resume", (int)(btnResume.x + 120 - MeasureText("Resume", 22)/2), (int)(btnResume.y + 14), 22, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnResume) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_GAMEPLAY;
    }

    // Save Game
    DrawRectangleRec(btnSave, CheckCollisionPointRec(mouse, btnSave) ? LIGHTGRAY : RAYWHITE);
    DrawText("Save & Exit", (int)(btnSave.x + 120 - MeasureText("Save & Exit", 22)/2), (int)(btnSave.y + 14), 22, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnSave) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SaveGameBinary(game, "data/savegame.dat");
        *currentScreen = SCREEN_MENU;
    }

    // Return to Menu
    DrawRectangleRec(btnMenu, CheckCollisionPointRec(mouse, btnMenu) ? LIGHTGRAY : RAYWHITE);
    DrawText("Main Menu", (int)(btnMenu.x + 120 - MeasureText("Main Menu", 22)/2), (int)(btnMenu.y + 14), 22, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnMenu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}

// 5. GAME OVER SCREEN
void DrawGameOverScreen(GameScreen *currentScreen, GameState *game, bool *scoreSaved) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(0, 0, sw, sh, (Color){238, 228, 218, 220});

    DrawText("GAME OVER!", sw/2 - MeasureText("GAME OVER!", 60)/2, sh/2 - 150, 60, (Color){119, 110, 101, 255});

    char finalScoreStr[64];
    sprintf(finalScoreStr, "Final Score: %d  |  Highest Tile: %d", game->score, game->maxTile);
    DrawText(finalScoreStr, sw/2 - MeasureText(finalScoreStr, 24)/2, sh/2 - 50, 24, (Color){119, 110, 101, 255});

    Rectangle btnMenu = {sw/2 - 120, sh/2 + 40, 240, 50};
    Vector2 mouse = GetMousePosition();

    DrawRectangleRec(btnMenu, CheckCollisionPointRec(mouse, btnMenu) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnMenu, 2, (Color){119, 110, 101, 255});
    DrawText("Main Menu", (int)(btnMenu.x + 120 - MeasureText("Main Menu", 22)/2), (int)(btnMenu.y + 14), 22, (Color){119, 110, 101, 255});

    if (CheckCollisionPointRec(mouse, btnMenu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}

// 6. LEADERBOARD SCREEN - Explicit Column X Positions
void DrawLeaderboardScreen(GameScreen *currentScreen, ScoreRecord leaderboard[], int recordCount) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawText("TOP HIGH SCORES", sw/2 - MeasureText("TOP HIGH SCORES", 50)/2, 100, 50, (Color){119, 110, 101, 255});

    int startY = 220;
    int colRank  = sw/2 - 450;
    int colName  = sw/2 - 320;
    int colGrid  = sw/2 + 50;
    int colScore = sw/2 + 200;
    int colMax   = sw/2 + 370;

    // Headers
    DrawText("Rank", colRank, startY, 22, (Color){143, 131, 118, 255});
    DrawText("Player Name", colName, startY, 22, (Color){143, 131, 118, 255});
    DrawText("Grid", colGrid, startY, 22, (Color){143, 131, 118, 255});
    DrawText("Score", colScore, startY, 22, (Color){143, 131, 118, 255});
    DrawText("Max Tile", colMax, startY, 22, (Color){143, 131, 118, 255});

    DrawLine(colRank, startY + 30, colMax + 100, startY + 30, (Color){187, 173, 160, 255});

    // Rows
    for (int i = 0; i < recordCount && i < 10; i++) {
        int rowY = startY + 50 + (i * 38);
        char rankStr[16], gridStr[16], scoreStr[32], maxTileStr[32];

        sprintf(rankStr, "#%d", i + 1);
        sprintf(gridStr, "%dx%d", leaderboard[i].gridSize, leaderboard[i].gridSize);
        sprintf(scoreStr, "%d", leaderboard[i].score);
        sprintf(maxTileStr, "%d", leaderboard[i].maxTile);

        DrawText(rankStr, colRank, rowY, 20, (Color){119, 110, 101, 255});
        DrawText(leaderboard[i].playerName, colName, rowY, 20, (Color){119, 110, 101, 255});
        DrawText(gridStr, colGrid, rowY, 20, (Color){119, 110, 101, 255});
        DrawText(scoreStr, colScore, rowY, 20, (Color){119, 110, 101, 255});
        DrawText(maxTileStr, colMax, rowY, 20, (Color){119, 110, 101, 255});
    }

    Rectangle btnBack = {sw/2 - 100, sh - 120, 200, 50};
    Vector2 mouse = GetMousePosition();

    DrawRectangleRec(btnBack, CheckCollisionPointRec(mouse, btnBack) ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(btnBack, 2, (Color){119, 110, 101, 255});
    DrawText("Back to Menu", (int)(btnBack.x + 100 - MeasureText("Back to Menu", 20)/2), (int)(btnBack.y + 15), 20, (Color){119, 110, 101, 255});

    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}
