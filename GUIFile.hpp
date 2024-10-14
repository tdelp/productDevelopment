#ifndef __GUIFILE_HPP__
#define __GUIFILE_HPP__

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <regex>
#include <iostream>
#include "Screen.hpp"

// Abstract base class for GUI elements
class Element {
public:
    virtual ~Element() = default;
    virtual void draw(Screen& screen) const = 0;
    virtual void writeToFile(std::ofstream& outputFile) const = 0;
};

// Concrete element classes: Line, Box, Point, Triangle
class LineElement : public Element { /* ... */ };
class BoxElement : public Element { /* ... */ };
class PointElement : public Element { /* ... */ };
class TriangleElement : public Element { /* ... */ };

// GUIFile Class: Handles Parsing and Factory Logic
class GUIFile {
private:
    std::vector<std::unique_ptr<Element>> elements;
    
    // Parsing functions
    void parseLine(const std::string& data);
    void parseBox(const std::string& data);
    void parsePoint(const std::string& data);
    void parseTriangle(const std::string& data);

public:
    bool readFile(const std::string& filename); // Parses input file
    void writeFile(const std::string& filename); // Outputs to file
    const std::vector<std::unique_ptr<Element>>& getElements() const;
};

#endif // __GUIFILE_HPP__
