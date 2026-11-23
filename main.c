#include "raylib.h"
#include "game_types.h"

int main(void) {
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "2048");
    SetTargetFPS(60);

    int currentScreen = 0;


    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ONE))   currentScreen = 0; // Menu Screen
        if (IsKeyPressed(KEY_TWO))   currentScreen = 1; // Game Screen
        if (IsKeyPressed(KEY_THREE)) currentScreen = 2; // Settings Screen
        if (IsKeyPressed(KEY_FOUR))  currentScreen = 3; // Game Over Screen

        BeginDrawing();
            ClearBackground(RAYWHITE);

            switch (currentScreen) {
                case 0: // Menu Screen
                    DrawText("DYNAMIC 2048 - MAIN MENU", 400, 300, 81, DARKBLUE);
                    DrawText("Press [2] to Start Game", 700, 500, 36, DARKGRAY);
                    DrawText("Press [3] for Settings", 700, 560, 36, DARKGRAY);
                    DrawText("Press [ESC] to Exit", 700, 620, 36, RED);
                    break;

                case 1: // Game Screen (Dummy 4x4 Grid Placeholder for Dev 2 UI)
                    DrawText("GAMEPLAY SCREEN", 700, 50, 54, MAROON);
                    DrawText("Press [1] for Menu | [4] for Game Over", 600, 520, 38, DARKGRAY);
                    // UI grid rendering will be called here via DrawGameUI()
                    break;

                case 2: // Settings Screen
                    DrawText("SETTINGS SCREEN", 700, 150, 54, PURPLE);
                    DrawText("Grid Size Selection (3x3 - 8x8)", 700, 420, 36, DARKGRAY);
                    DrawText("Press [1] to Return to Menu", 700, 600, 36, DARKGRAY);
                    break;

                case 3: // Game Over Screen
                    DrawText("GAME OVER!", 750, 420, 54, RED);
                    DrawText("Press [1] to Return to Main Menu", 650, 600, 36, DARKGRAY);
                    break;

                default:
                    break;
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
