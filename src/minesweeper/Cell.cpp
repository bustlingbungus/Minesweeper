#include "Cell.hpp"

SDL_Color getColour(const int& numSurroundingBombs)
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

Cell::Cell(bool isBomb, std::vector<std::shared_ptr<Cell>*> surrounding, 
            std::shared_ptr<LWindow> window, std::shared_ptr<LTexture> bkg_revealed, 
            std::shared_ptr<LTexture> bkg_hidden, std::shared_ptr<LTexture> flag_tex, 
            std::shared_ptr<LTexture> bomb_tex)
: window(window), bkg0(bkg_revealed), bkg1(bkg_hidden), surrounding(surrounding),
  flag(flag_tex), bomb(bomb_tex)
{
    num = std::make_unique<LTexture>(window);

    if (isBomb) num_bombs = -1;
    else countSurrounding();
}

Cell::~Cell()
{
    num->free();
}

void Cell::render(SDL_Rect *dest) const 
{
    if (is_revealed) 
    {
        bkg0->render(dest);
        if (isBomb()) bomb->render(dest);
        else if (num_bombs > 0) {
            SDL_Rect numRect = {dest->x+(dest->w/4), dest->y+(dest->h/4), dest->w/2, dest->h/2};
            num->render(&numRect);
        }
    }
    else 
    {
        bkg1->render(dest);
        if (has_flag) flag->render(dest);
    }
}

bool Cell::isBomb() const { return num_bombs == -1; }
bool Cell::isRevealed() const { return is_revealed; }
bool Cell::hasFlag() const { return has_flag; }
int Cell::numSurrounding() const { return num_bombs; }

bool Cell::toggleFlag()
{
    has_flag = !has_flag;
    return isBomb();
}

bool Cell::reveal()
{
    is_revealed = true;
    if (num_bombs == 0) {
        for (auto cell : surrounding) {
            if (!(*cell)->isRevealed()) (*cell)->reveal();
        }
    }
    return isBomb();
}

void Cell::countSurrounding()
{
    if (!isBomb())
    {
        num_bombs = 0;
        for (auto cell : surrounding) {
            if (*cell != nullptr) if ((*cell)->isBomb()) num_bombs++;
        }
        if (num_bombs > 0) num->loadFromRenderedText(std::to_string(num_bombs), getColour(num_bombs));
    }
}