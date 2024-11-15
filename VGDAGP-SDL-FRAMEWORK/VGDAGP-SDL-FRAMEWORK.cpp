// Liana Hockin
// November 4th 2024
// main SDL WORK
#include <iostream>
#include<SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool quit = false;

int main(int argc, char* args[])
{
    // initialize SDL subsystems
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) > 0) {
        // we have failed to initialize
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    window = SDL_CreateWindow(
        "SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        //we have failed to create a window
        std::cerr << "unable to create Window! SDL_Error: " << SDL_GetError() << std:: endl;
        return -1;
    }

    // anything after this can assume that our window was able to succesfully create itself
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        //we have failed to create a renderer
        std::cerr << "unable to get renderer. SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // main game loop
    SDL_SetRenderDrawColor(renderer, 192,192,192,192); // silver colour
    SDL_Event events = {};
    
    //main game loop
    while(!quit) {
        // event polling loop
        // while there are events inside of our events variable
        while (SDL_PollEvent(&events)) {
            // handle each and every event we want to look for
            switch (events.type) {
            case SDL_QUIT:
                quit = true;
                break;

            }
        }
        //draw code goes down here
        SDL_RenderFillRect(renderer, nullptr);
        SDL_RenderPresent(renderer);
    }

    //destroy the renderer
    SDL_DestroyRenderer(renderer);
    //destroy the window
    SDL_DestroyWindow(window);

    // Terminate SDL Subsystems
    //Functions are NOT all uppercase, so... SDL_Quit();
    //Enums ARE all uppercase! so... SDL_INIT_VIDEO is an enum
    SDL_Quit();
    return 0;
}