// GUIFile.hpp
#ifndef __GUIFILE_HPP__
#define __GUIFILE_HPP__

#include <vector>
#include <string>
#include "Screen.hpp"

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

class GUIFile {
public:
    GUIFile();
    ~GUIFile();
};

#endif // __GUIFILE_HPP__
