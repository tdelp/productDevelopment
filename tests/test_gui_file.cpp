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

    // Initialize Parser and load the root layout from XML
    Parser parser("input.xml");  // Assuming the XML file is named "input.xml"
    auto rootLayout = parser.parseRootLayout();
    
    if (!rootLayout) {
        std::cerr << "Error: Root layout could not be parsed." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Calculate initial positions for root and nested layouts
    rootLayout->calculatePosition({0, 0}, {1280, 720});
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

        // Check if mouse is inside the first element of the root layout (assuming first element is a triangle)
        if (!rootLayout->getElements().empty()) {
            bool mouseInsideFirstElement = rootLayout->getElements()[0]->isInside({x, y});
            std::cout << "Mouse inside first element: " << (mouseInsideFirstElement ? "Yes" : "No") << std::endl;

            // Toggle the first nested layout based on mouse position
            if (!rootLayout->getNestedLayouts().empty()) {
                auto& nestedLayout = rootLayout->getNestedLayouts()[0];
                nestedLayout->setActive(mouseInsideFirstElement);
                std::cout << "Nested layout activation status: " << (mouseInsideFirstElement ? "Activated" : "Deactivated") << std::endl;
            }
        }

        // Render the root layout and its nested layouts based on active state
        rootLayout->render(screen);

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
