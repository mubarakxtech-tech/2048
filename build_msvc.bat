@echo off
title Building Dynamic 2048 (MSVC cl.exe - x64)
cls

echo ===========================================
echo   Building Dynamic 2048 (MSVC cl.exe - x64)
echo ===========================================

:: 1. Auto-detect file locations (root vs src/)
set "MAIN_FILE=main.c"
if not exist "%MAIN_FILE%" (
    if exist "src\main.c" set "MAIN_FILE=src\main.c"
)

set "GAMELOGIC_FILE=gamelogic.c"
if not exist "%GAMELOGIC_FILE%" (
    if exist "src\gamelogic.c" (
        set "GAMELOGIC_FILE=src\gamelogic.c"
    ) else if exist "game_logic.c" (
        set "GAMELOGIC_FILE=game_logic.c"
    ) else if exist "src\game_logic.c" (
        set "GAMELOGIC_FILE=src\game_logic.c"
    )
)

set "FILEMANAGER_FILE=filemanager.c"
if not exist "%FILEMANAGER_FILE%" (
    if exist "src\filemanager.c" (
        set "FILEMANAGER_FILE=src\filemanager.c"
    ) else if exist "file_manager.c" (
        set "FILEMANAGER_FILE=file_manager.c"
    ) else if exist "src\file_manager.c" (
        set "FILEMANAGER_FILE=src\file_manager.c"
    )
)

set "UI_FILE=ui_renderer.c"
if not exist "%UI_FILE%" (
    if exist "src\ui_renderer.c" set "UI_FILE=src\ui_renderer.c"
)

echo Compiling resolved files:
echo   Main:       %MAIN_FILE%
echo   GameLogic:  %GAMELOGIC_FILE%
echo   FileManager:%FILEMANAGER_FILE%
echo   UI:         %UI_FILE%
echo.

:: 2. Compile with MSVC (x64, Dynamic CRT /MD, Suppress sprintf deprecation warnings)
cl /nologo /W3 /Zi /EHsc /MD /D_CRT_SECURE_NO_WARNINGS /I. /Iinclude %MAIN_FILE% %GAMELOGIC_FILE% %FILEMANAGER_FILE% %UI_FILE% /Fe:2048.exe /link /machine:x64 raylib.lib user32.lib gdi32.lib winmm.lib shell32.lib

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ====================================================================
    echo [SUCCESS] 2048.exe 64-bit compiled successfully!
    echo ====================================================================
) else (
    echo.
    echo ====================================================================
    echo [ERROR] MSVC Compilation failed. Check errors above.
    echo ====================================================================
)
