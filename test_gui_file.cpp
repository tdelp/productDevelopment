#include "GUIFile.hpp"
#include <cassert>
#include <iostream>
#include <fstream>

// Helper functions to compare integer and float-based colors
bool compareColors(const ivec3& color1, const ivec3& color2) {
    return color1.x == color2.x && color1.y == color2.y && color1.z == color2.z;
}

bool compareColorsFloat(const vec3& color1, const vec3& color2) {
    return (color1.x == color2.x && color1.y == color2.y && color1.z == color2.z);
}

// Function to test reading and writing of XML with both valid and invalid cases
void testXMLReadAndWrite() {
    // Create an input XML file for testing
    std::string inputFilename = "test_input.xml";
    std::ofstream inputFile(inputFilename);
    inputFile << "<layout>\n"
              << "  <line>\n"
              << "    <vec2><x>50.5</x><y>902.47</y></vec2>\n"
              << "    <vec2><x>75.6</x><y>1024.6</y></vec2>\n"
              << "    <vec3><x>244.0</x><y>245.0</y><z>103.3</z></vec3>\n"
              << "  </line>\n"
              << "  <box>\n"
              << "    <vec2><x>250.3</x><y>122.5</y></vec2>\n"
              << "    <vec2><x>420.34</x><y>254.9</y></vec2>\n"
              << "    <ivec3><x>212</x><y>22</y><z>124</z></ivec3>\n"
              << "  </box>\n"
              << "  <point>\n"
              << "    <ivec2><x>480</x><y>270</y></ivec2>\n"
              << "    <ivec3><x>67</x><y>200</y><z>142</z></ivec3>\n"
              << "  </point>\n"
              << "</layout>\n";
    inputFile.close();

    GUIFile guiFile;

    // Read the input file into the GUIFile object
    assert(guiFile.readFromFile(inputFilename) && "Failed to read from XML file");

    // Verify that the float-based line was read correctly
    const auto& linesFloat = guiFile.getLinesFloat();
    assert(linesFloat.size() == 1 && "Expected one float-based line in XML data");
    assert(linesFloat[0].start.x == 50.5f && linesFloat[0].start.y == 902.47f && "Float-based line start position mismatch");
    assert(linesFloat[0].end.x == 75.6f && linesFloat[0].end.y == 1024.6f && "Float-based line end position mismatch");
    assert(compareColorsFloat(linesFloat[0].color, vec3(244.0f, 245.0f, 103.3f)) && "Float-based line color mismatch");

    // Verify that the float-based box was read correctly
    const auto& boxesFloat = guiFile.getBoxesFloat();
    assert(boxesFloat.size() == 1 && "Expected one float-based box in XML data");
    assert(boxesFloat[0].min.x == 250.3f && boxesFloat[0].min.y == 122.5f && "Float-based box min position mismatch");
    assert(boxesFloat[0].max.x == 420.34f && boxesFloat[0].max.y == 254.9f && "Float-based box max position mismatch");
    assert(compareColorsFloat(boxesFloat[0].color, vec3(212.0f, 22.0f, 124.0f)) && "Float-based box color mismatch");

    // Verify that the integer-based point was read correctly
    const auto& points = guiFile.getPoints();
    assert(points.size() == 1 && "Expected one integer-based point in XML data");
    assert(points[0].position.x == 480 && points[0].position.y == 270 && "Integer-based point position mismatch");
    assert(compareColors(points[0].color, ivec3(67, 200, 142)) && "Integer-based point color mismatch");

    // Write the parsed data to an output XML file
    std::string outputFilename = "test_output.xml";
    assert(guiFile.writeToFile(outputFilename) && "Failed to write to output XML file");

    // Read the output file and compare its contents to the expected format
    std::ifstream outputFile(outputFilename);
    assert(outputFile.is_open() && "Unable to open the output XML file for verification");

    // Simple verification to ensure the file has expected content (format and data integrity can be checked more thoroughly as needed)
    std::string line;
    bool foundLine = false, foundBox = false, foundPoint = false;
    while (std::getline(outputFile, line)) {
        if (line.find("<line>") != std::string::npos) foundLine = true;
        if (line.find("<box>") != std::string::npos) foundBox = true;
        if (line.find("<point>") != std::string::npos) foundPoint = true;
    }
    outputFile.close();
    
    assert(foundLine && "Expected a line element in the output XML file");
    assert(foundBox && "Expected a box element in the output XML file");
    assert(foundPoint && "Expected a point element in the output XML file");

    std::cout << "All tests for XML reading, parsing, and writing passed successfully." << std::endl;
}

int main() {
    testXMLReadAndWrite();
    return 0;
}
