@echo off
echo ===========================================
echo   Building Dynamic 2048 (MSVC cl.exe - x64)
echo ===========================================

:: Check if cl compiler is available
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] MSVC compiler 'cl' was not found in your environment PATH!
    echo Please run this script from 'x64 Native Tools Command Prompt for VS 2022'.
    echo.
    pause
    exit /b 1
)

:: Compile all source files (main.c, gamelogic.c, filemanager.c, ui_renderer.c)
cl /nologo /W3 /Zi /EHsc /MD /Iinclude main.c src/gamelogic.c src/filemanager.c src/ui_renderer.c /Fe:2048.exe /link /machine:x64 raylib.lib user32.lib gdi32.lib winmm.lib shell32.lib

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
