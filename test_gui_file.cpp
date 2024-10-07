#include "GUIFile.hpp"
#include "Screen.hpp"
#include <SDL2/SDL.h>
#include <cassert>
#include <iostream>

// Utility function to compare two ivec3 colors
bool compareColors(const ivec3& color1, const ivec3& color2) {
    return color1.x == color2.x && color1.y == color2.y && color1.z == color2.z;
}

// Utility function to compare two vec3 colors
bool compareColorsFloat(const vec3& color1, const vec3& color2) {
    return color1.x == color2.x && color1.y == color2.y && color1.z == color2.z;
}

void testXMLReadAndWrite() {
    GUIFile guiFile;

    // Test data to write to an XML file (integer-based data)
    Line testLine = {ivec2(10, 20), ivec2(30, 40), ivec3(255, 0, 0)};
    Box testBox = {ivec2(50, 60), ivec2(70, 80), ivec3(0, 255, 0)};
    Point testPoint = {ivec2(90, 100), ivec3(0, 0, 255)};

    // Test data to write to an XML file (floating-point data)
    LineFloat testLineFloat = {vec2(15.5f, 25.5f), vec2(35.5f, 45.5f), vec3(255.0f, 100.0f, 50.0f)};
    BoxFloat testBoxFloat = {vec2(55.5f, 65.5f), vec2(75.5f, 85.5f), vec3(0.0f, 255.0f, 125.0f)};
    PointFloat testPointFloat = {vec2(95.5f, 105.5f), vec3(25.0f, 50.0f, 75.0f)};

    // Add integer-based data to the GUIFile
    guiFile.addLine(testLine);
    guiFile.addBox(testBox);
    guiFile.addPoint(testPoint);

    // Add floating-point-based data to the GUIFile
    guiFile.addLineFloat(testLineFloat);
    guiFile.addBoxFloat(testBoxFloat);
    guiFile.addPointFloat(testPointFloat);

    // Write the data to an XML file
    std::string filename = "test_output.xml";
    assert(guiFile.writeToFile(filename) && "Failed to write to XML file");

    // Clear the GUIFile instance and read back the data from the XML file
    GUIFile newGuiFile;
    assert(newGuiFile.readFromFile(filename) && "Failed to read from XML file");

    // Verify integer-based line was read correctly
    const auto& lines = newGuiFile.getLines();
    assert(lines.size() == 1 && "Expected one integer-based line in XML data");
    assert(lines[0].start.x == testLine.start.x && lines[0].start.y == testLine.start.y);
    assert(lines[0].end.x == testLine.end.x && lines[0].end.y == testLine.end.y);
    assert(compareColors(lines[0].color, testLine.color) && "Integer-based line color mismatch");

    // Verify floating-point-based line was read correctly
    const auto& linesFloat = newGuiFile.getLinesFloat();
    assert(linesFloat.size() == 1 && "Expected one floating-point-based line in XML data");
    assert(linesFloat[0].start.x == testLineFloat.start.x && linesFloat[0].start.y == testLineFloat.start.y);
    assert(linesFloat[0].end.x == testLineFloat.end.x && linesFloat[0].end.y == testLineFloat.end.y);
    assert(compareColorsFloat(linesFloat[0].color, testLineFloat.color) && "Floating-point line color mismatch");

    // Repeat similar checks for boxes and points (both integer-based and floating-point-based)

    std::cout << "XML read/write tests for integer and floating-point data passed successfully." << std::endl;
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

    // Render integer-based elements using the Screen class
    for (const auto& line : guiFile.getLines()) {
        screen.drawLine(line.start, line.end, line.color);
    }
    for (const auto& box : guiFile.getBoxes()) {
        screen.drawSafeBox(box.min, box.max, box.color);
    }
    for (const auto& point : guiFile.getPoints()) {
        screen.setPixel(point.position, point.color);
    }

    // Render floating-point-based elements using the Screen class
    for (const auto& lineFloat : guiFile.getLinesFloat()) {
        screen.drawLine(ivec2(lineFloat.start.x, lineFloat.start.y), ivec2(lineFloat.end.x, lineFloat.end.y), ivec3(lineFloat.color.x, lineFloat.color.y, lineFloat.color.z));
    }
    for (const auto& boxFloat : guiFile.getBoxesFloat()) {
        screen.drawSafeBox(ivec2(boxFloat.min.x, boxFloat.min.y), ivec2(boxFloat.max.x, boxFloat.max.y), ivec3(boxFloat.color.x, boxFloat.color.y, boxFloat.color.z));
    }
    for (const auto& pointFloat : guiFile.getPointsFloat()) {
        screen.setPixel(ivec2(pointFloat.position.x, pointFloat.position.y), ivec3(pointFloat.color.x, pointFloat.color.y, pointFloat.color.z));
    }

    // Copy the rendered screen to the window surface and display it
    screen.blitTo(windowSurface);
    SDL_UpdateWindowSurface(window);

    // Wait for a few seconds before exiting to allow viewing of the rendered elements
    SDL_Delay(3000);

    // Cleanup SDL resources
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Rendering test for integer and floating-point data completed." << std::endl;
}

int main() {
    testXMLReadAndWrite();
    testRenderingWithScreen();
    return 0;
}
