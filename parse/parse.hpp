#ifndef __PARSE_HPP__
#define __PARSE_HPP__

#include "../gui/gui.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class Parser {
public:
    Parser(const std::string& fileName);
    std::unique_ptr<Layout> parseRootLayout();

private:
    std::string data;  // The entire XML content in a single string for easy parsing
    void loadFile(const std::string& fileName);
    
    // Parse methods
    std::unique_ptr<Layout> parseLayout(size_t& pos);
    std::unique_ptr<Element> parseElement(const std::string& type, size_t& pos);
    
    // Helper methods to parse specific data
    std::array<float, 2> parseVec2(size_t& pos);
    std::array<float, 3> parseVec3(size_t& pos);
    bool parseBooleanAttribute(const std::string& attributeName, size_t pos, bool defaultValue);
    
    size_t findTag(const std::string& tag, size_t pos);
};

#endif // __PARSE_HPP__
