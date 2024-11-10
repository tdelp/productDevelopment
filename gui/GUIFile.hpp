#ifndef __GUIFILE_HPP__
#define __GUIFILE_HPP__

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <array>

// Define a structure for GUI elements: Line, Box, and Point
struct Line {
    std::array<float, 2> start;
    std::array<float, 2> end;
    std::array<float, 3> color;
};

struct Box {
    std::array<float, 2> min;
    std::array<float, 2> max;
    std::array<float, 3> color;
};

struct Point {
    std::array<float, 2> position;
    std::array<float, 3> color;
};

class GUIFile {
private:
    std::vector<Line> lines;
    std::vector<Box> boxes;
    std::vector<Point> points;
    std::ofstream outputFile;

    // Helper method to trim whitespace from a string
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    // Improved function to extract all values from a line with multiple tags
    std::vector<std::string> extractValues(const std::string& line) {
        std::vector<std::string> values;
        std::regex valueRegex(R"(<[^>]+>([^<]+)<\/[^>]+>)");
        auto begin = std::sregex_iterator(line.begin(), line.end(), valueRegex);
        auto end = std::sregex_iterator();

        for (auto i = begin; i != end; ++i) {
            std::smatch match = *i;
            values.push_back(trim(match.str(1))); // Extract the value inside the tags
        }

        return values;
    }

    // Safe conversion function for converting a string to a float
    float safeStringToFloat(const std::string& value) {
        try {
            return std::stof(value);
        } catch (const std::invalid_argument&) {
            std::cerr << "Conversion error: '" << value << "' is not a valid float." << std::endl;
            return 0.0f; // Default value in case of conversion failure
        }
    }

    // Improved method to parse a 2D vector element (vec2 or ivec2) with multi-line support
    void parseVec2(std::istringstream& stream, std::array<float, 2>& vector) {
        std::string line;
        bool xFound = false, yFound = false;

        while (std::getline(stream, line)) {
            line = trim(line);
            std::vector<std::string> values = extractValues(line);

            if (!values.empty()) {
                for (const auto& value : values) {
                    if (line.find("<x>") != std::string::npos && !xFound) {
                        vector[0] = safeStringToFloat(value);
                        xFound = true;
                    } else if (line.find("<y>") != std::string::npos && !yFound) {
                        vector[1] = safeStringToFloat(value);
                        yFound = true;
                    }
                }
            }

            // Handle cases where <x> or <y> might be on separate lines
            if (line.find("<x>") != std::string::npos && !xFound) {
                std::getline(stream, line);
                vector[0] = safeStringToFloat(trim(line));
                xFound = true;
            }
            if (line.find("<y>") != std::string::npos && !yFound) {
                std::getline(stream, line);
                vector[1] = safeStringToFloat(trim(line));
                yFound = true;
            }

            if (xFound && yFound) break;
        }
    }

    // Method to parse a 3D vector element (vec3 or ivec3) with multi-line support
    void parseVec3(std::istringstream& stream, std::array<float, 3>& vector) {
        std::string line;
        bool xFound = false, yFound = false, zFound = false;

        while (std::getline(stream, line)) {
            line = trim(line);
            std::vector<std::string> values = extractValues(line);

            if (!values.empty()) {
                for (const auto& value : values) {
                    if (line.find("<x>") != std::string::npos && !xFound) {
                        vector[0] = safeStringToFloat(value);
                        xFound = true;
                    } else if (line.find("<y>") != std::string::npos && !yFound) {
                        vector[1] = safeStringToFloat(value);
                        yFound = true;
                    } else if (line.find("<z>") != std::string::npos && !zFound) {
                        vector[2] = safeStringToFloat(value);
                        zFound = true;
                    }
                }
            }

            // Handle cases where <x>, <y>, or <z> might be on separate lines
            if (line.find("<x>") != std::string::npos && !xFound) {
                std::getline(stream, line);
                vector[0] = safeStringToFloat(trim(line));
                xFound = true;
            }
            if (line.find("<y>") != std::string::npos && !yFound) {
                std::getline(stream, line);
                vector[1] = safeStringToFloat(trim(line));
                yFound = true;
            }
            if (line.find("<z>") != std::string::npos && !zFound) {
                std::getline(stream, line);
                vector[2] = safeStringToFloat(trim(line));
                zFound = true;
            }

            if (xFound && yFound && zFound) break;
        }
    }

    // Function to parse and extract data for lines
    void parseLineData(const std::string& data) {
        std::istringstream stream(data);
        std::array<float, 2> start, end;
        std::array<float, 3> color;

        parseVec2(stream, start);
        parseVec2(stream, end);
        parseVec3(stream, color);

        addLine(start, end, color);
        writeLineElement(lines.back());
    }

    // Function to parse and extract data for boxes
    void parseBoxData(const std::string& data) {
        std::istringstream stream(data);
        std::array<float, 2> min, max;
        std::array<float, 3> color;

        parseVec2(stream, min);
        parseVec2(stream, max);
        parseVec3(stream, color);

        addBox(min, max, color);
        writeBoxElement(boxes.back());
    }

    // Function to parse and extract data for points
    void parsePointData(const std::string& data) {
        std::istringstream stream(data);
        std::array<float, 2> position;
        std::array<float, 3> color;

        parseVec2(stream, position);
        parseVec3(stream, color);

        addPoint(position, color);
        writePointElement(points.back());
    }

// Functions to write elements to the output file
void writeLineElement(const Line& line) {
    if (outputFile.is_open()) {
        outputFile << "  <line>" << std::endl;
        outputFile << "    <vec2>" << std::endl;
        outputFile << "      <x>" << line.start[0] << "</x>" << std::endl;
        outputFile << "      <y>" << line.start[1] << "</y>" << std::endl;
        outputFile << "    </vec2>" << std::endl;
        outputFile << "    <vec2>" << std::endl;
        outputFile << "      <x>" << line.end[0] << "</x>" << std::endl;
        outputFile << "      <y>" << line.end[1] << "</y>" << std::endl;
        outputFile << "    </vec2>" << std::endl;
        outputFile << "    <vec3>" << std::endl;
        outputFile << "      <x>" << line.color[0] << "</x>" << std::endl;
        outputFile << "      <y>" << line.color[1] << "</y>" << std::endl;
        outputFile << "      <z>" << line.color[2] << "</z>" << std::endl;
        outputFile << "    </vec3>" << std::endl;
        outputFile << "  </line>" << std::endl;
    }
}

void writeBoxElement(const Box& box) {
    if (outputFile.is_open()) {
        outputFile << "  <box>" << std::endl;
        outputFile << "    <vec2>" << std::endl;
        outputFile << "      <x>" << box.min[0] << "</x>" << std::endl;
        outputFile << "      <y>" << box.min[1] << "</y>" << std::endl;
        outputFile << "    </vec2>" << std::endl;
        outputFile << "    <vec2>" << std::endl;
        outputFile << "      <x>" << box.max[0] << "</x>" << std::endl;
        outputFile << "      <y>" << box.max[1] << "</y>" << std::endl;
        outputFile << "    </vec2>" << std::endl;
        outputFile << "    <vec3>" << std::endl;
        outputFile << "      <x>" << box.color[0] << "</x>" << std::endl;
        outputFile << "      <y>" << box.color[1] << "</y>" << std::endl;
        outputFile << "      <z>" << box.color[2] << "</z>" << std::endl;
        outputFile << "    </vec3>" << std::endl;
        outputFile << "  </box>" << std::endl;
    }
}

void writePointElement(const Point& point) {
    if (outputFile.is_open()) {
        outputFile << "  <point>" << std::endl;
        outputFile << "    <vec2>" << std::endl;
        outputFile << "      <x>" << point.position[0] << "</x>" << std::endl;
        outputFile << "      <y>" << point.position[1] << "</y>" << std::endl;
        outputFile << "    </vec2>" << std::endl;
        outputFile << "    <vec3>" << std::endl;
        outputFile << "      <x>" << point.color[0] << "</x>" << std::endl;
        outputFile << "      <y>" << point.color[1] << "</y>" << std::endl;
        outputFile << "      <z>" << point.color[2] << "</z>" << std::endl;
        outputFile << "    </vec3>" << std::endl;
        outputFile << "  </point>" << std::endl;
    }
}


    // Methods to open and close the output file
    void openOutputFile() {
        outputFile.open("output.xml");
        if (outputFile.is_open()) {
            outputFile << "<layout>" << std::endl;
        } else {
            std::cerr << "Error opening output file for writing." << std::endl;
        }
    }

    void closeOutputFile() {
        if (outputFile.is_open()) {
            outputFile << "</layout>" << std::endl;
            outputFile.close();
        }
    }

public:
    // Setter methods for staging GUI elements programmatically
    void addLine(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color) {
        lines.push_back({start, end, color});
    }

    void addBox(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color) {
        boxes.push_back({min, max, color});
    }

    void addPoint(const std::array<float, 2>& position, const std::array<float, 3>& color) {
        points.push_back({position, color});
    }

    // Method to read and parse the XML file in chunks
    bool readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return false;
        }

        openOutputFile();

        std::string line;
        std::string elementData;
        bool capturing = false;
        std::string currentElement;

        while (std::getline(file, line)) {
            line = trim(line);

            if (line == "<line>" || line == "<box>" || line == "<point>") {
                capturing = true;
                currentElement = line;
                elementData.clear();
                elementData += line + "\n";
            }
            else if (line == "</line>" || line == "</box>" || line == "</point>") {
                capturing = false;
                elementData += line + "\n";

                if (currentElement == "<line>") {
                    parseLineData(elementData);
                } else if (currentElement == "<box>") {
                    parseBoxData(elementData);
                } else if (currentElement == "<point>") {
                    parsePointData(elementData);
                }
            }
            else if (capturing) {
                elementData += line + "\n";
            }
        }

        closeOutputFile();
        return true;
    }

    // Getters for accessing stored elements
    const std::vector<Line>& getLines() const { return lines; }
    const std::vector<Box>& getBoxes() const { return boxes; }
    const std::vector<Point>& getPoints() const { return points; }
};

#endif // __GUIFILE_HPP__
