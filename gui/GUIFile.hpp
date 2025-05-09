#ifndef __GUI_HPP__
#define __GUI_HPP__

#include "../all_headers.hpp"

// Abstract Base Class for all GUI Elements
class Element {
public:
    virtual ~Element() = default;
    virtual void draw(Screen& screen, const ivec2& start, const ivec2& end) const = 0;
    virtual bool isInside(const ivec2& point) const = 0; // Check if a point is inside the element
};

// Concrete Element classes
class LineElement : public Element {
    std::array<float, 2> start, end;
    std::array<float, 3> color;
public:
    LineElement(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override;
    bool isInside(const ivec2& point) const override { return false; } // Lines are not considered "inside"
};

class BoxElement : public Element {
    std::array<float, 2> min, max;
    std::array<float, 3> color;
public:
    BoxElement(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override;
    bool isInside(const ivec2& point) const override;
};

class PointElement : public Element {
    std::array<float, 2> position;
    std::array<float, 3> color;
public:
    PointElement(const std::array<float, 2>& position, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override;
    bool isInside(const ivec2& point) const override;
};

class TriangleElement : public Element {
    std::array<float, 2> v0, v1, v2;
    std::array<float, 3> color;
public:
    TriangleElement(const std::array<float, 2>& v0, const std::array<float, 2>& v1, const std::array<float, 2>& v2, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override;
    bool isInside(const ivec2& point) const override;
};

// Factory Class for Creating Elements
class ElementFactory {
public:
    static std::unique_ptr<Element> createLine(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color);
    static std::unique_ptr<Element> createBox(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color);
    static std::unique_ptr<Element> createPoint(const std::array<float, 2>& position, const std::array<float, 3>& color);
    static std::unique_ptr<Element> createTriangle(const std::array<float, 2>& v0, const std::array<float, 2>& v1, const std::array<float, 2>& v2, const std::array<float, 3>& color);
};

#endif // __GUI_HPP__
