#include "GUIFile.hpp"
#include "Screen.hpp"
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("XML GUI Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    Screen screen(800, 600, windowSurface);

    GUIFile guiFile;
    guiFile.readFromFile("gui_layout.xml"); // Assuming you have this XML file to read from

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

    screen.blitTo(windowSurface);
    SDL_UpdateWindowSurface(window);

    // Wait for a few seconds before exiting
    SDL_Delay(5000);

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
