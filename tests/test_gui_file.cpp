#include "../all_headers.hpp"
#include "../SoundPlayer.hpp"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Dynamic GUI Layout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1280, 720, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, 1280, 720, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
    Screen screen(1280, 720, screenSurface);

    // Initialize SoundPlayer and load a sound file
    SoundPlayer soundPlayer;
    if (!soundPlayer.loadSound("ding.wav")) {
        std::cerr << "Failed to load sound file" << std::endl;
    }

    // Load and display the first layout (input1.xml)
    Parser parser1("input1.xml");
    auto rootLayout1 = parser1.parseRootLayout();
    if (!rootLayout1) {
        std::cerr << "Error: First root layout could not be parsed." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    rootLayout1->calculatePosition({0, 0}, {1280, 720});

    // Create the SHOW ButtonElement in the bottom middle of the screen, hoverable but not clickable
    ivec2 showButtonPosition(565, 650);  // Position for bottom middle
    ivec2 buttonSize(150, 50);
    ivec3 buttonColor(0, 255, 0);
    auto hoverableButton = std::make_unique<ButtonElement>(showButtonPosition, buttonSize, buttonColor, true, false);
    rootLayout1->addElement(std::move(hoverableButton));

    // Display the first layout for 5 seconds
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
            } else if (event.type == SDL_MOUSEMOTION) {
                // Handle SHOW event for hovering
                Event showEvent(EventType::SHOW, event.motion.x, event.motion.y);
                rootLayout1->handleEvent(showEvent, &soundPlayer);
            }
        }
        SDL_Delay(16); // Delay for 60 FPS
    }

    // Clear the screen after displaying the first layout
    SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));
    screen.blitTo(windowSurface);
    SDL_UpdateWindowSurface(window);

    // Load and interact with the second layout (input.xml)
    Parser parser2("input.xml");
    auto rootLayout2 = parser2.parseRootLayout();
    if (!rootLayout2) {
        std::cerr << "Error: Second root layout could not be parsed." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    rootLayout2->calculatePosition({0, 0}, {1280, 720});

    // Create the CLICK ButtonElement in the second layout (clickable with sound but not hoverable)
    ivec2 buttonPosition(50, 25);
    auto clickableButton = std::make_unique<ButtonElement>(buttonPosition, buttonSize, buttonColor, false, true);
    rootLayout2->addElement(std::move(clickableButton));

    // Main loop to interact with the second layout
    bool running = true;
    while (running) {
        SDL_FillRect(screen.surface, NULL, SDL_MapRGB(screen.surface->format, 0, 0, 0));
        rootLayout2->render(screen);
        screen.blitTo(windowSurface);
        SDL_UpdateWindowSurface(window);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Handle CLICK event
                Event clickEvent(EventType::CLICK, event.button.x, event.button.y);
                rootLayout2->handleEvent(clickEvent, &soundPlayer);
            } else if (event.type == SDL_MOUSEMOTION) {
                // Handle SHOW event for hovering
                Event showEvent(EventType::SHOW, event.motion.x, event.motion.y);
                rootLayout2->handleEvent(showEvent, &soundPlayer);
            }
        }
        SDL_Delay(16); // Delay for 60 FPS
    }

    // Clean up and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
