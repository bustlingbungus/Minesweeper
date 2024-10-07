#define SDL_MAIN_HANDLED

// sdl
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "sdl/LWindow.hpp"
#include "sdl/LTexture.hpp"
#include "sdl/LAudio.hpp"

// std
#include <iostream>
#include <memory>
#include <random>

// minesweeper
#include "minesweeper/Cell.hpp"
#include "minesweeper/Minesweeper.hpp"

int main()
{
    // window dimensions
    int WIDTH = 600, HEIGHT = 600;
    auto window = std::make_shared<LWindow>(WIDTH, HEIGHT, "Minesweeper");

    // initialise the game, and create a grid
    Minesweeper minesweeper(window);
    minesweeper.createGrid(10, 10);

    SDL_Event e;
    bool quit = false;
    // main window loop
    while (!quit)
    {
        // handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
            window->handleEvent(e);
            minesweeper.getInput(e);
        }

        // render when not minimised
        if (!window->isMinimized())
        {
            // clear the window
            SDL_SetRenderDrawColor(window->gRenderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(window->gRenderer);

            // render the game
            minesweeper.render();

            // update the window
            SDL_RenderPresent(window->gRenderer);
        }
    }

    return 0;
}