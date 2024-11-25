#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <algorithm> // For std::min and std::max
#include "Screen.hpp"

const int RES_X = 960;
const int RES_Y = 540;

int main(int argc, char* args[])
{
    SDL_Window* window = nullptr;
    SDL_Surface* vbuffer = nullptr;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;  // Return error if initialization fails
    }

    // Create SDL window
    window = SDL_CreateWindow("SDL2 Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_X, RES_Y, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return 2;  // Return error if window creation fails
    }

    // Get the window's surface (framebuffer)
    vbuffer = SDL_GetWindowSurface(window);
    
    // Clear screen to black
    SDL_FillRect(vbuffer, NULL, SDL_MapRGB(vbuffer->format, 0x00, 0x00, 0x00));
    SDL_UpdateWindowSurface(window);

    // Create Screen object with the window surface
    Screen screen(RES_X, RES_Y, vbuffer);

    // Draw four lines from the center to the corners using Bresenham Line Algorithm
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(0, 0), ivec3(255, 0, 0)); // Top-left
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(RES_X, 0), ivec3(0, 255, 0)); // Top-right
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(0, RES_Y), ivec3(0, 0, 255)); // Bottom-left
    screen.drawLine(ivec2(RES_X / 2, RES_Y / 2), ivec2(RES_X, RES_Y), ivec3(255, 255, 0)); // Bottom-right

    // Draw a centered box on the screen
    screen.drawSafeBox(ivec2(RES_X / 2 - 50, RES_Y / 2 - 50), ivec2(RES_X / 2 + 50, RES_Y / 2 + 50), ivec3(255, 0, 255));

    // Copy the drawn content to the window surface
    screen.blitTo(vbuffer);
    SDL_UpdateWindowSurface(window);  // Update window to reflect changes

    // Main event loop to keep the window open
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)  // Quit if the window is closed
                quit = true;
        }
        SDL_UpdateWindowSurface(window);  // Continuously update the window surface
    }

    // Cleanup: Destroy the window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}