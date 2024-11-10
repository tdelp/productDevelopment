#include "GUIFile.hpp"
#include "Screen.hpp"
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    // Create the SDL window and surface
    SDL_Window* window = SDL_CreateWindow("XML GUI Renderer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        SDL_Log("Failed to get window surface: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create an instance of the Screen class
    Screen screen(800, 600, windowSurface);

    // Create an instance of GUIFile and read the XML file
    GUIFile guiFile;
    if (!guiFile.readFromFile("gui_layout.xml")) {
        SDL_Log("Failed to read the XML file.");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Render the elements using the Screen class
    for (const auto& line : guiFile.getLines()) {
        screen.drawLine(line.start, line.end, line.color);
    }

    for (const auto& box : guiFile.getBoxes()) {
        screen.drawSafeBox(box.min, box.max, box.color);
    }

    for (const auto& point : guiFile.getPoints()) {
        screen.setPixel(point.position, point.color);
    }

    // Copy the rendered screen to the window surface and display it
    screen.blitTo(windowSurface);
    SDL_UpdateWindowSurface(window);

    // Wait for a few seconds before exiting to allow viewing of the rendered elements
    SDL_Delay(5000);

    // Cleanup SDL resources
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
