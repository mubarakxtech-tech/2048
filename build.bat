@echo off
gcc src/*.c -Iinclude -Wall -Wextra -lraylib -lopengl32 -lgdi32 -lwinmm -o 2048_game.exe
if %errorlevel% == 0 (
    echo [SUCCESS] Launching 2048_game.exe...
    2048_game.exe
) else (
    echo [ERROR] Compilation failed. Fix errors above.
)