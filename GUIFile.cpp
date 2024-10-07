#include "GUIFile.hpp"

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