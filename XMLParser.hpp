#ifndef __XMLPARSER_HPP__
#define __XMLPARSER_HPP__

#include <string>
#include <vector>
#include "GUIFile.hpp"

class XMLParser {
public:
    static bool parseXML(const std::string& filename, std::vector<Line>& lines, std::vector<Box>& boxes, std::vector<Point>& points,
                         std::vector<LineFloat>& linesFloat, std::vector<BoxFloat>& boxesFloat, std::vector<PointFloat>& pointsFloat);
};

#endif // __XMLPARSER_HPP__
