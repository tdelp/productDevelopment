#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "all_headers.hpp"

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

#endif // __TVEC2_HPP__