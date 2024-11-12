#include "../all_headers.hpp"

// Implementation of LineElement
LineElement::LineElement(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color)
    : start(start), end(end), color(color) {}

void LineElement::draw(Screen& screen, const ivec2& offset, const ivec2& limit) const {
    ivec2 startPoint(static_cast<int>(std::round(start[0])) + offset.x, static_cast<int>(std::round(start[1])) + offset.y);
    ivec2 endPoint(static_cast<int>(std::round(end[0])) + offset.x, static_cast<int>(std::round(end[1])) + offset.y);

    // Ensure line is within the bounds of offset and limit
    if ((startPoint.x < offset.x || startPoint.y < offset.y || startPoint.x > limit.x || startPoint.y > limit.y) &&
        (endPoint.x < offset.x || endPoint.y < offset.y || endPoint.x > limit.x || endPoint.y > limit.y)) {
        return;
    }

    screen.drawSafeLine(startPoint, endPoint, ivec3(color[0], color[1], color[2]));
}

// Implementation of BoxElement
BoxElement::BoxElement(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color)
    : min(min), max(max), color(color) {}

void BoxElement::draw(Screen& screen, const ivec2& offset, const ivec2& limit) const {
    ivec2 minPoint(static_cast<int>(std::round(min[0])) + offset.x, static_cast<int>(std::round(min[1])) + offset.y);
    ivec2 maxPoint(static_cast<int>(std::round(max[0])) + offset.x, static_cast<int>(std::round(max[1])) + offset.y);

    minPoint.x = std::max(minPoint.x, offset.x);
    minPoint.y = std::max(minPoint.y, offset.y);
    maxPoint.x = std::min(maxPoint.x, limit.x);
    maxPoint.y = std::min(maxPoint.y, limit.y);

    screen.drawSafeBox(minPoint, maxPoint, ivec3(color[0], color[1], color[2]));
}

bool BoxElement::isInside(const ivec2& point) const {
    return (point.x >= min[0] && point.x <= max[0] && point.y >= min[1] && point.y <= max[1]);
}

// Implementation of PointElement
PointElement::PointElement(const std::array<float, 2>& position, const std::array<float, 3>& color)
    : position(position), color(color) {}

void PointElement::draw(Screen& screen, const ivec2& offset, const ivec2& limit) const {
    ivec2 point(static_cast<int>(std::round(position[0])) + offset.x, static_cast<int>(std::round(position[1])) + offset.y);

    if (point.x >= offset.x && point.y >= offset.y && point.x <= limit.x && point.y <= limit.y) {
        screen.setSafePixel(point, ivec3(color[0], color[1], color[2]));
    }
}

bool PointElement::isInside(const ivec2& point) const {
    return (point.x == position[0] && point.y == position[1]);
}

// Implementation of TriangleElement
TriangleElement::TriangleElement(const std::array<float, 2>& v0, const std::array<float, 2>& v1, const std::array<float, 2>& v2, const std::array<float, 3>& color)
    : v0(v0), v1(v1), v2(v2), color(color) {}

void TriangleElement::draw(Screen& screen, const ivec2& offset, const ivec2& limit) const {
    ivec2 v0Point(static_cast<int>(std::round(v0[0])) + offset.x, static_cast<int>(std::round(v0[1])) + offset.y);
    ivec2 v1Point(static_cast<int>(std::round(v1[0])) + offset.x, static_cast<int>(std::round(v1[1])) + offset.y);
    ivec2 v2Point(static_cast<int>(std::round(v2[0])) + offset.x, static_cast<int>(std::round(v2[1])) + offset.y);

    if ((v0Point.x >= offset.x && v0Point.y >= offset.y && v0Point.x <= limit.x && v0Point.y <= limit.y) ||
        (v1Point.x >= offset.x && v1Point.y >= offset.y && v1Point.x <= limit.x && v1Point.y <= limit.y) ||
        (v2Point.x >= offset.x && v2Point.y >= offset.y && v2Point.x <= limit.x && v2Point.y <= limit.y)) {
        screen.drawSafeTriangle(v0Point, v1Point, v2Point, ivec3(color[0], color[1], color[2]));
    }
}

bool TriangleElement::isInside(const ivec2& point) const {
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

// ElementFactory implementations
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
