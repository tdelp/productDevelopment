#include "../all_headers.hpp"

// Layout implementation
void Layout::addElement(std::unique_ptr<Element> element) {
    elements.push_back(std::move(element));
}

void Layout::addNestedLayout(std::unique_ptr<Layout> layout) {
    nestedLayouts.push_back(std::move(layout));
}

void Layout::setActive(bool state) { active = state; }

void Layout::calculatePosition(const ivec2& parentStart, const ivec2& parentEnd) {
    ivec2 space = parentEnd - parentStart;
    start = ivec2(static_cast<int>(sX * space.x), static_cast<int>(sY * space.y)) + parentStart;
    end = ivec2(static_cast<int>(eX * space.x), static_cast<int>(eY * space.y)) + parentStart;

    // Debug output
    std::cout << "Layout position calculated: " << std::endl;
    std::cout << "Start: (" << start.x << ", " << start.y << ") End: (" << end.x << ", " << end.y << ")" << std::endl;

    // Recursively calculate positions for nested layouts
    for (auto& nestedLayout : nestedLayouts) {
        nestedLayout->calculatePosition(start, end);
    }
}

void Layout::render(Screen& screen) {
    if (!active) return;

    for (const auto& element : elements) {
        element->draw(screen, start, end);
    }

    for (const auto& nestedLayout : nestedLayouts) {
        nestedLayout->render(screen);
    }
}

// New method to handle and propagate events
void Layout::handleEvent(const Event& event) {
    // Toggle visibility on SHOW event
    if (event.type == EventType::SHOW && event.target == name) {
        active = !active;
    }

    // Propagate CLICK events to all elements
    for (auto& element : elements) {
        // Only call handleEvent if the element is a ButtonElement
        ButtonElement* button = dynamic_cast<ButtonElement*>(element.get());
        if (button && button->handleEvent(event) && event.type == EventType::CLICK) {
            // Trigger SHOW event based on ButtonElement's target layout
            Event showEvent(EventType::SHOW, 0, 0, event.target);
            handleEvent(showEvent);
        }
    }

    // Propagate event to nested layouts
    for (auto& nestedLayout : nestedLayouts) {
        nestedLayout->handleEvent(event);
    }
}