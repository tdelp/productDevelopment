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

bool XMLParser::parseXML(const std::string& filename, std::vector<Line>& lines, std::vector<Box>& boxes, std::vector<Point>& points) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }

    std::string line;
    Line currentLine;
    Box currentBox;
    Point currentPoint;
    bool insideLine = false, insideBox = false, insidePoint = false;

    while (std::getline(file, line)) {
        line = trim(line);

        // Check for Line start and end tags
        if (line == "<line>") {
            insideLine = true;
        } else if (line == "</line>") {
            lines.push_back(currentLine);
            insideLine = false;
        }

        // Check for Box start and end tags
        if (line == "<box>") {
            insideBox = true;
        } else if (line == "</box>") {
            boxes.push_back(currentBox);
            insideBox = false;
        }

        // Check for Point start and end tags
        if (line == "<point>") {
            insidePoint = true;
        } else if (line == "</point>") {
            points.push_back(currentPoint);
            insidePoint = false;
        }

        // Parse individual elements for Line, Box, and Point
        if (insideLine || insideBox || insidePoint) {
            std::stringstream ss(line);
            std::string tag, value;
            if (line.find("<x>") != std::string::npos) {
                std::getline(ss, tag, '>');
                std::getline(ss, value, '<');
                int x = std::stoi(value);
                if (insideLine) {
                    if (currentLine.start.x == 0 && currentLine.start.y == 0) currentLine.start.x = x;
                    else currentLine.end.x = x;
                }
                if (insideBox) {
                    if (currentBox.min.x == 0 && currentBox.min.y == 0) currentBox.min.x = x;
                    else currentBox.max.x = x;
                }
                if (insidePoint) currentPoint.position.x = x;
            } else if (line.find("<y>") != std::string::npos) {
                std::getline(ss, tag, '>');
                std::getline(ss, value, '<');
                int y = std::stoi(value);
                if (insideLine) {
                    if (currentLine.start.x != 0 && currentLine.start.y == 0) currentLine.start.y = y;
                    else currentLine.end.y = y;
                }
                if (insideBox) {
                    if (currentBox.min.x != 0 && currentBox.min.y == 0) currentBox.min.y = y;
                    else currentBox.max.y = y;
                }
                if (insidePoint) currentPoint.position.y = y;
            } else if (line.find("<z>") != std::string::npos) {
                std::getline(ss, tag, '>');
                std::getline(ss, value, '<');
                int z = std::stoi(value);
                if (insideLine) currentLine.color.z = z;
                if (insideBox) currentBox.color.z = z;
                if (insidePoint) currentPoint.color.z = z;
            }
        }
    }

    file.close();
    return true;
}
