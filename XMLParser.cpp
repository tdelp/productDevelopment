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
        return 0;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Integer value out of range: '" << str << "'" << std::endl;
        return 0;
    }
}

// Function to safely convert a string to a float with error handling
float safeStringToFloat(const std::string& str) {
    try {
        return std::stof(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid float value encountered: '" << str << "'" << std::endl;
        return 0.0f;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Float value out of range: '" << str << "'" << std::endl;
        return 0.0f;
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
    LineFloat currentLineFloat;
    BoxFloat currentBoxFloat;
    PointFloat currentPointFloat;
    Line currentLine;
    Box currentBox;
    Point currentPoint;
    bool insideLine = false;
    bool insideBox = false;
    bool insidePoint = false;
    bool isFloatData = false;
    bool parsingColor = false;

    while (std::getline(file, line)) {
        line = trim(line);
        std::cout << "Processing line: '" << line << "'" << std::endl;

        // Handle opening tags for each element type
        if (line.find("<line>") != std::string::npos) {
            insideLine = true;
            isFloatData = false;
            parsingColor = false;
            currentLineFloat = {};
            currentLine = {};
            std::cout << "Detected start of a line element." << std::endl;
        } else if (line.find("</line>") != std::string::npos) {
            if (isFloatData) {
                linesFloat.push_back(currentLineFloat);
            } else {
                lines.push_back(currentLine);
            }
            insideLine = false;
        }

        if (line.find("<box>") != std::string::npos) {
            insideBox = true;
            isFloatData = false;
            parsingColor = false;
            currentBoxFloat = {};
            currentBox = {};
            std::cout << "Detected start of a box element." << std::endl;
        } else if (line.find("</box>") != std::string::npos) {
            if (isFloatData) {
                boxesFloat.push_back(currentBoxFloat);
                std::cout << "Finished reading a float-based box: Min (" << currentBoxFloat.min.x << ", " << currentBoxFloat.min.y << "), "
                          << "Max (" << currentBoxFloat.max.x << ", " << currentBoxFloat.max.y << ")" << std::endl;
            } else {
                boxes.push_back(currentBox);
            }
            insideBox = false;
        }

        if (line.find("<point>") != std::string::npos) {
            insidePoint = true;
            isFloatData = false;
            parsingColor = false;
            currentPointFloat = {};
            currentPoint = {};
            std::cout << "Detected start of a point element." << std::endl;
        } else if (line.find("</point>") != std::string::npos) {
            if (isFloatData) {
                pointsFloat.push_back(currentPointFloat);
            } else {
                points.push_back(currentPoint);
            }
            insidePoint = false;
        }

        // Detect float or integer data in vec2/vec3 and ivec2/ivec3 tags
        if (insideBox || insideLine || insidePoint) {  // Check if we are inside any element that supports float data
            if (line.find("<vec2>") != std::string::npos) {
                isFloatData = true;
                parsingColor = false;
            } else if (line.find("<vec3>") != std::string::npos) {
                isFloatData = true;
                parsingColor = true;
            } else if (line.find("<ivec2>") != std::string::npos) {
                isFloatData = false;
                parsingColor = false;
            } else if (line.find("<ivec3>") != std::string::npos) {
                if (insideBox && isFloatData) {
                    // Special case handling: if the box was already detected as float-based (using <vec2>), allow integer colors
                    parsingColor = true;  // Allow integer-based colors even for float-based boxes
                } else {
                    isFloatData = false;
                    parsingColor = true;
                }
            }
        }

        // Detect float or integer data in vec2/vec3 and ivec2/ivec3 tags
        if (insideBox || insideLine || insidePoint) {  // Check if we are inside any element that supports float data
            if (line.find("<vec2>") != std::string::npos || line.find("<vec3>") != std::string::npos) {
                // Mark as float-based and ensure we maintain float status for the current element
                isFloatData = true;
                parsingColor = (line.find("<vec3>") != std::string::npos);
            } else if (line.find("<ivec2>") != std::string::npos || line.find("<ivec3>") != std::string::npos) {
                // Check if we have already determined the element as float-based; if not, set to integer-based
                if (!isFloatData) {
                    isFloatData = false;
                    parsingColor = (line.find("<ivec3>") != std::string::npos);
                } else {
                    // If we already marked the element as float-based, we allow integer colors to be assigned
                    parsingColor = true;
                }
            }
        }

        // Parsing for coordinates (x, y) and colors (x, y, z) for float and integer values
        if (!parsingColor) {
            if (line.find("<x>") != std::string::npos) {
                std::string value = line.substr(line.find("<x>") + 3, line.find("</x>") - (line.find("<x>") + 3));
                if (isFloatData) {
                    float x = safeStringToFloat(value);
                    if (insideLine && currentLineFloat.start.x == 0.0f) {
                        currentLineFloat.start.x = x;
                    } else if (insideLine) {
                        currentLineFloat.end.x = x;
                    } else if (insideBox && currentBoxFloat.min.x == 0.0f) {
                        currentBoxFloat.min.x = x;
                    } else if (insideBox) {
                        currentBoxFloat.max.x = x;
                    } else if (insidePoint && currentPointFloat.position.x == 0.0f) {
                        currentPointFloat.position.x = x;
                    }
                } else {
                    int x = safeStringToInt(value);
                    if (insideLine && currentLine.start.x == 0) {
                        currentLine.start.x = x;
                    } else if (insideLine) {
                        currentLine.end.x = x;
                    } else if (insideBox && currentBox.min.x == 0) {
                        currentBox.min.x = x;
                    } else if (insideBox) {
                        currentBox.max.x = x;
                    } else if (insidePoint && currentPoint.position.x == 0) {
                        currentPoint.position.x = x;
                    }
                }
            }

            if (line.find("<y>") != std::string::npos) {
                std::string value = line.substr(line.find("<y>") + 3, line.find("</y>") - (line.find("<y>") + 3));
                if (isFloatData) {
                    float y = safeStringToFloat(value);
                    if (insideLine && currentLineFloat.start.y == 0.0f) {
                        currentLineFloat.start.y = y;
                    } else if (insideLine) {
                        currentLineFloat.end.y = y;
                    } else if (insideBox && currentBoxFloat.min.y == 0.0f) {
                        currentBoxFloat.min.y = y;
                    } else if (insideBox) {
                        currentBoxFloat.max.y = y;
                    } else if (insidePoint && currentPointFloat.position.y == 0.0f) {
                        currentPointFloat.position.y = y;
                    }
                } else {
                    int y = safeStringToInt(value);
                    if (insideLine && currentLine.start.y == 0) {
                        currentLine.start.y = y;
                    } else if (insideLine) {
                        currentLine.end.y = y;
                    } else if (insideBox && currentBox.min.y == 0) {
                        currentBox.min.y = y;
                    } else if (insideBox) {
                        currentBox.max.y = y;
                    } else if (insidePoint && currentPoint.position.y == 0) {
                        currentPoint.position.y = y;
                    }
                }
            }
        }

        // Parsing for color values inside any element
        if (parsingColor) {
            if (line.find("<x>") != std::string::npos) {
                std::string value = line.substr(line.find("<x>") + 3, line.find("</x>") - (line.find("<x>") + 3));
                if (isFloatData) {
                    if (insideLine) currentLineFloat.color.x = safeStringToFloat(value);
                    else if (insideBox) currentBoxFloat.color.x = safeStringToFloat(value);
                    else if (insidePoint) currentPointFloat.color.x = safeStringToFloat(value);
                } else {
                    if (insideLine) currentLine.color.x = safeStringToInt(value);
                    else if (insideBox) currentBox.color.x = safeStringToInt(value);
                    else if (insidePoint) currentPoint.color.x = safeStringToInt(value);
                }
            }

            if (line.find("<y>") != std::string::npos) {
                std::string value = line.substr(line.find("<y>") + 3, line.find("</y>") - (line.find("<y>") + 3));
                if (isFloatData) {
                    if (insideLine) currentLineFloat.color.y = safeStringToFloat(value);
                    else if (insideBox) currentBoxFloat.color.y = safeStringToFloat(value);
                    else if (insidePoint) currentPointFloat.color.y = safeStringToFloat(value);
                } else {
                    if (insideLine) currentLine.color.y = safeStringToInt(value);
                    else if (insideBox) currentBox.color.y = safeStringToInt(value);
                    else if (insidePoint) currentPoint.color.y = safeStringToInt(value);
                }
            }

            if (line.find("<z>") != std::string::npos) {
                std::string value = line.substr(line.find("<z>") + 3, line.find("</z>") - (line.find("<z>") + 3));
                if (isFloatData) {
                    if (insideLine) currentLineFloat.color.z = safeStringToFloat(value);
                    else if (insideBox) currentBoxFloat.color.z = safeStringToFloat(value);
                    else if (insidePoint) currentPointFloat.color.z = safeStringToFloat(value);
                } else {
                    if (insideLine) currentLine.color.z = safeStringToInt(value);
                    else if (insideBox) currentBox.color.z = safeStringToInt(value);
                    else if (insidePoint) currentPoint.color.z = safeStringToInt(value);
                }
            }

        }
    }

    file.close();
    return true;
}



