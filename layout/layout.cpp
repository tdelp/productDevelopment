#include "../all_headers.hpp"

void Layout::addElement(std::unique_ptr<Element> element) {
    elements.push_back(std::move(element));
}

void Layout::addNestedLayout(std::unique_ptr<Layout> layout) {
    layout->parentLayout = this;  // Set parent for nested layouts
    nestedLayouts.push_back(std::move(layout));
}

void Layout::calculatePosition(const ivec2& parentStart, const ivec2& parentEnd) {
    ivec2 space = parentEnd - parentStart;
    start = ivec2(static_cast<int>(sX * space.x), static_cast<int>(sY * space.y)) + parentStart;
    end = ivec2(static_cast<int>(eX * space.x), static_cast<int>(eY * space.y)) + parentStart;

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

void Layout::handleEvent(const Event& event, SoundPlayer* soundPlayer) {
    if (event.type == EventType::CLICK) {
        for (auto& element : elements) {
            ButtonElement* button = dynamic_cast<ButtonElement*>(element.get());
            if (button && button->handleEvent(event)) {
                // Toggle the visibility of the first nested layout
                if (!nestedLayouts.empty()) {
                    nestedLayouts[0]->setActive(!nestedLayouts[0]->isActive());
                }

                // Play sound only if the button handled the click
                if (soundPlayer) {
                    soundPlayer->playSound();
                }
                
                return; // Stop further processing after handling click on button
            }
        }

        // Propagate CLICK event to nested layouts if not handled by any element
        for (auto& nestedLayout : nestedLayouts) {
            nestedLayout->handleEvent(event, soundPlayer);
        }
    }
}
