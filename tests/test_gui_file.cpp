#include "../all_headers.hpp"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an SDL window and Screen object
    SDL_Window* window = SDL_CreateWindow("Dynamic GUI Layout",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1280, 720, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, 1280, 720, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
    Screen screen(1280, 720, screenSurface);

    // Load the first layout from the first XML file
    Parser parser1("input1.xml");
    auto rootLayout1 = parser1.parseRootLayout();
    if (!rootLayout1) {
        std::cerr << "Error: First root layout could not be parsed." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    rootLayout1->calculatePosition({0, 0}, {1280, 720});

    // Start time for the 5-second render loop
    Uint32 startTime = SDL_GetTicks();

    // Render loop for the first layout (runs for 5 seconds)
    while (SDL_GetTicks() - startTime < 5000) {
        // Clear screen buffer
        SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));
        
        // Render the first layout
        rootLayout1->render(screen);
        
        // Blit the rendered screen surface to the window surface and update the window
        screen.blitTo(windowSurface);
        SDL_UpdateWindowSurface(window);

        // Poll for quit event
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }

        SDL_Delay(16); // Delay for 60 FPS
    }

    // Clear the screen after 5 seconds
    SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));
    screen.blitTo(windowSurface);
    SDL_UpdateWindowSurface(window);

    // Load the second layout from the second XML file
    Parser parser2("input.xml");
    auto rootLayout2 = parser2.parseRootLayout();
    if (!rootLayout2) {
        std::cerr << "Error: Second root layout could not be parsed." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    rootLayout2->calculatePosition({0, 0}, {1280, 720});
    std::cout << "Root layout and nested layouts positions calculated." << std::endl;

    // Main SDL loop with mouse interaction to toggle nested layout activation
    bool running = true;
    SDL_Event event;
    while (running) {
        // Clear screen buffer
        SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));

        // Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Check if mouse is inside the first element of the second root layout (assuming first element is a triangle)
        if (!rootLayout2->getElements().empty()) {
            bool mouseInsideFirstElement = rootLayout2->getElements()[0]->isInside({x, y});

            // Toggle the first nested layout based on mouse position
            if (!rootLayout2->getNestedLayouts().empty()) {
                auto& nestedLayout = rootLayout2->getNestedLayouts()[0];
                nestedLayout->setActive(mouseInsideFirstElement);
            }
        }

        // Render the second root layout and its nested layouts based on active state
        rootLayout2->render(screen);

        // Blit the rendered screen surface to the window surface and update the window
        screen.blitTo(windowSurface);
        SDL_UpdateWindowSurface(window);

        // Poll for quit event
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_Delay(16); // Delay for 60 FPS
    }

    // Cleanup SDL window and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
