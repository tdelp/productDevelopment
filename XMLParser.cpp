#include "XMLParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Utility function to trim whitespace from strings
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Function to safely convert a string to an integer with error handling
int safeStringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid integer value encountered: '" << str << "'" << std::endl;
        return 0;  // Return a default value or handle it as needed
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Integer value out of range: '" << str << "'" << std::endl;
        return 0;  // Return a default value or handle it as needed
    }
}

// Function to safely convert a string to a float with error handling
float safeStringToFloat(const std::string& str) {
    try {
        return std::stof(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid float value encountered: '" << str << "'" << std::endl;
        return 0.0f;  // Return a default value or handle it as needed
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Float value out of range: '" << str << "'" << std::endl;
        return 0.0f;  // Return a default value or handle it as needed
    }
}

bool XMLParser::parseXML(const std::string& filename, std::vector<Line>& lines, std::vector<Box>& boxes, std::vector<Point>& points,
                         std::vector<LineFloat>& linesFloat, std::vector<BoxFloat>& boxesFloat, std::vector<PointFloat>& pointsFloat) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }

    std::string line;
    Line currentLine;
    LineFloat currentLineFloat;
    Box currentBox;
    BoxFloat currentBoxFloat;
    Point currentPoint;
    PointFloat currentPointFloat;
    bool insideLine = false, insideBox = false, insidePoint = false;
    bool isFloatData = false;

    while (std::getline(file, line)) {
        line = trim(line);
        std::cout << "Processing line: '" << line << "'" << std::endl;  // Debug statement

        // Determine the type of data (float or integer) based on the presence of tags
        if (line.find("<line>") != std::string::npos) {
            insideLine = true;
            isFloatData = false;  // Reset the flag for each new line element
            std::cout << "Detected start of a line element." << std::endl;
        } else if (line.find("</line>") != std::string::npos) {
            if (isFloatData) {
                linesFloat.push_back(currentLineFloat);
                std::cout << "Finished reading a float-based line." << std::endl;
            } else {
                lines.push_back(currentLine);
                std::cout << "Finished reading an integer-based line." << std::endl;
            }
            insideLine = false;
        }

        if (line.find("<ivec2>") != std::string::npos || line.find("<ivec3>") != std::string::npos) {
            isFloatData = false;  // Integer-based data
        } else if (line.find("<vec2>") != std::string::npos || line.find("<vec3>") != std::string::npos) {
            isFloatData = true;  // Float-based data
        }

        if (line.find("<box>") != std::string::npos) {
            insideBox = true;
            isFloatData = false;  // Reset the flag for each new box element
            std::cout << "Detected start of a box element." << std::endl;
        } else if (line.find("</box>") != std::string::npos) {
            if (isFloatData) {
                boxesFloat.push_back(currentBoxFloat);
                std::cout << "Finished reading a float-based box." << std::endl;
            } else {
                boxes.push_back(currentBox);
                std::cout << "Finished reading an integer-based box." << std::endl;
            }
            insideBox = false;
        }

        if (line.find("<point>") != std::string::npos) {
            insidePoint = true;
            isFloatData = false;  // Reset the flag for each new point element
            std::cout << "Detected start of a point element." << std::endl;
        } else if (line.find("</point>") != std::string::npos) {
            if (isFloatData) {
                pointsFloat.push_back(currentPointFloat);
                std::cout << "Finished reading a float-based point." << std::endl;
            } else {
                points.push_back(currentPoint);
                std::cout << "Finished reading an integer-based point." << std::endl;
            }
            insidePoint = false;
        }

        // Parse individual x, y, z elements for both integer and float data
        if (line.find("<x>") != std::string::npos) {
            std::string value = line.substr(line.find("<x>") + 3, line.find("</x>") - (line.find("<x>") + 3));
            std::cout << "Parsed x value: '" << value << "'" << std::endl;
            if (isFloatData) {
                float x = safeStringToFloat(value);
                if (insideLine) currentLineFloat.start.x = x;
                else if (insideBox) currentBoxFloat.min.x = x;
                else if (insidePoint) currentPointFloat.position.x = x;
            } else {
                int x = safeStringToInt(value);
                if (insideLine) currentLine.start.x = x;
                else if (insideBox) currentBox.min.x = x;
                else if (insidePoint) currentPoint.position.x = x;
            }
        }

        if (line.find("<y>") != std::string::npos) {
            std::string value = line.substr(line.find("<y>") + 3, line.find("</y>") - (line.find("<y>") + 3));
            std::cout << "Parsed y value: '" << value << "'" << std::endl;
            if (isFloatData) {
                float y = safeStringToFloat(value);
                if (insideLine) currentLineFloat.start.y = y;
                else if (insideBox) currentBoxFloat.min.y = y;
                else if (insidePoint) currentPointFloat.position.y = y;
            } else {
                int y = safeStringToInt(value);
                if (insideLine) currentLine.start.y = y;
                else if (insideBox) currentBox.min.y = y;
                else if (insidePoint) currentPoint.position.y = y;
            }
        }

        if (line.find("<z>") != std::string::npos) {
            std::string value = line.substr(line.find("<z>") + 3, line.find("</z>") - (line.find("<z>") + 3));
            std::cout << "Parsed z value: '" << value << "'" << std::endl;
            if (isFloatData) {
                float z = safeStringToFloat(value);
                if (insideLine) currentLineFloat.color.z = z;
                else if (insideBox) currentBoxFloat.color.z = z;
                else if (insidePoint) currentPointFloat.color.z = z;
            } else {
                int z = safeStringToInt(value);
                if (insideLine) currentLine.color.z = z;
                else if (insideBox) currentBox.color.z = z;
                else if (insidePoint) currentPoint.color.z = z;
            }
        }
    }

    file.close();
    return true;
}
