#include "ui_renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Color palette for 2048 tiles
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
        default:   return (Color){60, 58, 50, 255}; // > 2048 tiles
    }
}

// ----------------------------------------------------------------------------
// 1. MAIN MENU SCREEN
// ----------------------------------------------------------------------------
void DrawMenuScreen(GameScreen *currentScreen, bool hasSaveGame, GameState *game) {
    Vector2 mouse = GetMousePosition();

    // Title
    DrawText("DYNAMIC 2048", 710, 200, 70, (Color){119, 110, 101, 255});
    DrawText("Programming Fundamentals - FAST NUCES", 710, 280, 24, (Color){143, 131, 117, 255});

    // Button Dimensions
    Rectangle btnNew   = { 760, 360, 400, 60 };
    Rectangle btnResume= { 760, 440, 400, 60 };
    Rectangle btnConfig= { 760, 520, 400, 60 };
    Rectangle btnLeader= { 760, 600, 400, 60 };
    Rectangle btnExit  = { 760, 680, 400, 60 };

    // New Game Button
    bool hoverNew = CheckCollisionPointRec(mouse, btnNew);
    DrawRectangleRec(btnNew, hoverNew ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("NEW GAME", 880, 375, 28, (Color){119, 110, 101, 255});
    if (hoverNew && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        GameConfig defaultConfig = {"Player", 4, 2, 2, 0.0f, false};
        LoadDefaultConfig(&defaultConfig, "data/settings.txt");
        InitGame(game, defaultConfig);
        *currentScreen = SCREEN_GAMEPLAY;
    }

    // Resume Button (Enabled only if save game exists)
    if (hasSaveGame) {
        bool hoverResume = CheckCollisionPointRec(mouse, btnResume);
        DrawRectangleRec(btnResume, hoverResume ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
        DrawText("RESUME GAME", 860, 455, 28, (Color){119, 110, 101, 255});
        if (hoverResume && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (LoadGameBinary(game, "data/savegame.dat")) {
                *currentScreen = SCREEN_GAMEPLAY;
            }
        }
    } else {
        DrawRectangleRec(btnResume, (Color){200, 195, 185, 255});
        DrawText("RESUME GAME (No Save)", 800, 455, 24, (Color){150, 145, 135, 255});
    }

    // Settings Button
    bool hoverConfig = CheckCollisionPointRec(mouse, btnConfig);
    DrawRectangleRec(btnConfig, hoverConfig ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("SETTINGS", 890, 535, 28, (Color){119, 110, 101, 255});
    if (hoverConfig && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_CONFIG;
    }

    // Leaderboard Button
    bool hoverLeader = CheckCollisionPointRec(mouse, btnLeader);
    DrawRectangleRec(btnLeader, hoverLeader ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("LEADERBOARD", 855, 615, 28, (Color){119, 110, 101, 255});
    if (hoverLeader && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_LEADERBOARD;
    }

    // Exit Button
    bool hoverExit = CheckCollisionPointRec(mouse, btnExit);
    DrawRectangleRec(btnExit, hoverExit ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("EXIT", 920, 695, 28, (Color){119, 110, 101, 255});
    if (hoverExit && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        CloseWindow();
    }
}

// ----------------------------------------------------------------------------
// 2. CONFIGURATION / SETTINGS SCREEN
// ----------------------------------------------------------------------------
void DrawConfigScreen(GameScreen *currentScreen, GameConfig *config, GameState *game) {
    Vector2 mouse = GetMousePosition();

    DrawText("SETTINGS", 830, 150, 60, (Color){119, 110, 101, 255});

    // Grid Size Options (3x3 to 8x8)
    DrawText("Grid Size:", 650, 280, 28, (Color){119, 110, 101, 255});
    for (int size = 3; size <= 8; size++) {
        Rectangle sizeBtn = { 800 + (size - 3) * 80, 270, 60, 50 };
        bool selected = (config->gridSize == size);
        bool hover = CheckCollisionPointRec(mouse, sizeBtn);

        Color btnCol = selected ? (Color){242, 177, 121, 255} : (hover ? (Color){238, 228, 218, 255} : (Color){205, 193, 180, 255});
        DrawRectangleRec(sizeBtn, btnCol);

        char label[8];
        snprintf(label, sizeof(label), "%dx%d", size, size);
        DrawText(label, sizeBtn.x + 10, sizeBtn.y + 12, 22, (Color){119, 110, 101, 255});

        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            config->gridSize = size;
        }
    }

    // Mode Option (Standard vs Endless)
    DrawText("Game Mode:", 650, 380, 28, (Color){119, 110, 101, 255});
    Rectangle btnStd = { 800, 370, 180, 50 };
    Rectangle btnEnd = { 1000, 370, 180, 50 };

    bool selStd = !config->isEndless;
    bool selEnd = config->isEndless;

    DrawRectangleRec(btnStd, selStd ? (Color){242, 177, 121, 255} : (Color){205, 193, 180, 255});
    DrawText("Target (2048)", btnStd.x + 20, btnStd.y + 12, 20, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnStd) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        config->isEndless = false;
    }

    DrawRectangleRec(btnEnd, selEnd ? (Color){242, 177, 121, 255} : (Color){205, 193, 180, 255});
    DrawText("Endless Mode", btnEnd.x + 20, btnEnd.y + 12, 20, (Color){119, 110, 101, 255});
    if (CheckCollisionPointRec(mouse, btnEnd) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        config->isEndless = true;
    }

    // Save & Back Buttons
    Rectangle btnSave = { 760, 520, 400, 60 };
    Rectangle btnBack = { 760, 600, 400, 60 };

    bool hoverSave = CheckCollisionPointRec(mouse, btnSave);
    DrawRectangleRec(btnSave, hoverSave ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("SAVE & START", 855, 535, 28, (Color){119, 110, 101, 255});
    if (hoverSave && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SaveDefaultConfig(config, "data/settings.txt");
        InitGame(game, *config);
        *currentScreen = SCREEN_GAMEPLAY;
    }

    bool hoverBack = CheckCollisionPointRec(mouse, btnBack);
    DrawRectangleRec(btnBack, hoverBack ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("BACK TO MENU", 850, 615, 28, (Color){119, 110, 101, 255});
    if (hoverBack && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}

// ----------------------------------------------------------------------------
// 3. ACTIVE GAMEPLAY SCREEN (1920x1080 Centered Grid)
// ----------------------------------------------------------------------------
void DrawGameplayScreen(GameScreen *currentScreen, GameState *game) {
    if (game == NULL || game->grid == NULL) return;

    // Header Info
    DrawText("DYNAMIC 2048", 100, 50, 50, (Color){119, 110, 101, 255});

    char scoreStr[64];
    snprintf(scoreStr, sizeof(scoreStr), "SCORE: %d", game->score);
    DrawRectangle(1350, 40, 200, 70, (Color){187, 173, 160, 255});
    DrawText(scoreStr, 1370, 60, 26, WHITE);

    char maxTileStr[64];
    snprintf(maxTileStr, sizeof(maxTileStr), "BEST: %d", game->maxTile);
    DrawRectangle(1580, 40, 200, 70, (Color){187, 173, 160, 255});
    DrawText(maxTileStr, 1600, 60, 26, WHITE);

    DrawText("Controls: [W/A/S/D] Move | [Z] Undo | [P/ESC] Pause", 100, 120, 22, (Color){143, 131, 117, 255});

    // Grid Dimensions Calculation
    int n = game->gridSize;
    int tileSize = (n <= 4) ? 100 : ((n <= 6) ? 80 : 65);
    int gap = 12;

    int totalBoardSize = n * tileSize + (n + 1) * gap;
    int startX = (1920 - totalBoardSize) / 2;
    int startY = (1080 - totalBoardSize) / 2 + 30;

    // Outer Board Container
    DrawRectangle(startX, startY, totalBoardSize, totalBoardSize, (Color){187, 173, 160, 255});

    // Render Board Grid & Tiles
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            int tileX = startX + gap + c * (tileSize + gap);
            int tileY = startY + gap + r * (tileSize + gap);

            int val = GetTileAt(game, r, c);

            // Empty slot background
            DrawRectangle(tileX, tileY, tileSize, tileSize, (Color){205, 193, 180, 255});

            if (val > 0) {
                DrawRectangle(tileX, tileY, tileSize, tileSize, GetTileColor(val));

                char valStr[16];
                snprintf(valStr, sizeof(valStr), "%d", val);
                int fontSize = (val < 100) ? 36 : ((val < 1000) ? 28 : 22);
                Color textCol = (val <= 4) ? (Color){119, 110, 101, 255} : WHITE;

                int textWidth = MeasureText(valStr, fontSize);
                DrawText(valStr, tileX + (tileSize - textWidth) / 2, tileY + (tileSize - fontSize) / 2, fontSize, textCol);
            }
        }
    }
}

// ----------------------------------------------------------------------------
// 4. PAUSE OVERLAY
// ----------------------------------------------------------------------------
void DrawPauseOverlay(GameScreen *currentScreen, GameState *game) {
    Vector2 mouse = GetMousePosition();

    // Dark semi-transparent background overlay
    DrawRectangle(0, 0, 1920, 1080, (Color){0, 0, 0, 150});

    DrawText("GAME PAUSED", 780, 320, 60, WHITE);

    Rectangle btnResume = { 760, 440, 400, 60 };
    Rectangle btnSave   = { 760, 520, 400, 60 };
    Rectangle btnQuit   = { 760, 600, 400, 60 };

    bool hResume = CheckCollisionPointRec(mouse, btnResume);
    DrawRectangleRec(btnResume, hResume ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("RESUME", 900, 455, 28, (Color){119, 110, 101, 255});
    if (hResume && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_GAMEPLAY;
    }

    bool hSave = CheckCollisionPointRec(mouse, btnSave);
    DrawRectangleRec(btnSave, hSave ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("SAVE & EXIT", 870, 535, 28, (Color){119, 110, 101, 255});
    if (hSave && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SaveGameBinary(game, "data/savegame.dat");
        *currentScreen = SCREEN_MENU;
    }

    bool hQuit = CheckCollisionPointRec(mouse, btnQuit);
    DrawRectangleRec(btnQuit, hQuit ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("QUIT TO MENU", 860, 615, 28, (Color){119, 110, 101, 255});
    if (hQuit && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}

// ----------------------------------------------------------------------------
// 5. GAME OVER / VICTORY SCREEN
// ----------------------------------------------------------------------------
void DrawGameOverScreen(GameScreen *currentScreen, GameState *game, bool *scoreSaved) {
    Vector2 mouse = GetMousePosition();

    DrawRectangle(0, 0, 1920, 1080, (Color){250, 248, 239, 230});

    if (game->targetReached) {
        DrawText("YOU WIN! 2048 REACHED!", 600, 300, 55, (Color){237, 194, 46, 255});
    } else {
        DrawText("GAME OVER!", 780, 300, 60, (Color){119, 110, 101, 255});
    }

    char finalScoreStr[64];
    snprintf(finalScoreStr, sizeof(finalScoreStr), "Final Score: %d", game->score);
    DrawText(finalScoreStr, 830, 400, 32, (Color){143, 131, 117, 255});

    Rectangle btnMenu = { 760, 520, 400, 60 };
    bool hMenu = CheckCollisionPointRec(mouse, btnMenu);
    DrawRectangleRec(btnMenu, hMenu ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("MAIN MENU", 880, 535, 28, (Color){119, 110, 101, 255});

    if (hMenu && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}

// ----------------------------------------------------------------------------
// 6. LEADERBOARD SCREEN
// ----------------------------------------------------------------------------
void DrawLeaderboardScreen(GameScreen *currentScreen, const ScoreRecord leaderboard[], int recordCount) {
    Vector2 mouse = GetMousePosition();

    DrawText("TOP HIGH SCORES", 720, 120, 50, (Color){119, 110, 101, 255});

    // Table Headers
    DrawRectangle(560, 220, 800, 40, (Color){187, 173, 160, 255});
    DrawText("Rank", 580, 230, 20, WHITE);
    DrawText("Player", 680, 230, 20, WHITE);
    DrawText("Grid", 880, 230, 20, WHITE);
    DrawText("Max Tile", 1000, 230, 20, WHITE);
    DrawText("Score", 1200, 230, 20, WHITE);

    int maxDisplay = (recordCount > 10) ? 10 : recordCount;
    for (int i = 0; i < maxDisplay; i++) {
        int y = 270 + i * 42;
        Color rowBg = (i % 2 == 0) ? (Color){238, 228, 218, 255} : (Color){250, 248, 239, 255};
        DrawRectangle(560, y, 800, 38, rowBg);

        char rankStr[8], gridStr[16], tileStr[16], scoreStr[32];
        snprintf(rankStr, sizeof(rankStr), "#%d", i + 1);
        snprintf(gridStr, sizeof(gridStr), "%dx%d", leaderboard[i].gridSize, leaderboard[i].gridSize);
        snprintf(tileStr, sizeof(tileStr), "%d", leaderboard[i].maxTile);
        snprintf(scoreStr, sizeof(scoreStr), "%d", leaderboard[i].score);

        DrawText(rankStr, 580, y + 8, 20, (Color){119, 110, 101, 255});
        DrawText(leaderboard[i].playerName, 680, y + 8, 20, (Color){119, 110, 101, 255});
        DrawText(gridStr, 880, y + 8, 20, (Color){119, 110, 101, 255});
        DrawText(tileStr, 1000, y + 8, 20, (Color){119, 110, 101, 255});
        DrawText(scoreStr, 1200, y + 8, 20, (Color){119, 110, 101, 255});
    }

    Rectangle btnBack = { 760, 720, 400, 60 };
    bool hBack = CheckCollisionPointRec(mouse, btnBack);
    DrawRectangleRec(btnBack, hBack ? (Color){242, 177, 121, 255} : (Color){238, 228, 218, 255});
    DrawText("BACK TO MENU", 850, 735, 28, (Color){119, 110, 101, 255});

    if (hBack && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScreen = SCREEN_MENU;
    }
}
