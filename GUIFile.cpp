#include <fstream>
#include "GUIFile.hpp"
#include "XMLParser.hpp"

// Constructor and Destructor
GUIFile::GUIFile() {
    // Constructor logic (if needed)
}

GUIFile::~GUIFile() {
    // Destructor logic (if needed)
}

// Integer-based methods
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

// Floating-point-based methods
void GUIFile::addLineFloat(LineFloat lineFloat) {
    linesFloat.push_back(lineFloat);
}

void GUIFile::addBoxFloat(BoxFloat boxFloat) {
    boxesFloat.push_back(boxFloat);
}

void GUIFile::addPointFloat(PointFloat pointFloat) {
    pointsFloat.push_back(pointFloat);
}

const std::vector<LineFloat>& GUIFile::getLinesFloat() const {
    return linesFloat;
}

const std::vector<BoxFloat>& GUIFile::getBoxesFloat() const {
    return boxesFloat;
}

const std::vector<PointFloat>& GUIFile::getPointsFloat() const {
    return pointsFloat;
}

// XML file reading and writing
bool GUIFile::writeToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "<layout>\n";

    // Write integer-based lines
    for (const auto& line : lines) {
        file << "  <line>\n"
             << "    <ivec2><x>" << line.start.x << "</x><y>" << line.start.y << "</y></ivec2>\n"
             << "    <ivec2><x>" << line.end.x << "</x><y>" << line.end.y << "</y></ivec2>\n"
             << "    <ivec3><x>" << line.color.x << "</x><y>" << line.color.y << "</y><z>" << line.color.z << "</z></ivec3>\n"
             << "  </line>\n";
    }

    // Write floating-point-based lines
    for (const auto& lineFloat : linesFloat) {
        file << "  <line>\n"
             << "    <vec2><x>" << lineFloat.start.x << "</x><y>" << lineFloat.start.y << "</y></vec2>\n"
             << "    <vec2><x>" << lineFloat.end.x << "</x><y>" << lineFloat.end.y << "</y></vec2>\n"
             << "    <vec3><x>" << lineFloat.color.x << "</x><y>" << lineFloat.color.y << "</y><z>" << lineFloat.color.z << "</z></vec3>\n"
             << "  </line>\n";
    }

    // Similar writing logic for boxes and points (both integer and floating-point-based)

    file << "</layout>\n";
    file.close();
    return true;
}

bool GUIFile::readFromFile(const std::string& filename) {
    lines.clear();
    boxes.clear();
    points.clear();
    linesFloat.clear();
    boxesFloat.clear();
    pointsFloat.clear();
    return XMLParser::parseXML(filename, lines, boxes, points, linesFloat, boxesFloat, pointsFloat);
}
