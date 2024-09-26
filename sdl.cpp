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

    void drawLine(ivec2 start, ivec2 end, ivec3 color) {
        int dx = abs(end.x - start.x), dy = abs(end.y - start.y);
        int sx = (start.x < end.x) ? 1 : -1;
        int sy = (start.y < end.y) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            setPixel(start, color);
            if (start.x == end.x && start.y == end.y) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; start.x += sx; }
            if (e2 < dx) { err += dx; start.y += sy; }
        }
    }

    void drawBox(ivec2 min, ivec2 max, ivec3 color) {
        int minX = std::min(min.x, max.x);
        int maxX = std::max(min.x, max.x);
        int minY = std::min(min.y, max.y);
        int maxY = std::max(min.y, max.y);

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                setPixel(ivec2(x, y), color);
            }
        }
    }

};

int main(int argc, char* args[])
{
    return 0;
}