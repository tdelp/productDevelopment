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

    // Render the first layout for 5 seconds
    Uint32 startTime = SDL_GetTicks();
    while (SDL_GetTicks() - startTime < 5000) {
        SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));
        rootLayout1->render(screen);
        screen.blitTo(windowSurface);
        SDL_UpdateWindowSurface(window);

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

    // Create a ButtonElement and add it to rootLayout2
    ivec2 buttonPosition(50, 25);       // Position of the button on the screen
    ivec2 buttonSize(150, 50);            // Width and height of the button
    ivec3 buttonColor(255, 0, 0);         // Color of the button (red)
    std::string targetLayoutName = "NestedLayout"; // The layout to show/hide when the button is clicked

    auto button = std::make_unique<ButtonElement>(buttonPosition, buttonSize, buttonColor, targetLayoutName);
    rootLayout2->addElement(std::move(button));  // Add button to rootLayout2

    // Main SDL loop with mouse interaction to toggle nested layout activation
    bool running = true;
    while (running) {
        // Clear screen buffer
        SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));

        // Render the second root layout and its nested layouts based on active state
        rootLayout2->render(screen);

        // Blit the rendered screen surface to the window surface and update the window
        screen.blitTo(windowSurface);
        SDL_UpdateWindowSurface(window);

        // Poll for quit event and handle click events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Create a click event and send it to rootLayout2
                Event clickEvent(EventType::CLICK, event.button.x, event.button.y);
                rootLayout2->handleEvent(clickEvent);

                // Toggle the visibility of the first nested layout in rootLayout2 based on button click
                for (auto& element : rootLayout2->getElements()) {
                    ButtonElement* button = dynamic_cast<ButtonElement*>(element.get());
                    if (button && button->isClicked()) {
                        if (!rootLayout2->getNestedLayouts().empty()) {
                            auto& nestedLayout = rootLayout2->getNestedLayouts()[0];
                            nestedLayout->setActive(!nestedLayout->isActive());
                        }
                    }
                }
            }
        }
        SDL_Delay(16); // Delay for 60 FPS
    }

    // Cleanup SDL window and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
