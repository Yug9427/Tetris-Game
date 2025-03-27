# Tetris Game in C++

## Overview
This is a simple Tetris game implemented in C++ using console graphics. The game features different tetromino shapes, a scoring system, increasing difficulty levels, and colored pieces for better visualization. The game runs in the console and can be played using keyboard inputs.

## Features
- **7 Tetromino Shapes:** Includes I, O, T, S, Z, J, and L shapes.
- **Colored Blocks:** Each tetromino has a distinct color for better visibility.
- **Smooth Rendering:** Uses `ostringstream` to prevent flickering.
- **Scoring System:** Earn points for clearing lines.
- **Level Progression:** The game speeds up as the player scores more points.
- **Sound Effects:** Uses `Beep()` to provide auditory feedback.
- **Game Over Detection:** Ends the game when no valid moves are left.

## Controls
- **Left Arrow (`←`)** - Move piece left
- **Right Arrow (`→`)** - Move piece right
- **Down Arrow (`↓`)** - Move piece down
- **Up Arrow (`↑`)** - Rotate piece
- **Spacebar (`SPACE`)** - Hard drop (instantly places piece at the lowest possible position)

## Dependencies
The game uses the following headers:
- `<iostream>` - Standard input/output handling
- `<vector>` - Used to store grid and tetromino shapes
- `<conio.h>` - Handles keyboard input (`_kbhit()` and `_getch()`)
- `<windows.h>` - Enables system-related functions like `Sleep()` and `Beep()`
- `<ctime>` - Used for random tetromino generation
- `<sstream>` - Used to optimize rendering and avoid flickering

## Compilation & Execution
### Using g++ (MinGW) on Windows
```
g++ -o tetris tetris.cpp -static-libgcc -static-libstdc++
```
Run the executable:
```
tetris.exe
```

### Using MSVC (Visual Studio)
```
cl /EHsc tetris.cpp
```
Run the executable:
```
tetris.exe
```

## How the Game Works
1. A tetromino spawns at the top center of the grid.
2. The player moves the tetromino left, right, down, or rotates it.
3. If the tetromino reaches the bottom or lands on another piece, it locks in place.
4. If a row is completely filled, it disappears, and the player earns points.
5. The game speeds up as the score increases.
6. The game ends when a new piece cannot be placed.


### Enjoy playing Tetris!
