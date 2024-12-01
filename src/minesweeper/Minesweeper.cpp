#include "Minesweeper.hpp"

/* Assign variables */
Minesweeper::Minesweeper(std::shared_ptr<LWindow> window, int GRIDX, int GRIDY)
: window(window), WIDTH(window->getWidth()-(2*MARGIN_SIZE)), HEIGHT(window->getHeight()-(2*MARGIN_SIZE)),
  GRIDX(GRIDX), GRIDY(GRIDY)
{
    // create textures 
    loadTextures("../../assets/");
    // create the game
    createGrid(GRIDX, GRIDY);
}

/* Deallocate resources */
Minesweeper::~Minesweeper()
{
    // clear the grid
    for (auto& row : cells) row.clear();
    cells.clear();

    // free all textures
    cell_bkg0->free(); cell_bkg1->free();
    line_col->free();
    flag->free(); bomb->free(); 
    win->free(); lose->free();
}

/* Reads left/right mouse input */
void Minesweeper::getInput(SDL_Event& e)
{
    // check if a mouse button was clicked (ignore input when the game ends)
    if (e.type == SDL_MOUSEBUTTONDOWN && !game_over)
    {
        // find the coordinates of the mouse click
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (e.button.button == SDL_BUTTON_LEFT) leftClick(x, y);
        else if (e.button.button == SDL_BUTTON_RIGHT) rightClick(x, y);
    }
    // reset the grame is space is pressed
    else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            createGrid(GRIDX, GRIDY);
            game_over = false;
        }
    }
}

/* left clicks (x, y) */
void Minesweeper::leftClick(const int& x, const int& y)
{
    // find the indices of the cell that was pressed
    int cx = (x-MARGIN_SIZE)/side_len, cy = (y-MARGIN_SIZE)/side_len;
    // return if either index is out of bounds
    if (cx<0 || cy<0 || cx>=nx || cy>=ny) return;

    // reveal cells that get clicked, don't reveal flagged cells
    auto cell = cells[cx][cy];
    if (!cell->hasFlag()) 
    {
        // game over if the cell was a bomb
        if (cell->reveal()) {
            // reveal all hidden cells
            revealAll();
            game_over = true;
        }
    }
}

/* right clicks (x, y) */
void Minesweeper::rightClick(const int& x, const int& y)
{
    // find the indices of the cell that was pressed
    int cx = (x-MARGIN_SIZE)/side_len, cy = (y-MARGIN_SIZE)/side_len;
    // return if either index is out of bounds
    if (cx<0 || cy<0 || cx>=nx || cy>=ny) return;

    // toggle the flag for cells that get right clicked
    // alter bomb count if toggling flags on a bomb
    auto cell = cells[cx][cy];
    if (cell->toggleFlag()) {
        if (!cell->hasFlag()) numBombs++;
        else numBombs--;
        // win when there are no bombs left
        if (numBombs == 0) {
            // reveal all hidden cells
            revealAll();
            game_over = true;
        }
    }
}

/* Generates the grid containing all cells */
void Minesweeper::createGrid(const int& width, const int& height)
{
    nx = width; ny = height;
    // calculate the cell side length
    side_len = std::min(WIDTH/nx, HEIGHT/ny);

    // clear the previous grid, reset the number of bombs
    for (auto row : cells) row.clear();
    cells.clear();
    numBombs = 0;

    // initialise a new grid
    cells = std::vector<std::vector<std::shared_ptr<Cell>>>(nx, 
                std::vector<std::shared_ptr<Cell>>(ny, nullptr)
    );

    // create the cells by iterating through each (x,y) pair
    for (int x = 0; x < nx; x++) {
        for (int y = 0; y < ny; y++) 
        {
            // create a vector containing pointers to the cells surrounding (x,y)
            std::vector<std::shared_ptr<Cell>*> surrounding = {};
            // iterate through each of the surrounding (x,y) pairs (skip if either x or y is out of bounds)
            for (int i=x-1;i<=x+1;i++) {
                if (i<0 || i>=nx) continue;
                for (int j=y-1;j<=y+1;j++) {
                    if (j<0 || j>=ny || (j==y&&i==x)) continue;
                    surrounding.push_back(&cells[i][j]);
                }
            }

            // roll to see if the current cell should be a bomb
            bool bomb_roll = ((double)rand()/RAND_MAX) < BOMB_CHANCE;
            // create a new cell, and assign it to cells[x][y]
            cells[x][y] = std::make_shared<Cell>(
                bomb_roll, surrounding, window, 
                cell_bkg0, cell_bkg1, flag, bomb
            );
            if (bomb_roll) numBombs++;
        }
    }

    // have all the cells count the number of adjacent bombs
    for (auto row : cells) {
        for (auto cell : row) {
            cell->countSurrounding();
        }
    }

    // regenerate the board if it cannot be broken
    if (!breakBoard()) createGrid(width, height);
}

/* Reveals a cell with zero adjacent bombs closest to the centre of the grid.
 * returns false if there are no cells with zero bombs */
bool Minesweeper::breakBoard()
{
    // the coordinates of the cell to be broken
    int bx = -1, by = -1;
    // the current smallest displacement
    double mlen = std::sqrt(((nx*nx)/4) + ((ny*ny)/4));

    // iterate through each cell, and check if there are any surrounding bombs
    for (int x=0; x<nx; x++) {
        for (int y=0; y<ny; y++) 
        {
            // if there are no surrounding bombs, see if this cell is closer to the centre than
            // the current shortest distance
            if (cells[x][y]->numSurrounding() == 0) 
            {
                // find the displacement of (x,y) from the grid's centre, and compare it to mlen
                int dx = std::abs((nx/2) - x), dy = std::abs((ny/2) - y);
                double dist = std::sqrt((dx*dx) + (dy*dy));
                if (dist <= mlen) {
                    mlen = dist;
                    bx = x; by = y;
                }
            }
        }
    }

    // if any zero-cell was found, reveal the best one. otherwise, 
    // bx and by will still be -1, so return false
    if (bx>0 && by>0) {
        cells[bx][by]->reveal();
        return true;
    }
    return false;
}

/* reveals all hidden cells */
void Minesweeper::revealAll()
{
    // iterate through each cell. if a cell has not been revealed, reveal it
    for (auto row : cells) {
        for (auto cell : row) {
            if (!cell->isRevealed()) {
                // do not reveal bombs that have been correctly flagged 
                if (!cell->isBomb() || !cell->hasFlag()) cell->reveal();
            }
        }
    }
}

/* renders the entire game */
void Minesweeper::render() const
{
    // render the actual minesweeper grid 
    renderCells(WIDTH, HEIGHT, side_len);
    renderGridlines(WIDTH, HEIGHT, side_len);
    // if the game ended, render the win/loss text
    if (game_over) {
        SDL_Rect txtRect = {0, (3*window->getHeight())/8, 
                            window->getWidth(), window->getHeight()/4};
        if (numBombs == 0) win->render(&txtRect);
        else lose->render(&txtRect);
    }
}

/* render all the cells */
void Minesweeper::renderCells(const int& w, const int& h, const int& side_len) const
{
    SDL_Rect rect = {MARGIN_SIZE,MARGIN_SIZE,side_len,side_len};
    // iterate through each cell, determine the (x,y) coorindate based on the cell indices, and render
    for (int i=0; i<nx; i++) {
        rect.x = (side_len*i) + MARGIN_SIZE;
        for (int j=0; j<ny; j++) {
            rect.y = (side_len*j) + MARGIN_SIZE;
            cells[i][j]->render(&rect);
        }
    }
}

/* renders grilines separating cells */
void Minesweeper::renderGridlines(const int& w, const int& h, const int& side_len) const
{
    // render vertical lines
    SDL_Rect line = {MARGIN_SIZE, MARGIN_SIZE, GRIDLINE_WIDTH, h};
    for (int i=0; i<=nx; i++) {
        line.x = (side_len*i) + MARGIN_SIZE;
        line_col->render(&line);
    }

    // render horizontal lines
    line = {MARGIN_SIZE, MARGIN_SIZE, w+GRIDLINE_WIDTH, GRIDLINE_WIDTH};
    for (int i=0; i<=ny; i++) {
        line.y = (side_len*i) + MARGIN_SIZE;
        line_col->render(&line);
    }
}

/* load/create the textures */
void Minesweeper::loadTextures(const std::string& assets_dir)
{
    // create solid colours 
    cell_bkg0 = std::make_shared<LTexture>(window);
    if (!cell_bkg0->solidColour({170,170,170,255})) {
        std::cerr << "Failed to create solid colour!\n";
    }
    cell_bkg1 = std::make_shared<LTexture>(window);
    if (!cell_bkg1->solidColour({90,90,90,255})) {
        std::cerr << "Failed to create solid colour!\n";
    }
    line_col = std::make_shared<LTexture>(window);
    if (!line_col->solidColour({75,75,75,255})) {
        std::cerr << "Failed to create solid colour!\n";
    }

    // create win/loss text
    win = std::make_shared<LTexture>(window);
    if (!win->loadFromRenderedText("YOU WIN", {0,255,0,255})) {
        std::cerr << "Failed to create win texture!\n";
    }
    lose = std::make_shared<LTexture>(window);
    if (!lose->loadFromRenderedText("YOU LOSE", {255,0,0,255})) {
        std::cerr << "Failed to create lose texture!\n";
    }

    // load images from file 
    std::string filename = assets_dir + "flag.png";
    flag = std::make_shared<LTexture>(window);
    if (!flag->loadFromFile(filename)) {
        std::cerr << "failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "bomb.png";
    bomb = std::make_shared<LTexture>(window);
    if (!bomb->loadFromFile(filename)) {
        std::cerr << "failed to load " << filename <<"!\n";
    }
}