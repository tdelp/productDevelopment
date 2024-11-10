#include "gui.hpp"
#include <cmath>
#include <iostream>

LineElement::LineElement(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color)
    : start(start), end(end), color(color) {}

void LineElement::draw(Screen& screen) const {
    screen.drawSafeLine(
        ivec2(static_cast<int>(std::round(start[0])), static_cast<int>(std::round(start[1]))),
        ivec2(static_cast<int>(std::round(end[0])), static_cast<int>(std::round(end[1]))),
        ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
    );
}

void LineElement::writeToFile(std::ofstream& outputFile) const {
    outputFile << "  <line>\n";
    outputFile << "    <vec2><x>" << start[0] << "</x><y>" << start[1] << "</y></vec2>\n";
    outputFile << "    <vec2><x>" << end[0] << "</x><y>" << end[1] << "</y></vec2>\n";
    outputFile << "    <vec3><x>" << color[0] << "</x><y>" << color[1] << "</y><z>" << color[2] << "</z></vec3>\n";
    outputFile << "  </line>\n";
}

BoxElement::BoxElement(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color)
    : min(min), max(max), color(color) {}

void BoxElement::draw(Screen& screen) const {
    screen.drawSafeBox(
        ivec2(static_cast<int>(std::round(min[0])), static_cast<int>(std::round(min[1]))),
        ivec2(static_cast<int>(std::round(max[0])), static_cast<int>(std::round(max[1]))),
        ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
    );
}
bool BoxElement::isInside(const ivec2& point) const {
    return (point.x >= min[0] && point.x <= max[0] && point.y >= min[1] && point.y <= max[1]);
}

void BoxElement::writeToFile(std::ofstream& outputFile) const {
    outputFile << "  <box>\n";
    outputFile << "    <vec2><x>" << min[0] << "</x><y>" << min[1] << "</y></vec2>\n";
    outputFile << "    <vec2><x>" << max[0] << "</x><y>" << max[1] << "</y></vec2>\n";
    outputFile << "    <vec3><x>" << color[0] << "</x><y>" << color[1] << "</y><z>" << color[2] << "</z></vec3>\n";
    outputFile << "  </box>\n";
}

PointElement::PointElement(const std::array<float, 2>& position, const std::array<float, 3>& color)
    : position(position), color(color) {}

void PointElement::draw(Screen& screen) const {
    screen.setSafePixel(
        ivec2(static_cast<int>(std::round(position[0])), static_cast<int>(std::round(position[1]))),
        ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
    );
}
bool PointElement::isInside(const ivec2& point) const {
    return (point.x == position[0] && point.y == position[1]);
}

void PointElement::writeToFile(std::ofstream& outputFile) const {
    outputFile << "  <point>\n";
    outputFile << "    <vec2><x>" << position[0] << "</x><y>" << position[1] << "</y></vec2>\n";
    outputFile << "    <vec3><x>" << color[0] << "</x><y>" << color[1] << "</y><z>" << color[2] << "</z></vec3>\n";
    outputFile << "  </point>\n";
}

TriangleElement::TriangleElement(const std::array<float, 2>& v0, const std::array<float, 2>& v1, const std::array<float, 2>& v2, const std::array<float, 3>& color)
    : v0(v0), v1(v1), v2(v2), color(color) {}

void TriangleElement::draw(Screen& screen) const {
    screen.drawSafeTriangle(
        ivec2(static_cast<int>(std::round(v0[0])), static_cast<int>(std::round(v0[1]))),
        ivec2(static_cast<int>(std::round(v1[0])), static_cast<int>(std::round(v1[1]))),
        ivec2(static_cast<int>(std::round(v2[0])), static_cast<int>(std::round(v2[1]))),
        ivec3(static_cast<int>(std::round(color[0])), static_cast<int>(std::round(color[1])), static_cast<int>(std::round(color[2])))
    );
}

void TriangleElement::writeToFile(std::ofstream& outputFile) const {
    outputFile << "  <triangle>\n";
    outputFile << "    <vec2><x>" << v0[0] << "</x><y>" << v0[1] << "</y></vec2>\n";
    outputFile << "    <vec2><x>" << v1[0] << "</x><y>" << v1[1] << "</y></vec2>\n";
    outputFile << "    <vec2><x>" << v2[0] << "</x><y>" << v2[1] << "</y></vec2>\n";
    outputFile << "    <vec3><x>" << color[0] << "</x><y>" << color[1] << "</y><z>" << color[2] << "</z></vec3>\n";
    outputFile << "  </triangle>\n";
}
bool TriangleElement::isInside(const ivec2& point) const {
    // Using cross-product method to check if point is inside triangle
    auto sign = [](const ivec2& p1, const ivec2& p2, const ivec2& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    ivec2 pt(point.x, point.y);
    ivec2 pv0(static_cast<int>(v0[0]), static_cast<int>(v0[1]));
    ivec2 pv1(static_cast<int>(v1[0]), static_cast<int>(v1[1]));
    ivec2 pv2(static_cast<int>(v2[0]), static_cast<int>(v2[1]));

    bool b1 = sign(pt, pv0, pv1) < 0.0f;
    bool b2 = sign(pt, pv1, pv2) < 0.0f;
    bool b3 = sign(pt, pv2, pv0) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

std::unique_ptr<Element> ElementFactory::createLine(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color) {
    return std::make_unique<LineElement>(start, end, color);
}

std::unique_ptr<Element> ElementFactory::createBox(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color) {
    return std::make_unique<BoxElement>(min, max, color);
}

std::unique_ptr<Element> ElementFactory::createPoint(const std::array<float, 2>& position, const std::array<float, 3>& color) {
    return std::make_unique<PointElement>(position, color);
}

std::unique_ptr<Element> ElementFactory::createTriangle(const std::array<float, 2>& v0, const std::array<float, 2>& v1, const std::array<float, 2>& v2, const std::array<float, 3>& color) {
    return std::make_unique<TriangleElement>(v0, v1, v2, color);
}