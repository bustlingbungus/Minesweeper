#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

// sdl
#include <SDL.h>

#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"
#include "../sdl/LAudio.hpp"

// std
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <random>

// minesweeper
#include "Cell.hpp"

/* Minesweeper game */
class Minesweeper
{
    public:
        Minesweeper(std::shared_ptr<LWindow> window);
        /* Deallocates resources */
        ~Minesweeper();

        /* Reads left/right mouse input */
        void getInput(SDL_Event& e);

        /* renders the grid, and any other text */
        void render() const;

        /* Creates the grid containing all cells */
        void createGrid(const int& width, const int& height);

    private:

        /* Input functions */
        // left clicks (x,y)
        void leftClick(const int& x, const int& y);
        // right clicks (x,y)
        void rightClick(const int& x, const int& y);


        /* ==========  CONSTANT VARIABLES  ========== */

        // the empty space surrounding the grid (in pixels)
        int MARGIN_SIZE = 20;
        // how thick each gridline is (in pixels)
        int GRIDLINE_WIDTH = 3;
        // the chance each cell has to be a bomb
        double BOMB_CHANCE = 0.25;


        /* ==========  GAME VARIABLES  ========== */

        // the number of bombs remaining
        int numBombs = 0;
        // whether or not the game has ended
        bool game_over = false;
        // the dimensions of the grid
        int WIDTH, HEIGHT;
        // the side length of each cell
        int side_len;



        /* the window the game is running in */
        std::shared_ptr<LWindow> window;

        /* the dimensions of the cell array */
        int nx, ny;
        /* a 2d vector of cell pointers */
        std::vector<std::vector<std::shared_ptr<Cell>>> cells;


        /* TEXTURES */
        std::shared_ptr<LTexture> cell_bkg0 = nullptr, cell_bkg1 = nullptr, line_col = nullptr,
            flag = nullptr, bomb = nullptr, win = nullptr, lose = nullptr;

        /* creates all textures */
        void loadTextures(const std::string& assets_dir);



        /* ==========  HELPER FUNCTIONS  ========== */

        /* Renders all cells */
        void renderCells(const int& w, const int& h, const int& side_len) const;
        /* Renders grid lines */
        void renderGridlines(const int& w, const int& h, const int& side_len) const;
        
        /* Reveals all hidden cells */
        void revealAll();

        /* Reveals a cell with zero adjacent bombs closest to the centre of the grid.
           returns false if there are no cells with zero bombs */
        bool breakBoard();
};

#endif