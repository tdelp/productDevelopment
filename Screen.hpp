#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "all_headers.hpp"
#include <SDL2/SDL.h>
#include <iostream> // Include for debugging output

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

    // Function to set a pixel at a specific position with a given color, with safe boundary checks
    void setSafePixel(ivec2 position, ivec3 color) {
        int x = position.x;
        int y = position.y;

        // Check for valid position inside the screen bounds
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }

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

    // Screen.hpp - Added drawSafeLine() and drawSafeBox()
    void drawSafeLine(int x1, int y1, int x2, int y2, Uint32 color) {
        int dx = abs(x2 - x1), dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            setSafePixel(x1, y1, color);
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x1 += sx; }
            if (e2 < dx) { err += dx; y1 += sy; }
        }
    }

    void drawSafeBox(int x1, int y1, int x2, int y2, Uint32 color) {
        for (int x = x1; x <= x2; ++x) {
            for (int y = y1; y <= y2; ++y) {
                setSafePixel(x, y, color);
            }
        }
    }


    // Function to draw a box with no boundary checks (internal helper method)
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
                setSafePixel(ivec2(x, y), color);  // Color each pixel in the box using the safe pixel function
            }
        }
    }
};

#endif // __SCREEN_HPP__