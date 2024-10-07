#include <fstream>
#include "GUIFile.hpp"
#include "XMLParser.hpp"

// Constructor and Destructor
GUIFile::GUIFile() = default;
GUIFile::~GUIFile() = default;

// Integer-based methods
void GUIFile::addLine(Line line) { lines.push_back(line); }
void GUIFile::addBox(Box box) { boxes.push_back(box); }
void GUIFile::addPoint(Point point) { points.push_back(point); }

const std::vector<Line>& GUIFile::getLines() const { return lines; }
const std::vector<Box>& GUIFile::getBoxes() const { return boxes; }
const std::vector<Point>& GUIFile::getPoints() const { return points; }

// Floating-point-based methods
void GUIFile::addLineFloat(LineFloat lineFloat) { linesFloat.push_back(lineFloat); }
void GUIFile::addBoxFloat(BoxFloat boxFloat) { boxesFloat.push_back(boxFloat); }
void GUIFile::addPointFloat(PointFloat pointFloat) { pointsFloat.push_back(pointFloat); }

const std::vector<LineFloat>& GUIFile::getLinesFloat() const { return linesFloat; }
const std::vector<BoxFloat>& GUIFile::getBoxesFloat() const { return boxesFloat; }
const std::vector<PointFloat>& GUIFile::getPointsFloat() const { return pointsFloat; }

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

    // Write float-based lines
    for (const auto& lineFloat : linesFloat) {
        file << "  <line>\n"
             << "    <vec2><x>" << lineFloat.start.x << "</x><y>" << lineFloat.start.y << "</y></vec2>\n"
             << "    <vec2><x>" << lineFloat.end.x << "</x><y>" << lineFloat.end.y << "</y></vec2>\n"
             << "    <vec3><x>" << lineFloat.color.x << "</x><y>" << lineFloat.color.y << "</y><z>" << lineFloat.color.z << "</z></vec3>\n"
             << "  </line>\n";
    }

    // Write integer-based boxes
    for (const auto& box : boxes) {
        file << "  <box>\n"
             << "    <ivec2><x>" << box.min.x << "</x><y>" << box.min.y << "</y></ivec2>\n"
             << "    <ivec2><x>" << box.max.x << "</x><y>" << box.max.y << "</y></ivec2>\n"
             << "    <ivec3><x>" << box.color.x << "</x><y>" << box.color.y << "</y><z>" << box.color.z << "</z></ivec3>\n"
             << "  </box>\n";
    }

    // Write float-based boxes
    for (const auto& boxFloat : boxesFloat) {
        file << "  <box>\n"
             << "    <vec2><x>" << boxFloat.min.x << "</x><y>" << boxFloat.min.y << "</y></vec2>\n"
             << "    <vec2><x>" << boxFloat.max.x << "</x><y>" << boxFloat.max.y << "</y></vec2>\n"
             << "    <ivec3><x>" << static_cast<int>(boxFloat.color.x) << "</x><y>" << static_cast<int>(boxFloat.color.y) << "</y><z>" << static_cast<int>(boxFloat.color.z) << "</z></ivec3>\n"
             << "  </box>\n";
    }

    // Write integer-based points
    for (const auto& point : points) {
        file << "  <point>\n"
             << "    <ivec2><x>" << point.position.x << "</x><y>" << point.position.y << "</y></ivec2>\n"
             << "    <ivec3><x>" << point.color.x << "</x><y>" << point.color.y << "</y><z>" << point.color.z << "</z></ivec3>\n"
             << "  </point>\n";
    }

    // Write float-based points
    for (const auto& pointFloat : pointsFloat) {
        file << "  <point>\n"
             << "    <vec2><x>" << pointFloat.position.x << "</x><y>" << pointFloat.position.y << "</y></vec2>\n"
             << "    <vec3><x>" << pointFloat.color.x << "</x><y>" << pointFloat.color.y << "</y><z>" << pointFloat.color.z << "</z></vec3>\n"
             << "  </point>\n";
    }

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
