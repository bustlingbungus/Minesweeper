#ifndef CELL_HPP
#define CELL_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std 
#include <memory>
#include <vector>

/* returns a colour based on the number of surrounding bombs */
SDL_Color getColour(const int& numSurroundingBombs);

/* A cell in the minesweeper board */
class Cell
{
    public:
        Cell(bool isBomb = false, std::vector<std::shared_ptr<Cell>*> surrounding = {}, 
            std::shared_ptr<LWindow> window = nullptr, std::shared_ptr<LTexture> bkg_revealed = nullptr, 
            std::shared_ptr<LTexture> bkg_hidden = nullptr, std::shared_ptr<LTexture> flag_tex = nullptr, 
            std::shared_ptr<LTexture> bomb_tex = nullptr
        );
        /* Deallocates resources */
        ~Cell();

        /* renders the cell into the specified rect */
        void render(SDL_Rect *dest) const;

        /* true if the cell contains a bomb */
        bool isBomb() const;
        /* true if the cell has been revealed */
        bool isRevealed() const;
        /* true if the cell has a flag */
        bool hasFlag() const;
        /* returns the number of surrounding cells */
        int numSurrounding() const;

        /* Reveals the cell. If there are no surrounding bombs, reveals surrounding cells.
         * Returns true if the cell is a bomb */
        bool reveal();

        /* Toggles the cell's flag. Returns true if the cell is a bomb */
        bool toggleFlag();

        /* counts and saves the number of surrounding bombs */
        void countSurrounding();

    private:

        /* the number of surrounding bombs, -1 if the cell is a bomb */
        int num_bombs = 0;
        /* whether or not the cell has been revealed */
        bool is_revealed = false;
        /* whether or not the cell has a flag */
        bool has_flag = false;

        /* pointers to the surrounding cells */
        std::vector<std::shared_ptr<Cell>*> surrounding;

        /* the window, and the cell backgrounds, a flag, bomb, and the number of bombs, as a texture */
        std::shared_ptr<LWindow> window;
        std::shared_ptr<LTexture> bkg0, bkg1, flag, bomb;
        std::unique_ptr<LTexture> num;
};

#endif