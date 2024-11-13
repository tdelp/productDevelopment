#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include "all_headers.hpp"

enum class EventType { CLICK };

struct Event {
    EventType type;
    int x, y;  // Coordinates for CLICK events

    Event(EventType eventType, int xPos = 0, int yPos = 0)
        : type(eventType), x(xPos), y(yPos) {}
};

// ButtonElement class representing a clickable button
class ButtonElement : public Element {
    ivec2 position;  // Position of the button
    ivec2 size;      // Size of the button
    ivec3 color;     // Color of the button
    bool clicked;

public:
    ButtonElement(const ivec2& pos, const ivec2& sz, const ivec3& clr)
        : position(pos), size(sz), color(clr), clicked(false) {}

    // Method to handle CLICK events
    bool handleEvent(const Event& event) {
        if (wasClicked(event)) {
            clicked = !clicked;
            return true;
        }
        return false;
    }

    bool wasClicked(const Event& event) const {
        return (event.type == EventType::CLICK && isInside({event.x, event.y}));
    }

    bool isClicked() const { return clicked; }

    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override {
        ivec2 topLeft = position + start;
        ivec2 bottomRight = topLeft + size;
        screen.drawSafeBox(topLeft, bottomRight, color);
    }

    bool isInside(const ivec2& point) const override {
        return (point.x >= position.x && point.x <= position.x + size.x &&
                point.y >= position.y && point.y <= position.y + size.y);
    }
};

#endif // EVENT_SYSTEM_HPP
