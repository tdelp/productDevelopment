#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "../all_headers.hpp"
#include "../EventSystem.hpp"
#include "../SoundPlayer.hpp"

class Layout {
public:
    Layout(float startX, float startY, float endX, float endY, bool isActive = true, Layout* parent = nullptr)
        : sX(startX), sY(startY), eX(endX), eY(endY), active(isActive), parentLayout(parent) {}

    void addElement(std::unique_ptr<Element> element);
    void addNestedLayout(std::unique_ptr<Layout> layout);
    void setActive(bool state) { active = state; }
    bool isActive() const { return active; }

    void calculatePosition(const ivec2& parentStart, const ivec2& parentEnd);
    void render(Screen& screen);
    void handleEvent(const Event& event, SoundPlayer* soundPlayer);

private:
    float sX, sY, eX, eY;
    bool active;
    ivec2 start, end;
    Layout* parentLayout = nullptr;  // Pointer to parent layout for upward propagation
    std::vector<std::unique_ptr<Element>> elements;
    std::vector<std::unique_ptr<Layout>> nestedLayouts;
};

#endif // LAYOUT_HPP
