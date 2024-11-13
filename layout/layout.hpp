// Layout.hpp
#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "../all_headers.hpp"
#include "../EventSystem.hpp"

class Layout {
public:
    Layout(float startX, float startY, float endX, float endY, bool isActive = true)
        : sX(startX), sY(startY), eX(endX), eY(endY), active(isActive) {}

    void addElement(std::unique_ptr<Element> element);
    void addNestedLayout(std::unique_ptr<Layout> layout);
    void setActive(bool state) { active = state; }
    
    bool isActive() const { return active; }

    void calculatePosition(const ivec2& parentStart, const ivec2& parentEnd);
    void render(Screen& screen);
    void handleEvent(const Event& event); // Handle events directly without needing names

    const ivec2& getStart() const { return start; }
    const ivec2& getEnd() const { return end; }
    const std::vector<std::unique_ptr<Element>>& getElements() const { return elements; }
    const std::vector<std::unique_ptr<Layout>>& getNestedLayouts() const { return nestedLayouts; }

private:
    float sX, sY, eX, eY;
    bool active;
    ivec2 start, end;
    std::vector<std::unique_ptr<Element>> elements;
    std::vector<std::unique_ptr<Layout>> nestedLayouts;
};

#endif // LAYOUT_HPP
