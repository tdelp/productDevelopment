#include "../all_headers.hpp"

// Layout.cpp
void Layout::addElement(std::unique_ptr<Element> element) {
    elements.push_back(std::move(element));
}

void Layout::addNestedLayout(std::unique_ptr<Layout> layout) {
    layout->parentLayout = this;
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
            if (button && button->isClickable() && button->handleEvent(event)) {
                // Toggle visibility due to CLICK
                clickToggled = !clickToggled;
                if (!nestedLayouts.empty()) {
                    nestedLayouts[0]->setActive(clickToggled);
                }
                
                // Play sound if clickable button is clicked
                Event soundEvent(EventType::SOUND);
                propagateEventUp(soundEvent, soundPlayer);
                return;
            }
        }
        for (auto& nestedLayout : nestedLayouts) {
            nestedLayout->handleEvent(event, soundPlayer);
        }
    } 
    else if (event.type == EventType::SHOW) {
        bool isHovering = false;
        for (auto& element : elements) {
            ButtonElement* button = dynamic_cast<ButtonElement*>(element.get());
            if (button && button->isHoverable() && button->handleHover(event)) {
                // Show due to hover only if not already toggled by click
                if (!clickToggled && !nestedLayouts.empty()) {
                    nestedLayouts[0]->setActive(true);
                }
                isHovering = true;
                break;
            }
        }
        // Hide layout if hover ends and it wasn’t toggled by click
        if (!isHovering && !clickToggled && !nestedLayouts.empty()) {
            nestedLayouts[0]->setActive(false);
        }
        for (auto& nestedLayout : nestedLayouts) {
            nestedLayout->handleEvent(event, soundPlayer);
        }
    } 
    else if (event.type == EventType::SOUND && parentLayout == nullptr) {
        soundPlayer->playSound();
    }
}


void Layout::propagateEventUp(const Event& event, SoundPlayer* soundPlayer) {
    if (parentLayout) {
        parentLayout->propagateEventUp(event, soundPlayer);
    } else if (event.type == EventType::SOUND) {
        soundPlayer->playSound();
    }
}
