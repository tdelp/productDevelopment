// EventSystem.hpp
#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include "all_headers.hpp"

enum class EventType { CLICK, SHOW, SOUND };

struct Event {
    EventType type;
    int x, y;              // For Click events
    std::string target;    // For Show events (layout name)

    Event(EventType type, int x = 0, int y = 0, const std::string& target = "")
        : type(type), x(x), y(y), target(target) {}
};

// ButtonElement class inheriting from Element, using a box shape for simplicity
class ButtonElement : public Element {
    ivec2 position;        // Position of the button
    ivec2 size;            // Size of the button
    ivec3 color;           // Color of the button
    std::string targetLayout; // Name of the layout to show/hide
    bool clicked;

public:
    ButtonElement(const ivec2& position, const ivec2& size, const ivec3& color, const std::string& targetLayout)
        : position(position), size(size), color(color), targetLayout(targetLayout), clicked(false) {}

    // Method to handle events
    bool handleEvent(const Event& event) {
        if (event.type == EventType::CLICK &&
            event.x >= position.x && event.x <= position.x + size.x &&
            event.y >= position.y && event.y <= position.y + size.y) {
            clicked = !clicked;
            return true; // Signal that the click event was handled
        }
        return false;
    }

    bool isClicked() const { return clicked; }

    // Override the draw function to render the button on the screen
    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override {
        ivec2 topLeft = position + start;
        ivec2 bottomRight = topLeft + size;
        screen.drawSafeBox(topLeft, bottomRight, color);
    }

    // Override the isInside function to check if a point is within the button
    bool isInside(const ivec2& point) const override {
        return (point.x >= position.x && point.x <= position.x + size.x &&
                point.y >= position.y && point.y <= position.y + size.y);
    }
};

#endif // EVENT_SYSTEM_HPP
