#include "Cell.hpp"

/* returns a colour based on the number of surrounding bombs */
SDL_Color getColour(int numSurroundingBombs)
{
    switch (numSurroundingBombs)
    {
        case 1: return {50,50,255,255};
        case 2: return {50,255,50,255};
        case 3: return {255,50,50,255};
        case 4: return {170,0,255,255};
        case 5: return {255,255,0,255};
        case 6: return {255,150,0,255};
        case 7: return {20,20,20,255};
        case 8: return {30,0,0,255};
    }
}

/*
 * A cell in the minesweeper board.
 * 
 * \param isBomb whether or not the cell contains a bomb
 * \param surrounding vector of pointers to cell pointers surrounding this one
 * \param window the window the cell gets rendered to 
 * \param bkg_revealed the background of the cell when revealed
 * \param bkg_hidden the background of the cell when hidden
 * \param flag_tex the texture used for a flag
 * \param bomb_tex the texture used for a bomb
 */
Cell::Cell(bool isBomb, std::vector<std::shared_ptr<Cell>*> surrounding, 
            std::shared_ptr<LWindow> window, std::shared_ptr<LTexture> bkg_revealed, 
            std::shared_ptr<LTexture> bkg_hidden, std::shared_ptr<LTexture> flag_tex, 
            std::shared_ptr<LTexture> bomb_tex)
: window(window), bkg0(bkg_revealed), bkg1(bkg_hidden), surrounding(surrounding),
  flag(flag_tex), bomb(bomb_tex)
{
    // allocate the number texture
    num = std::make_unique<LTexture>(window);

    // if the cell is not a bomb, count how many adjacent cells are bombs
    if (isBomb) num_bombs = -1;
    else countSurrounding();
}

/* Deallocate resources */
Cell::~Cell()
{
    num->free();
    surrounding.clear();
}

/* Renders the cell within the provided rect */
void Cell::render(SDL_Rect *dest) 
{
    // render cell information if revealed 
    if (is_revealed) 
    {
        bkg0->render(dest);
        if (isBomb()) bomb->render(dest);
        // render the number of bombs in a smaller contained rect
        else if (num_bombs > 0) {
            SDL_Rect numRect = {dest->x+(dest->w/4), dest->y+(dest->h/4), dest->w/2, dest->h/2};
            num->render(&numRect);
        }
    }
    // if hidden just render hidden background, and any flag
    else 
    {
        bkg1->render(dest);
        if (has_flag) flag->render(dest);
    }
}

/* whether or not the cell contains a bomb */
bool Cell::isBomb() const { return num_bombs == -1; }
/* Whether or not the cell has been revealed */
bool Cell::isRevealed() const { return is_revealed; }
/* Whether or not the cell has been given a flag */
bool Cell::hasFlag() const { return has_flag; }
/* The number of bombs in cells adjacent to this one */
int Cell::numSurrounding() const { return num_bombs; }

/* Adds a flag if there is none. Removes flag if there is */
bool Cell::toggleFlag()
{
    has_flag = !has_flag;
    return isBomb();
}

/*
 * Reveals this cell.
 * If there are zero surrounding bombs, reveals all adjacent cells
 * \returns `true` if the cell was a bomb
 */
bool Cell::reveal()
{
    // make cell revealed 
    is_revealed = true;
    // if there are not surrounding bombs, reveal all surrounding
    if (num_bombs == 0) {
        for (auto& cell : surrounding) {
            if (!(*cell)->isRevealed()) (*cell)->reveal();
        }
    }
    // return whether the cell is a bomb
    return isBomb();
}

/* if the cell is not a bomb, checks all surrounding cells and counts how many are bombs. */
void Cell::countSurrounding()
{
    if (!isBomb())
    {
        // reset counter
        num_bombs = 0;
        // for each surrounding cell, increment counter for each bomb
        for (auto& cell : surrounding) {
            if (*cell != nullptr) if ((*cell)->isBomb()) num_bombs++;
        }
        // if there are surrounding bombs, create a texture representing the number
        if (num_bombs > 0) num->loadFromRenderedText(std::to_string(num_bombs), getColour(num_bombs));
    }
}