#ifndef __PARSE_HPP__
#define __PARSE_HPP__

#include "gui.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <memory>

class GUIFile {
    std::vector<std::unique_ptr<Element>> elements;
    std::ofstream outputFile;

    std::string trim(const std::string& str);
    std::vector<std::string> extractValues(const std::string& line);
    float safeStringToFloat(const std::string& value);

    void parseVec2(std::istringstream& stream, std::array<float, 2>& vector);
    void parseVec3(std::istringstream& stream, std::array<float, 3>& vector);
    void parseLineData(const std::string& data);
    void parseBoxData(const std::string& data);
    void parsePointData(const std::string& data);
    void parseTriangleData(const std::string& data);

    void openOutputFile();
    void closeOutputFile();

public:
    bool readFile(const std::string& filename);
    void writeFile();
    const std::vector<std::unique_ptr<Element>>& getElements() const;
};

#endif // __PARSE_HPP__