#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include "all_headers.hpp"

enum class EventType { CLICK, SOUND, SHOW };

struct Event {
    EventType type;
    int x = 0, y = 0;  // Coordinates for CLICK and SHOW events
    std::string soundFile;  // Filename for SOUND events

    // Constructor for CLICK and SHOW events
    Event(EventType eventType, int xPos = 0, int yPos = 0)
        : type(eventType), x(xPos), y(yPos) {}

    // Constructor for SOUND events
    Event(EventType eventType, const std::string& file)
        : type(eventType), soundFile(file) {}
};

class ButtonElement : public Element {
    ivec2 position;
    ivec2 size;
    ivec3 color;
    bool clicked;
    bool hoverable;   // Specifies if the button should respond to hover events
    bool clickable;   // Specifies if the button should respond to click events

public:
    ButtonElement(const ivec2& pos, const ivec2& sz, const ivec3& clr, bool isHoverable = false, bool isClickable = true)
        : position(pos), size(sz), color(clr), clicked(false), hoverable(isHoverable), clickable(isClickable) {}

    // Method to handle CLICK events only if clickable is enabled
    bool handleEvent(const Event& event) {
        if (clickable && event.type == EventType::CLICK && wasClicked(event)) {
            clicked = !clicked;
            return true;
        }
        return false;
    }

    // Method to handle hover detection for SHOW events only if hoverable is enabled
    bool handleHover(const Event& event) const {
        return hoverable && event.type == EventType::SHOW && isInside({event.x, event.y});
    }

    bool wasClicked(const Event& event) const {
        return (event.type == EventType::CLICK && isInside({event.x, event.y}));
    }

    bool isInside(const ivec2& point) const override {
        return (point.x >= position.x && point.x <= position.x + size.x &&
                point.y >= position.y && point.y <= position.y + size.y);
    }

    void draw(Screen& screen, const ivec2& start, const ivec2& end) const override {
        ivec2 topLeft = position + start;
        ivec2 bottomRight = topLeft + size;
        screen.drawSafeBox(topLeft, bottomRight, color);
    }

    bool isHoverable() const { return hoverable; }
    bool isClickable() const { return clickable; }
};

#endif // EVENT_SYSTEM_HPP
