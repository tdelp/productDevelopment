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

    void drawSafeBox(ivec2 min, ivec2 max, ivec3 color) {
        if (min.x < 0 || max.x >= width || min.y < 0 || max.y >= height) return;
        drawBox(min, max, color);
    }

};

int main(int argc, char* args[])
{
    SDL_Window* window = nullptr;
    SDL_Surface* vbuffer = nullptr;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("SDL2 Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_X, RES_Y, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return 2;
    }

    // Get window surface
    vbuffer = SDL_GetWindowSurface(window);
    SDL_FillRect(vbuffer, NULL, SDL_MapRGB(vbuffer->format, 0x00, 0x00, 0x00)); // Clear screen to black
    SDL_UpdateWindowSurface(window);

    // Create Screen object
    Screen screen(RES_X, RES_Y, vbuffer);

    // Draw four lines from the center to the corners using Bresenham Line Algorithm
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(0, 0), ivec3(255, 0, 0)); // Top-left
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(RES_X, 0), ivec3(0, 255, 0)); // Top-right
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(0, RES_Y), ivec3(0, 0, 255)); // Bottom-left
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(RES_X, RES_Y), ivec3(255, 255, 0)); // Bottom-right

    // Draw a box in the center of the screen
    screen.drawSafeBox(ivec2(RES_X / 2 - 50, RES_Y / 2 - 50), ivec2(RES_X / 2 + 50, RES_Y / 2 + 50), ivec3(255, 0, 255)); // Centered box

    // Update window with drawn content
    screen.blitTo(vbuffer);
    SDL_UpdateWindowSurface(window);

    // Event loop
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        SDL_UpdateWindowSurface(window);
    }

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}