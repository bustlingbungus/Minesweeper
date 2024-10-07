## Minesweeper

This is a C++ implementation of the classic minesweeper game.

Very simple program, functions by creating a 2d vectors to Cell objects, each cell contains a vector of pointers to their adjacent cells.

In theory, it should not be difficult to modify this to have more unique grid configurations (such as having a hexagonal grid of cells, like in Tametsi),
because each Cell's vector of pointers could be of any length and the code would work.

Makes use of SDL, and is build off of the SDL2 boilerplate, found [here](https://github.com/bustlingbungus/SDL_Boilerplate)

## Build Instructions

### Dependencies
- C++ compiler
- CMake
- Installation of ```SDL2```, ```SDL2_Image```, ```SDL2_ttf```, and ```SDL2_mixer```

### Instructions

Ensure there is a 'build' folder in your project directory. Change to this directory with ```cd build```.
From the build folder, run these commands:
```
cmake ..
cmake --build . --verbose
```

Then, run ```cd bin``` to go to the output directory, and run ```./Minesweeper.exe``` to launch the game.

On VSCode, pressing ```ctrl+shift+b``` to run the included default build tasks should also work.