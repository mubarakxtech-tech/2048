# Dynamic 2048 Game (C + Raylib)

Programming Fundamentals Semester Project featuring customizable grid sizes, predefined tile increments, endless mode, countdown timer, undo support, and persistent file tracking.

## Features
- **Dynamic NxN Grid:** User-configurable board sizes (3x3 up to 8x8) allocated dynamically in C.
- **Predefined Increment & Endless Mode:** Custom starting tile values, merge multipliers, and toggleable "No Limit" play.
- **Countdown Timer:** Frame-independent time limit mode using Raylib's `GetFrameTime()`.
- **File Tracking System:**
  - `data/leaderboard.csv` — Top 10 high scores sorted and persisted across runs.
  - `data/savegame.dat` — Binary save/resume snapshot (`fwrite` / `fread`).
  - `data/settings.txt` — Saved user configuration defaults.

## Project Structure
├── include/          # Shared headers (game_types.h, game_logic.h, file_manager.h, ui_renderer.h)
├── src/              # C source files (main.c, game_logic.c, file_manager.c, ui_renderer.c)
├── data/             # Runtime CSV, binary save, and config files
├── build.bat         # Windows GCC + Raylib build script
└── README.md

## Build & Run (Windows)
```bat
build.bat OR build_msvc.bat
