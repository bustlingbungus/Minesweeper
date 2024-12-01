#ifndef CELL_HPP
#define CELL_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std 
#include <memory>
#include <vector>

SDL_Color getColour(int numSurroundingBombs);

/* A cell in the minesweeper board */
class Cell
{
    public:
        Cell(bool isBomb = false, std::vector<std::shared_ptr<Cell>*> surrounding = {}, 
            std::shared_ptr<LWindow> window = nullptr, std::shared_ptr<LTexture> bkg_revealed = nullptr, 
            std::shared_ptr<LTexture> bkg_hidden = nullptr, std::shared_ptr<LTexture> flag_tex = nullptr, 
            std::shared_ptr<LTexture> bomb_tex = nullptr
        );
        ~Cell();

        void render(SDL_Rect *dest);

        bool isBomb() const;
        bool isRevealed() const;
        bool hasFlag() const;
        int numSurrounding() const;

        bool reveal();
        bool toggleFlag();

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