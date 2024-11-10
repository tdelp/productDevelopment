#include "parse.hpp"

Parser::Parser(const std::string& fileName) {
    loadFile(fileName);
}

void Parser::loadFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    while (std::getline(file, line)) {
        for (const char& c : line) {
            if (!std::isspace(c)) {
                data += c;
            }
        }
    }
}

std::unique_ptr<Layout> Parser::parseRootLayout() {
    size_t pos = data.find("<layout>");
    if (pos == std::string::npos) {
        return nullptr;
    }
    pos += std::string("<layout>").size();
    return parseLayout(pos);
}

std::unique_ptr<Layout> Parser::parseLayout(size_t& pos) {
    float sX = 0, sY = 0, eX = 1, eY = 1;
    bool active = true;

    // Check for 'active' attribute if it exists in the layout tag
    size_t activePos = data.find("active=", pos);
    if (activePos != std::string::npos && activePos < data.find(">", pos)) {
        active = parseBooleanAttribute("active", pos, true);
    }

    size_t layoutEnd = data.find("</layout>", pos);
    auto layout = std::make_unique<Layout>(sX, sY, eX, eY, active);

    while (pos < layoutEnd) {
        size_t boxPos = findTag("<box>", pos);
        size_t linePos = findTag("<line>", pos);
        size_t pointPos = findTag("<point>", pos);
        size_t trianglePos = findTag("<triangle>", pos);
        size_t nestedLayoutPos = findTag("<layout>", pos);

        if (nestedLayoutPos < layoutEnd && (nestedLayoutPos < boxPos && nestedLayoutPos < linePos &&
            nestedLayoutPos < pointPos && nestedLayoutPos < trianglePos)) {
            pos = nestedLayoutPos + std::string("<layout>").size();
            layout->addNestedLayout(parseLayout(pos));
        } else if (boxPos < layoutEnd && (boxPos < linePos && boxPos < pointPos && boxPos < trianglePos)) {
            pos = boxPos + std::string("<box>").size();
            layout->addElement(parseElement("box", pos));
        } else if (linePos < layoutEnd && (linePos < pointPos && linePos < trianglePos)) {
            pos = linePos + std::string("<line>").size();
            layout->addElement(parseElement("line", pos));
        } else if (pointPos < layoutEnd && pointPos < trianglePos) {
            pos = pointPos + std::string("<point>").size();
            layout->addElement(parseElement("point", pos));
        } else if (trianglePos < layoutEnd) {
            pos = trianglePos + std::string("<triangle>").size();
            layout->addElement(parseElement("triangle", pos));
        } else {
            break;
        }
    }
    pos = layoutEnd + std::string("</layout>").size();
    return layout;
}

std::unique_ptr<Element> Parser::parseElement(const std::string& type, size_t& pos) {
    if (type == "box") {
        auto min = parseVec2(pos);
        auto max = parseVec2(pos);
        auto color = parseVec3(pos);
        return ElementFactory::createBox(min, max, color);
    } else if (type == "line") {
        auto start = parseVec2(pos);
        auto end = parseVec2(pos);
        auto color = parseVec3(pos);
        return ElementFactory::createLine(start, end, color);
    } else if (type == "point") {
        auto position = parseVec2(pos);
        auto color = parseVec3(pos);
        return ElementFactory::createPoint(position, color);
    } else if (type == "triangle") {
        auto v0 = parseVec2(pos);
        auto v1 = parseVec2(pos);
        auto v2 = parseVec2(pos);
        auto color = parseVec3(pos);
        return ElementFactory::createTriangle(v0, v1, v2, color);
    }
    return nullptr;
}

std::array<float, 2> Parser::parseVec2(size_t& pos) {
    std::array<float, 2> vec;
    pos = data.find("<x>", pos) + 3;
    vec[0] = std::stof(data.substr(pos, data.find("</x>", pos) - pos));
    pos = data.find("<y>", pos) + 3;
    vec[1] = std::stof(data.substr(pos, data.find("</y>", pos) - pos));
    return vec;
}

std::array<float, 3> Parser::parseVec3(size_t& pos) {
    std::array<float, 3> vec;
    pos = data.find("<x>", pos) + 3;
    vec[0] = std::stof(data.substr(pos, data.find("</x>", pos) - pos));
    pos = data.find("<y>", pos) + 3;
    vec[1] = std::stof(data.substr(pos, data.find("</y>", pos) - pos));
    pos = data.find("<z>", pos) + 3;
    vec[2] = std::stof(data.substr(pos, data.find("</z>", pos) - pos));
    return vec;
}

bool Parser::parseBooleanAttribute(const std::string& attributeName, size_t pos, bool defaultValue) {
    size_t attrPos = data.find(attributeName + "=\"", pos);
    if (attrPos != std::string::npos && attrPos < data.find(">", pos)) {
        attrPos += attributeName.size() + 2;
        std::string value = data.substr(attrPos, data.find("\"", attrPos) - attrPos);
        return (value == "true");
    }
    return defaultValue;
}

size_t Parser::findTag(const std::string& tag, size_t pos) {
    return data.find(tag, pos);
}
