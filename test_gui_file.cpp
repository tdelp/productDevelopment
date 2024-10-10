#include "GUIFile.hpp"
#include "Screen.hpp"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialize SDL and create a window
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("GUI Element Display", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    // Create the Screen object to handle drawing operations
    Screen screen(800, 600, screenSurface);

    // Load and parse the XML file with GUI elements
    GUIFile guiFile;
    if (!guiFile.readFile("input.xml")) {
        std::cerr << "Failed to read and parse XML file" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Clear the screen before drawing
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0)); // Fill with black color

    // Render parsed lines
    for (const auto& line : guiFile.getLines()) {
        screen.drawLine(
            ivec2(line.start[0], line.start[1]),
            ivec2(line.end[0], line.end[1]),
            ivec3(line.color[0], line.color[1], line.color[2])
        );
    }

    // Render parsed boxes
    for (const auto& box : guiFile.getBoxes()) {
        screen.drawSafeBox(
            ivec2(box.min[0], box.min[1]),
            ivec2(box.max[0], box.max[1]),
            ivec3(box.color[0], box.color[1], box.color[2])
        );
    }

    // Render parsed points
    for (const auto& point : guiFile.getPoints()) {
        screen.setPixel(
            ivec2(point.position[0], point.position[1]),
            ivec3(point.color[0], point.color[1], point.color[2])
        );
    }

    // Update the window surface to display the drawn elements
    screen.blitTo(screenSurface);
    SDL_UpdateWindowSurface(window);

    // Event loop to keep the window open
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Break the loop if the user closes the window
            }
        }
        SDL_Delay(16); // Delay to control the loop speed
    }

    // Clean up SDL resources
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
