#include "GUIFile.hpp"
#include "Screen.hpp"
#include <SDL2/SDL.h>
#include <cassert>
#include <iostream>

// Utility function to compare two ivec3 colors
bool compareColors(const ivec3& color1, const ivec3& color2) {
    return color1.x == color2.x && color1.y == color2.y && color1.z == color2.z;
}

void testXMLReadAndWrite() {
    GUIFile guiFile;

    // Test data to write to an XML file
    Line testLine = {ivec2(10, 20), ivec2(30, 40), ivec3(255, 0, 0)};
    Box testBox = {ivec2(50, 60), ivec2(70, 80), ivec3(0, 255, 0)};
    Point testPoint = {ivec2(90, 100), ivec3(0, 0, 255)};

    guiFile.addLine(testLine);
    guiFile.addBox(testBox);
    guiFile.addPoint(testPoint);

    // Write the data to an XML file
    std::string filename = "test_output.xml";
    assert(guiFile.writeToFile(filename) && "Failed to write to XML file");

    // Clear the GUIFile instance and read back the data from the XML file
    GUIFile newGuiFile;
    assert(newGuiFile.readFromFile(filename) && "Failed to read from XML file");

    // Verify that the line was read correctly
    const auto& lines = newGuiFile.getLines();
    assert(lines.size() == 1 && "Expected one line in XML data");
    assert(lines[0].start.x == testLine.start.x && lines[0].start.y == testLine.start.y);
    assert(lines[0].end.x == testLine.end.x && lines[0].end.y == testLine.end.y);
    assert(compareColors(lines[0].color, testLine.color) && "Line color mismatch");

    // Verify that the box was read correctly
    const auto& boxes = newGuiFile.getBoxes();
    assert(boxes.size() == 1 && "Expected one box in XML data");
    assert(boxes[0].min.x == testBox.min.x && boxes[0].min.y == testBox.min.y);
    assert(boxes[0].max.x == testBox.max.x && boxes[0].max.y == testBox.max.y);
    assert(compareColors(boxes[0].color, testBox.color) && "Box color mismatch");

    // Verify that the point was read correctly
    const auto& points = newGuiFile.getPoints();
    assert(points.size() == 1 && "Expected one point in XML data");
    assert(points[0].position.x == testPoint.position.x && points[0].position.y == testPoint.position.y);
    assert(compareColors(points[0].color, testPoint.color) && "Point color mismatch");

    std::cout << "XML read/write tests passed successfully." << std::endl;
}

void testRenderingWithScreen() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    // Create the SDL window and surface
    SDL_Window* window = SDL_CreateWindow("Test Renderer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        std::cerr << "Failed to get window surface: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Create an instance of the Screen class
    Screen screen(800, 600, windowSurface);

    // Create a test GUIFile instance and load data from a test XML file
    GUIFile guiFile;
    if (!guiFile.readFromFile("test_output.xml")) {
        std::cerr << "Failed to read the XML file." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
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
    SDL_Delay(3000);

    // Cleanup SDL resources
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Rendering test completed." << std::endl;
}

int main() {
    testXMLReadAndWrite();
    testRenderingWithScreen();
    return 0;
}
