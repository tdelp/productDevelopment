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

    // Constructor to initialize screen dimensions and surface
    Screen(unsigned int w, unsigned int h, SDL_Surface* targetSurface)
        : width(w), height(h), surface(targetSurface) {}

    // Destructor to free the surface
    ~Screen() {
        SDL_FreeSurface(surface);
    }

    // Function to set a pixel at a specific position with a given color
    void setPixel(ivec2 position, ivec3 color) {
        int x = position.x;
        int y = position.y;

        // Check for valid position inside the screen bounds
        if (x < 0 || x >= width || y < 0 || y >= height) return;

        // Get pointer to pixel array and map RGB color to Uint32 format
        Uint32* pixels = (Uint32*)surface->pixels;
        Uint32 pixelColor = SDL_MapRGB(surface->format, color.x, color.y, color.z);

        // Set the pixel at (x, y) to the specified color
        pixels[(y * surface->w) + x] = pixelColor;
    }

    // Function to copy the surface content to the destination surface
    void blitTo(SDL_Surface* destSurface) {
        SDL_BlitSurface(surface, NULL, destSurface, NULL);
    }

    // Bresenham's Line Algorithm to draw a line between two points
    void drawLine(ivec2 start, ivec2 end, ivec3 color) {
        int dx = abs(end.x - start.x), dy = abs(end.y - start.y);
        int sx = (start.x < end.x) ? 1 : -1;
        int sy = (start.y < end.y) ? 1 : -1;
        int err = dx - dy;

        // Loop until the end point is reached
        while (true) {
            setPixel(start, color);  // Draw the current pixel
            if (start.x == end.x && start.y == end.y) break;  // Stop when the line is complete
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; start.x += sx; }
            if (e2 < dx) { err += dx; start.y += sy; }
        }
    }

    void drawBox(ivec2 min, ivec2 max, ivec3 color) {
        // Ensure that minX is the smaller x-coordinate and maxX is the larger x-coordinate
        int minX = std::min(min.x, max.x);
        int maxX = std::max(min.x, max.x);

        // Ensure that minY is the smaller y-coordinate and maxY is the larger y-coordinate
        int minY = std::min(min.y, max.y);
        int maxY = std::max(min.y, max.y);

        // Loop through all pixels in the specified box and set their color
        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                setPixel(ivec2(x, y), color);  // Color each pixel in the box
            }
        }
    }

    void drawSafeBox(ivec2 min, ivec2 max, ivec3 color) {
        // Check if the box coordinates are within the screen's valid boundaries
        if (min.x < 0 || max.x >= width || min.y < 0 || max.y >= height) return;

        // If valid, proceed to draw the box with the corrected min/max coordinates
        drawBox(min, max, color);
    }


};

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
