#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <algorithm> // For std::min and std::max
#include "all_headers.hpp"

const int RES_X = 960;
const int RES_Y = 540;

class Screen {
public:
    unsigned int width, height;
    SDL_Surface* surface;

    Screen(unsigned int w, unsigned int h, SDL_Surface* targetSurface)
        : width(w), height(h), surface(targetSurface) {}

    ~Screen() {
        SDL_FreeSurface(surface);
    }

};

int main(int argc, char* args[])
{
    return 0;
}