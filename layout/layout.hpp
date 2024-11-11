#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include "../screen/Screen.hpp"
#include "../gui/GUIFile.hpp"
#include <array>
#include <vector>
#include <memory>

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

#endif // __LAYOUT_HPP__
