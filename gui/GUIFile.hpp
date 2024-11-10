#ifndef __GUI_HPP__
#define __GUI_HPP__

#include "../screen/Screen.hpp"
#include <array>
#include <memory>
#include <vector>

// Abstract Base Class for all GUI Elements
class Element {
public:
    virtual ~Element() = default;
    virtual void draw(Screen& screen, const ivec2& offset = ivec2(0, 0)) const = 0;
    virtual bool isInside(const ivec2& point) const = 0; // Check if a point is inside the element
};

// Concrete Element classes
class LineElement : public Element {
    std::array<float, 2> start, end;
    std::array<float, 3> color;
public:
    LineElement(const std::array<float, 2>& start, const std::array<float, 2>& end, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& offset = ivec2(0, 0)) const override;
    bool isInside(const ivec2& point) const override { return false; } // Lines are not considered "inside"
};

class BoxElement : public Element {
    std::array<float, 2> min, max;
    std::array<float, 3> color;
public:
    BoxElement(const std::array<float, 2>& min, const std::array<float, 2>& max, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& offset = ivec2(0, 0)) const override;
    bool isInside(const ivec2& point) const override;
};

class PointElement : public Element {
    std::array<float, 2> position;
    std::array<float, 3> color;
public:
    PointElement(const std::array<float, 2>& position, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& offset = ivec2(0, 0)) const override;
    bool isInside(const ivec2& point) const override;
};

class TriangleElement : public Element {
    std::array<float, 2> v0, v1, v2;
    std::array<float, 3> color;
public:
    TriangleElement(const std::array<float, 2>& v0, const std::array<float, 2>& v1, const std::array<float, 2>& v2, const std::array<float, 3>& color);
    void draw(Screen& screen, const ivec2& offset = ivec2(0, 0)) const override;
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

// Layout class to manage grouping of elements with optional nested layouts
class Layout {
public:
    Layout(float startX, float startY, float endX, float endY, bool isActive = true)
        : sX(startX), sY(startY), eX(endX), eY(endY), active(isActive) {}

    void addElement(std::unique_ptr<Element> element);
    void addNestedLayout(std::unique_ptr<Layout> layout);
    void setActive(bool state);

    void calculatePosition(const ivec2& parentStart, const ivec2& parentEnd);
    void render(Screen& screen);

    const ivec2& getStart() const { return start; }
    const ivec2& getEnd() const { return end; }
    bool isActive() const { return active; }

    const std::vector<std::unique_ptr<Element>>& getElements() const { return elements; }
    const std::vector<std::unique_ptr<Layout>>& getNestedLayouts() const { return nestedLayouts; }

private:
    float sX, sY, eX, eY;
    bool active;
    ivec2 start, end;
    std::vector<std::unique_ptr<Element>> elements;
    std::vector<std::unique_ptr<Layout>> nestedLayouts;
};

#endif // __GUI_HPP__