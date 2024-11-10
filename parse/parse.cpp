#include "parse.hpp"
#include <iostream>

std::string GUIFile::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::vector<std::string> GUIFile::extractValues(const std::string& line) {
    std::vector<std::string> values;
    std::regex valueRegex(R"(<[^>]+>([^<]+)<\/[^>]+>)");
    auto matches = std::sregex_iterator(line.begin(), line.end(), valueRegex);

    for (auto i = matches; i != std::sregex_iterator(); ++i) {
        values.push_back(trim(i->str(1)));
    }
    return values;
}

float GUIFile::safeStringToFloat(const std::string& value) {
    try {
        return std::stof(value);
    } catch (...) {
        std::cerr << "Conversion error: '" << value << "' is not a valid float.\n";
        return 0.0f;
    }
}

void GUIFile::parseVec2(std::istringstream& stream, std::array<float, 2>& vector) {
    std::string line;
    bool xFound = false, yFound = false;

    while (std::getline(stream, line)) {
        line = trim(line);
        std::vector<std::string> values = extractValues(line);

        for (const auto& value : values) {
            if (line.find("<x>") != std::string::npos && !xFound) {
                vector[0] = safeStringToFloat(value);
                xFound = true;
            } else if (line.find("<y>") != std::string::npos && !yFound) {
                vector[1] = safeStringToFloat(value);
                yFound = true;
            }
        }

        if (xFound && yFound) break;
    }
}

void GUIFile::parseVec3(std::istringstream& stream, std::array<float, 3>& vector) {
    std::string line;
    bool xFound = false, yFound = false, zFound = false;

    while (std::getline(stream, line)) {
        line = trim(line);
        std::vector<std::string> values = extractValues(line);

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

        if (xFound && yFound && zFound) break;
    }
}

void GUIFile::parseLineData(const std::string& data) {
    std::istringstream stream(data);
    std::array<float, 2> start, end;
    std::array<float, 3> color;

    parseVec2(stream, start);
    parseVec2(stream, end);
    parseVec3(stream, color);

    elements.push_back(ElementFactory::createLine(start, end, color));
}

void GUIFile::parseBoxData(const std::string& data) {
    std::istringstream stream(data);
    std::array<float, 2> min, max;
    std::array<float, 3> color;

    parseVec2(stream, min);
    parseVec2(stream, max);
    parseVec3(stream, color);

    elements.push_back(ElementFactory::createBox(min, max, color));
}

void GUIFile::parsePointData(const std::string& data) {
    std::istringstream stream(data);
    std::array<float, 2> position;
    std::array<float, 3> color;

    parseVec2(stream, position);
    parseVec3(stream, color);

    elements.push_back(ElementFactory::createPoint(position, color));
}

void GUIFile::parseTriangleData(const std::string& data) {
    std::istringstream stream(data);
    std::array<float, 2> v0, v1, v2;
    std::array<float, 3> color;

    parseVec2(stream, v0);
    parseVec2(stream, v1);
    parseVec2(stream, v2);
    parseVec3(stream, color);

    elements.push_back(ElementFactory::createTriangle(v0, v1, v2, color));
}

void GUIFile::openOutputFile() {
    outputFile.open("output.xml");
    if (outputFile.is_open()) {
        outputFile << "<layout>\n";
    } else {
        std::cerr << "Error opening output file for writing.\n";
    }
}

void GUIFile::closeOutputFile() {
    if (outputFile.is_open()) {
        outputFile << "</layout>\n";
        outputFile.close();
    }
}

bool GUIFile::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
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

void GUIFile::writeFile() {
    openOutputFile();
    for (const auto& element : elements) {
        element->writeToFile(outputFile);
    }
    closeOutputFile();
}

const std::vector<std::unique_ptr<Element>>& GUIFile::getElements() const {
    return elements;
}