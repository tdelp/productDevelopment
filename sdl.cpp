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

    void setPixel(ivec2 position, ivec3 color) {
        int x = position.x;
        int y = position.y;
        if (x < 0 || x >= width || y < 0 || y >= height) return;

        Uint32* pixels = (Uint32*)surface->pixels;
        Uint32 pixelColor = SDL_MapRGB(surface->format, color.x, color.y, color.z);
        pixels[(y * surface->w) + x] = pixelColor;
    }

    void blitTo(SDL_Surface* destSurface) {
        SDL_BlitSurface(surface, NULL, destSurface, NULL);
    }
    
};

int main(int argc, char* args[])
{
    return 0;
}