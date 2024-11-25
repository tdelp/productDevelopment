#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "../vecs/Tvec2.hpp"
#include "../vecs/Tvec3.hpp"
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

    // Bresenham's Line Algorithm to draw a line between two points with safe boundary checks
    void drawSafeLine(ivec2 start, ivec2 end, ivec3 color) {
        // Check if both start and end points are outside the screen bounds
        if ((start.x < 0 || start.x >= width || start.y < 0 || start.y >= height) &&
            (end.x < 0 || end.x >= width || end.y < 0 || end.y >= height)) {
            std::cerr << "Error: Both line endpoints are out of bounds. Skipping line drawing.\n";
            return; // Don't draw if both points are out of bounds
        }

        int dx = abs(end.x - start.x), dy = abs(end.y - start.y);
        int sx = (start.x < end.x) ? 1 : -1;
        int sy = (start.y < end.y) ? 1 : -1;
        int err = dx - dy;

        // Loop until the end point is reached
        while (true) {
            setSafePixel(start, color);  // Draw the current pixel using the safe function
            if (start.x == end.x && start.y == end.y) break;  // Stop when the line is complete
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; start.x += sx; }
            if (e2 < dx) { err += dx; start.y += sy; }
        }
    }

    // Function to draw a box using safe boundary checks
    void drawSafeBox(ivec2 min, ivec2 max, ivec3 color) {
        // Check if the box coordinates are outside the screen's valid boundaries
        if (min.x < 0 || max.x >= width || min.y < 0 || max.y >= height) {
            std::cerr << "Error: Box coordinates are out of bounds. Skipping box drawing.\n";
            return;
        }

        drawBox(min, max, color);
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

    // Function to draw a filled triangle using safe boundary checks
    void drawSafeTriangle(ivec2 v0, ivec2 v1, ivec2 v2, ivec3 color) {
        // Compute the bounding box of the triangle
        int minX = std::min({v0.x, v1.x, v2.x});
        int maxX = std::max({v0.x, v1.x, v2.x});
        int minY = std::min({v0.y, v1.y, v2.y});
        int maxY = std::max({v0.y, v1.y, v2.y});

        // Loop through all pixels within the bounding box
        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                // Use barycentric coordinates or cross-product method to check if the pixel is inside the triangle
                if (isInsideTriangle(ivec2(x, y), v0, v1, v2)) {
                    // Set the pixel using safe pixel function
                    setSafePixel(ivec2(x, y), color);
                }
            }
        }
    }

private:
    // Helper function to check if a point is inside the triangle using the cross product method
    bool isInsideTriangle(ivec2 p, ivec2 v0, ivec2 v1, ivec2 v2) {
        // Cross product to find if point p is on the left side of each edge
        int d1 = (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
        int d2 = (p.x - v1.x) * (v2.y - v1.y) - (p.y - v1.y) * (v2.x - v1.x);
        int d3 = (p.x - v2.x) * (v0.y - v2.y) - (p.y - v2.y) * (v0.x - v2.x);

        // Check if point p is inside the triangle by verifying all cross products have the same sign
        bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);
        return !(hasNeg && hasPos);
    }
};

#endif // __SCREEN_HPP__