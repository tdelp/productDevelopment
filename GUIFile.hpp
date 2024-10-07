#ifndef __GUIFILE_HPP__
#define __GUIFILE_HPP__

#include <vector>
#include <string>
#include "Screen.hpp"

// Structs for integer-based data
struct Line {
    ivec2 start;
    ivec2 end;
    ivec3 color;
};

struct Box {
    ivec2 min;
    ivec2 max;
    ivec3 color;
};

struct Point {
    ivec2 position;
    ivec3 color;
};

// Structs for floating-point-based data
struct LineFloat {
    vec2 start;
    vec2 end;
    vec3 color;
};

struct BoxFloat {
    vec2 min;
    vec2 max;
    vec3 color;
};

struct PointFloat {
    vec2 position;
    vec3 color;
};

class GUIFile {
public:
    GUIFile();
    ~GUIFile();

    void addLine(Line line);
    void addBox(Box box);
    void addPoint(Point point);
    void addLineFloat(LineFloat lineFloat);
    void addBoxFloat(BoxFloat boxFloat);
    void addPointFloat(PointFloat pointFloat);

    const std::vector<Line>& getLines() const;
    const std::vector<Box>& getBoxes() const;
    const std::vector<Point>& getPoints() const;
    const std::vector<LineFloat>& getLinesFloat() const;
    const std::vector<BoxFloat>& getBoxesFloat() const;
    const std::vector<PointFloat>& getPointsFloat() const;

    bool writeToFile(const std::string& filename);
    bool readFromFile(const std::string& filename);

private:
    std::vector<Line> lines;
    std::vector<Box> boxes;
    std::vector<Point> points;
    std::vector<LineFloat> linesFloat;
    std::vector<BoxFloat> boxesFloat;
    std::vector<PointFloat> pointsFloat;
};

#endif // __GUIFILE_HPP__
