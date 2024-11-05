// Liana Hockin
// November 4th 2024
#include <iostream>
#include<SDL.h>

int main(int argc, char* args[])
{
    // initialize SDL subsystems
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) > 0) {
        // we have failed to initialize
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }




    // Terminate SDL Subsystems
    //Functions are NOT all uppercase, so... SDL_Quit();
    //Enums ARE all uppercase! so... SDL_INIT_VIDEO is an enum
    SDL_Quit();
    return 0;
}