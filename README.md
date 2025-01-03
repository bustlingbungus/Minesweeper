# Minesweeper

This is an implementation of the classic minesweeper game, in both Python and C++.

Simply contains a 2d array of `Cell` objects. The C++ version has each cell contain pointers to the cells adjacent to itself, whereas the Python version has cells access the array directly to lookup adjacent cells.

The C++ breaks the board by choosing the cell with zero adjacent bombs closest to the board's centre, whereas the Python version chooses the zero cell closest to the bottom right of the board. This change was made intentioinally, and in general, the Python version was made the be slightly lower quality than the C++ version, for the sake of having slightly simpler code. This was done because the Python version was made with the potential of being used as a workshop of [USF GameDev Club](https://www.instagram.com/usf_gamedev/), which ought to have as simple code as possible.

In theory, it should not be difficult to modify the C++ code to have more unique grid configurations (such as having a hexagonal grid of cells, like in Tametsi), because each Cell's vector of pointers could be of any length and the code would work.

The C++ version Makes use of SDL, and is built off of an SDL2 boilerplate, found [here](https://github.com/bustlingbungus/SDL2_Boilerplate)

The Python version runs using the `pygame` API.

## C++ Build Instructions

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

## Python Build instructions

Being python this should be very simple

### Dependencies

- Python compiler
- pygame

With pygame and all the included files installed, simply compile and run main.py

If you do not have `pygame` (which cam be checked with `pip show pygame`), run this command:

``` bash
pip install pygame
```
