#ifndef __GUIFILE_HPP__
#define __GUIFILE_HPP__

#include "Screen.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <array>
#include <memory> // For smart pointers

// Forward declaration of Screen class
class Screen;

// Abstract Base Class for all GUI Elements
class Element {
public:
    virtual ~Element() = default; // Virtual destructor for proper cleanup
    virtual void draw(Screen& screen) const = 0; // Pure virtual draw function
    virtual void writeToFile(std::ofstream& outputFile) const = 0; // Virtual method to write to file
};

// Concrete Line Class
class LineElement : public Element {
private:
    std::array<float, 2> start;
    std::array<float, 2> end;
    std::array<float, 3> color;

public:
    LineElement(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color)
        : start(start), end(end), color(color) {}

    void draw(Screen& screen) const override {
        screen.drawSafeLine(
            ivec2(static_cast<int>(std::round(start[0])), static_cast<int>(std::round(start[1]))),
            ivec2(static_cast<int>(std::round(end[0])), static_cast<int>(std::round(end[1]))),
            ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
        );
    }

    void writeToFile(std::ofstream& outputFile) const override {
        outputFile << "  <line>\n";
        outputFile << "    <vec2>\n";
        outputFile << "      <x>" << start[0] << "</x>\n";
        outputFile << "      <y>" << start[1] << "</y>\n";
        outputFile << "    </vec2>\n";
        outputFile << "    <vec2>\n";
        outputFile << "      <x>" << end[0] << "</x>\n";
        outputFile << "      <y>" << end[1] << "</y>\n";
        outputFile << "    </vec2>\n";
        outputFile << "    <vec3>\n";
        outputFile << "      <x>" << color[0] << "</x>\n";
        outputFile << "      <y>" << color[1] << "</y>\n";
        outputFile << "      <z>" << color[2] << "</z>\n";
        outputFile << "    </vec3>\n";
        outputFile << "  </line>\n";
    }
};

// Concrete Box Class
class BoxElement : public Element {
private:
    std::array<float, 2> min;
    std::array<float, 2> max;
    std::array<float, 3> color;

public:
    BoxElement(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color)
        : min(min), max(max), color(color) {}

    void draw(Screen& screen) const override {
        screen.drawSafeBox(
            ivec2(static_cast<int>(std::round(min[0])), static_cast<int>(std::round(min[1]))),
            ivec2(static_cast<int>(std::round(max[0])), static_cast<int>(std::round(max[1]))),
            ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
        );
    }

    void writeToFile(std::ofstream& outputFile) const override {
        outputFile << "  <box>\n";
        outputFile << "    <vec2>\n";
        outputFile << "      <x>" << min[0] << "</x>\n";
        outputFile << "      <y>" << min[1] << "</y>\n";
        outputFile << "    </vec2>\n";
        outputFile << "    <vec2>\n";
        outputFile << "      <x>" << max[0] << "</x>\n";
        outputFile << "      <y>" << max[1] << "</y>\n";
        outputFile << "    </vec2>\n";
        outputFile << "    <vec3>\n";
        outputFile << "      <x>" << color[0] << "</x>\n";
        outputFile << "      <y>" << color[1] << "</y>\n";
        outputFile << "      <z>" << color[2] << "</z>\n";
        outputFile << "    </vec3>\n";
        outputFile << "  </box>\n";

    }
};

// Concrete Point Class
class PointElement : public Element {
private:
    std::array<float, 2> position;
    std::array<float, 3> color;

public:
    PointElement(const std::array<float, 2>& position, const std::array<float, 3>& color)
        : position(position), color(color) {}

    void draw(Screen& screen) const override {
        screen.setSafePixel(
            ivec2(static_cast<int>(std::round(position[0])), static_cast<int>(std::round(position[1]))),
            ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
        );
    }

    void writeToFile(std::ofstream& outputFile) const override {
        outputFile << "  <point>\n";
        outputFile << "    <vec2>\n";
        outputFile << "      <x>" << position[0] << "</x>\n";
        outputFile << "      <y>" << position[1] << "</y>\n";
        outputFile << "    </vec2>\n";
        outputFile << "    <vec3>\n";
        outputFile << "      <x>" << color[0] << "</x>\n";
        outputFile << "      <y>" << color[1] << "</y>\n";
        outputFile << "      <z>" << color[2] << "</z>\n";
        outputFile << "    </vec3>\n";
        outputFile << "  </point>\n";
    }
};

// Factory Class for Creating Elements
class ElementFactory {
public:
    static std::unique_ptr<Element> createLine(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color) {
        return std::make_unique<LineElement>(start, end, color);
    }

    static std::unique_ptr<Element> createBox(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color) {
        return std::make_unique<BoxElement>(min, max, color);
    }

    static std::unique_ptr<Element> createPoint(const std::array<float, 2>& position, const std::array<float, 3>& color) {
        return std::make_unique<PointElement>(position, color);
    }

};

// GUIFile Class for Parsing XML and Managing Elements
class GUIFile {
private:
    std::vector<std::unique_ptr<Element>> elements;
    std::ofstream outputFile;  // Add an output file stream for writing elements

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

            if (xFound && yFound && zFound) break;
        }
    }

    // Function to parse and extract data for line elements
    void parseLineData(const std::string& data) {
        std::istringstream stream(data);
        std::array<float, 2> start, end;
        std::array<float, 3> color;

        parseVec2(stream, start);
        parseVec2(stream, end);
        parseVec3(stream, color);

        elements.push_back(ElementFactory::createLine(start, end, color));
    }

    // Function to parse and extract data for box elements
    void parseBoxData(const std::string& data) {
        std::istringstream stream(data);
        std::array<float, 2> min, max;
        std::array<float, 3> color;

        parseVec2(stream, min);
        parseVec2(stream, max);
        parseVec3(stream, color);

        elements.push_back(ElementFactory::createBox(min, max, color));
    }

    // Function to parse and extract data for point elements
    void parsePointData(const std::string& data) {
        std::istringstream stream(data);
        std::array<float, 2> position;
        std::array<float, 3> color;

        parseVec2(stream, position);
        parseVec3(stream, color);

        elements.push_back(ElementFactory::createPoint(position, color));
    }

    // Methods to open and close the output file
    void openOutputFile() {
        outputFile.open("output.xml");
        if (outputFile.is_open()) {
            outputFile << "<layout>\n";
        } else {
            std::cerr << "Error opening output file for writing." << std::endl;
        }
    }

    void closeOutputFile() {
        if (outputFile.is_open()) {
            outputFile << "</layout>\n";
            outputFile.close();
        }
    }

public:
    // Method to read and parse the XML file
    bool readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::string elementData;
        bool capturing = false;
        std::string currentElement;

        while (std::getline(file, line)) {
            line = trim(line);

            if (line == "<line>" || line == "<box>" || line == "<point>" || line == "<triangle>") {
                capturing = true;
                currentElement = line;
                elementData.clear();
                elementData += line + "\n";
            }
            else if (line == "</line>" || line == "</box>" || line == "</point>" || line == "</triangle>") {
                capturing = false;
                elementData += line + "\n";

                if (currentElement == "<line>") {
                    parseLineData(elementData);
                } else if (currentElement == "<box>") {
                    parseBoxData(elementData);
                } else if (currentElement == "<point>") {
                    parsePointData(elementData);
                } else if (currentElement == "<triangle>") {
                    parseTriangleData(elementData);
                }
            }
            else if (capturing) {
                elementData += line + "\n";
            }
        }

        return true;
    }

    // Method to write all elements to output.xml
    void writeFile() {
        openOutputFile();
        for (const auto& element : elements) {
            element->writeToFile(outputFile);
        }
        closeOutputFile();
    }

    const std::vector<std::unique_ptr<Element>>& getElements() const {
        return elements;
    }
};

#endif // __GUIFILE_HPP__
