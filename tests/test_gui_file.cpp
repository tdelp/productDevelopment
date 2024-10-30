#include "../parse/parse.hpp"   // Use parse.hpp for the GUIFile class
#include "../gui/gui.hpp"     // Use gui.hpp for Element and ElementFactory classes
#include "../Screen.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath> // For std::round

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("GUI Element Display",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1280, 720,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Get the window surface
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        std::cerr << "Failed to get window surface: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create the Screen object
    SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, 1280, 720, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
    if (!screenSurface) {
        std::cerr << "Failed to create screen surface: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    Screen screen(1280, 720, screenSurface);

    // Load and parse the XML file with GUI elements
    GUIFile guiFile;
    if (!guiFile.readFile("input.xml")) {
        std::cerr << "Failed to read and parse XML file." << std::endl;
        SDL_FreeSurface(screen.surface);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Clear the screen by filling it with black
    SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));

    // Draw all elements using polymorphism
    for (const auto& element : guiFile.getElements()) {
        element->draw(screen);
    }

    // Blit the screen surface to the window surface
    SDL_BlitSurface(screen.surface, NULL, windowSurface, NULL);

    // Update the window to display the drawn elements
    SDL_UpdateWindowSurface(window);

    // Save the elements back to the output.xml file
    guiFile.writeFile();

    // Event loop to keep the window open
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Exit the loop if the window is closed
            }
        }
        SDL_Delay(16); // Delay to limit CPU usage
    }

    // Clean up resources
    SDL_FreeSurface(screen.surface);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
