#include <fstream>
#include "GUIFile.hpp"
#include "XMLParser.hpp"

GUIFile::GUIFile() {
    // Constructor logic (if needed)
}

GUIFile::~GUIFile() {
    // Destructor logic (if needed)
}

void GUIFile::addLine(Line line) {
    lines.push_back(line);
}

void GUIFile::addBox(Box box) {
    boxes.push_back(box);
}

void GUIFile::addPoint(Point point) {
    points.push_back(point);
}

const std::vector<Line>& GUIFile::getLines() const {
    return lines;
}

const std::vector<Box>& GUIFile::getBoxes() const {
    return boxes;
}

const std::vector<Point>& GUIFile::getPoints() const {
    return points;
}

bool GUIFile::writeToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "<layout>\n";
    for (const auto& line : lines) {
        file << "  <line>\n"
             << "    <ivec2><x>" << line.start.x << "</x><y>" << line.start.y << "</y></ivec2>\n"
             << "    <ivec2><x>" << line.end.x << "</x><y>" << line.end.y << "</y></ivec2>\n"
             << "    <ivec3><x>" << line.color.x << "</x><y>" << line.color.y << "</y><z>" << line.color.z << "</z></ivec3>\n"
             << "  </line>\n";
    }
    file << "</layout>\n";
    file.close();
    return true;
}

bool GUIFile::readFromFile(const std::string& filename) {
    lines.clear();
    boxes.clear();
    points.clear();
    return XMLParser::parseXML(filename, lines, boxes, points);
}
