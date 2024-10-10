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
        } catch (const std::invalid_argument& e) {
            std::cerr << "Conversion error: '" << value << "' is not a valid float." << std::endl;
            return 0.0f; // Default value in case of conversion failure
        }
    }

    // Method to parse a 2D vector element (vec2 or ivec2)
    void parseVec2(std::ifstream& file, std::array<float, 2>& vector) {
        std::string line;
        bool xFound = false, yFound = false; // Track if we have parsed x and y values

        while (std::getline(file, line)) {
            line = trim(line);
            std::vector<std::string> values = extractValues(line);

            if (!values.empty()) {
                for (const auto& value : values) {
                    if (line.find("<x>") != std::string::npos && !xFound) {
                        vector[0] = safeStringToFloat(value);
                        std::cout << "Parsed vec2 x: " << vector[0] << std::endl;
                        xFound = true;
                    } else if (line.find("<y>") != std::string::npos && !yFound) {
                        vector[1] = safeStringToFloat(value);
                        std::cout << "Parsed vec2 y: " << vector[1] << std::endl;
                        yFound = true;
                    }
                }
            }

            if (xFound && yFound) break; // Exit loop once both x and y are parsed
        }
    }

    // Method to parse a 3D vector element (vec3 or ivec3)

        void parseVec3(std::ifstream& file, std::array<float, 3>& vector) {
            std::string line;
            bool xFound = false, yFound = false, zFound = false; // Track if we have parsed x, y, and z values

            while (std::getline(file, line)) {
                line = trim(line);
                std::vector<std::string> values = extractValues(line);

                if (!values.empty()) {
                    for (const auto& value : values) {
                        if (line.find("<x>") != std::string::npos && !xFound) {
                            vector[0] = safeStringToFloat(value);
                            std::cout << "Parsed vec3 x: " << vector[0] << std::endl;
                            xFound = true;
                        } else if (line.find("<y>") != std::string::npos && !yFound) {
                            vector[1] = safeStringToFloat(value);
                            std::cout << "Parsed vec3 y: " << vector[1] << std::endl;
                            yFound = true;
                        } else if (line.find("<z>") != std::string::npos && !zFound) {
                            vector[2] = safeStringToFloat(value);
                            std::cout << "Parsed vec3 z: " << vector[2] << std::endl;
                            zFound = true;
                        }
                    }
                }

                // Handle cases where <x>, <y>, <z> might be on separate lines
                if (line.find("<x>") != std::string::npos && !xFound && !values.empty()) {
                    vector[0] = safeStringToFloat(values[0]);
                    std::cout << "Parsed vec3 x: " << vector[0] << std::endl;
                    xFound = true;
                }
                if (line.find("<y>") != std::string::npos && !yFound && !values.empty()) {
                    vector[1] = safeStringToFloat(values[0]);
                    std::cout << "Parsed vec3 y: " << vector[1] << std::endl;
                    yFound = true;
                }
                if (line.find("<z>") != std::string::npos && !zFound && !values.empty()) {
                    vector[2] = safeStringToFloat(values[0]);
                    std::cout << "Parsed vec3 z: " << vector[2] << std::endl;
                    zFound = true;
                }

                if (xFound && yFound && zFound) break; // Exit loop once all values are parsed
            }
        }


    // Method to open the output file for incremental writing
    void openOutputFile() {
        outputFile.open("output.xml");
        if (outputFile.is_open()) {
            outputFile << "<layout>" << std::endl;
        } else {
            std::cerr << "Error opening output file for writing." << std::endl;
        }
    }

    // Method to close the output file
    void closeOutputFile() {
        if (outputFile.is_open()) {
            outputFile << "</layout>" << std::endl;
            outputFile.close();
        }
    }

    // Method to write a Line element to the output file
    void writeLineElement(const Line& line) {
        if (outputFile.is_open()) {
            outputFile << "  <line>" << std::endl;
            outputFile << "    <vec2><x>" << line.start[0] << "</x><y>" << line.start[1] << "</y></vec2>" << std::endl;
            outputFile << "    <vec2><x>" << line.end[0] << "</x><y>" << line.end[1] << "</y></vec2>" << std::endl;
            outputFile << "    <vec3><x>" << line.color[0] << "</x><y>" << line.color[1] << "</y><z>" << line.color[2] << "</z></vec3>" << std::endl;
            outputFile << "  </line>" << std::endl;
        }
    }

    // Method to write a Box element to the output file
    void writeBoxElement(const Box& box) {
        if (outputFile.is_open()) {
            outputFile << "  <box>" << std::endl;
            outputFile << "    <vec2><x>" << box.min[0] << "</x><y>" << box.min[1] << "</y></vec2>" << std::endl;
            outputFile << "    <vec2><x>" << box.max[0] << "</x><y>" << box.max[1] << "</y></vec2>" << std::endl;
            outputFile << "    <vec3><x>" << box.color[0] << "</x><y>" << box.color[1] << "</y><z>" << box.color[2] << "</z></vec3>" << std::endl;
            outputFile << "  </box>" << std::endl;
        }
    }

    // Method to write a Point element to the output file
    void writePointElement(const Point& point) {
        if (outputFile.is_open()) {
            outputFile << "  <point>" << std::endl;
            outputFile << "    <vec2><x>" << point.position[0] << "</x><y>" << point.position[1] << "</y></vec2>" << std::endl;
            outputFile << "    <vec3><x>" << point.color[0] << "</x><y>" << point.color[1] << "</y><z>" << point.color[2] << "</z></vec3>" << std::endl;
            outputFile << "  </point>" << std::endl;
        }
    }

    // Method to parse a Line element
    void parseLineElement(std::ifstream& file) {
        std::string line;
        std::array<float, 2> start, end;
        std::array<float, 3> color;

        while (std::getline(file, line)) {
            line = trim(line);
            if (line.find("<vec2>") != std::string::npos || line.find("<ivec2>") != std::string::npos) {
                parseVec2(file, start);
                std::getline(file, line);  // move to next vec2 or ivec2
                parseVec2(file, end);
            } else if (line.find("<vec3>") != std::string::npos || line.find("<ivec3>") != std::string::npos) {
                parseVec3(file, color);
            } else if (line.find("</line>") != std::string::npos) {
                break;
            }
        }
        lines.push_back({start, end, color});
        writeLineElement(lines.back());  // Write the line element to the file
    }

    // Method to parse a Box element
    void parseBoxElement(std::ifstream& file) {
        std::string line;
        std::array<float, 2> min, max;
        std::array<float, 3> color;

        while (std::getline(file, line)) {
            line = trim(line);
            if (line.find("<vec2>") != std::string::npos || line.find("<ivec2>") != std::string::npos) {
                parseVec2(file, min);
                std::getline(file, line);  // move to next vec2 or ivec2
                parseVec2(file, max);
            } else if (line.find("<vec3>") != std::string::npos || line.find("<ivec3>") != std::string::npos) {
                parseVec3(file, color);
            } else if (line.find("</box>") != std::string::npos) {
                break;
            }
        }
        boxes.push_back({min, max, color});
        writeBoxElement(boxes.back());  // Write the box element to the file
    }

    // Method to parse a Point element
    void parsePointElement(std::ifstream& file) {
        std::string line;
        std::array<float, 2> position;
        std::array<float, 3> color;

        while (std::getline(file, line)) {
            line = trim(line);
            if (line.find("<vec2>") != std::string::npos || line.find("<ivec2>") != std::string::npos) {
                parseVec2(file, position);
            } else if (line.find("<vec3>") != std::string::npos || line.find("<ivec3>") != std::string::npos) {
                parseVec3(file, color);
            } else if (line.find("</point>") != std::string::npos) {
                break;
            }
        }
        points.push_back({position, color});
        writePointElement(points.back());  // Write the point element to the file
    }

public:
    // Method to read and parse the XML file manually
    bool readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return false;
        }

        openOutputFile();  // Open the output file for incremental writing

        std::string line;
        while (std::getline(file, line)) {
            line = trim(line);
            if (line.find("<line>") != std::string::npos) {
                parseLineElement(file);
            } else if (line.find("<box>") != std::string::npos) {
                parseBoxElement(file);
            } else if (line.find("<point>") != std::string::npos) {
                parsePointElement(file);
            }
        }

        closeOutputFile();  // Close the output file
        return true;
    }

    // Getters for accessing stored elements
    const std::vector<Line>& getLines() const { return lines; }
    const std::vector<Box>& getBoxes() const { return boxes; }
    const std::vector<Point>& getPoints() const { return points; }
};

#endif // __GUIFILE_HPP__
