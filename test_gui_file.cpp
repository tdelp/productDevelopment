#include "GUIFile.hpp"
#include "Screen.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath> // Include cmath for the round function

int main(int argc, char* argv[]) {
    // Initialize SDL and create a window
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window with a default size of 1280x720
    SDL_Window* window = SDL_CreateWindow("GUI Element Display", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    // Create the Screen object with a size of 1280x720
    Screen screen(1280, 720, SDL_CreateRGBSurface(0, 1280, 720, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0));

    // Load and parse the XML file with GUI elements
    GUIFile guiFile;
    if (!guiFile.readFile("input.xml")) {
        std::cerr << "Failed to read and parse XML file" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Clear the screen before drawing by filling it with a black color
    SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0)); 

    // Render parsed lines to the screen with debugging output
    for (const auto& line : guiFile.getLines()) {
        int startX = static_cast<int>(std::round(line.start[0]));
        int startY = static_cast<int>(std::round(line.start[1]));
        int endX = static_cast<int>(std::round(line.end[0]));
        int endY = static_cast<int>(std::round(line.end[1]));
        int colorR = static_cast<int>(std::round(line.color[0]));
        int colorG = static_cast<int>(std::round(line.color[1]));
        int colorB = static_cast<int>(std::round(line.color[2]));

        // Draw the line using the safe function
        screen.drawSafeLine(
            ivec2(startX, startY),
            ivec2(endX, endY),
            ivec3(colorR, colorG, colorB)
        );
    }

    // Render parsed boxes to the screen using the safe drawing function
    for (const auto& box : guiFile.getBoxes()) {
        screen.drawSafeBox(
            ivec2(static_cast<int>(std::round(box.min[0])), static_cast<int>(std::round(box.min[1]))),
            ivec2(static_cast<int>(std::round(box.max[0])), static_cast<int>(std::round(box.max[1]))),
            ivec3(static_cast<int>(std::round(box.color[0])), static_cast<int>(std::round(box.color[1])), static_cast<int>(std::round(box.color[2])))
        );
    }

    // Render parsed points to the screen using the safe pixel function
    for (const auto& point : guiFile.getPoints()) {
        screen.setSafePixel(
            ivec2(static_cast<int>(std::round(point.position[0])), static_cast<int>(std::round(point.position[1]))),
            ivec3(static_cast<int>(std::round(point.color[0])), static_cast<int>(std::round(point.color[1])), static_cast<int>(std::round(point.color[2])))
        );
    }

    // Blit the screen surface to the window surface
    screen.blitTo(windowSurface);

    // Update the window surface to display the drawn elements
    SDL_UpdateWindowSurface(window);

    // Event loop to keep the window open and handle SDL events
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Break the loop if the user closes the window
            }
        }
        SDL_Delay(16); // Control the loop speed to avoid high CPU usage
    }

    // Clean up SDL resources
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
