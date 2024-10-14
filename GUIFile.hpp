// Element.hpp
#ifndef __ELEMENT_HPP__
#define __ELEMENT_HPP__

#include <fstream>
#include "Screen.hpp"

class Element {
public:
    virtual ~Element() = default;
    virtual void draw(Screen& screen) const = 0;
    virtual void writeToFile(std::ofstream& outputFile) const = 0;
};

#endif // __ELEMENT_HPP__

// GUIFile.hpp
#ifndef __GUIFILE_HPP__
#define __GUIFILE_HPP__

#include "Element.hpp"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

class GUIFile {
private:
    std::vector<std::unique_ptr<Element>> elements;

public:
    bool readFile(const std::string& filename);
    const std::vector<std::unique_ptr<Element>>& getElements() const { return elements; }
    void writeFile(const std::string& filename);
};

#endif // __GUIFILE_HPP__

// Screen.hpp
#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include <SDL2/SDL.h>
#include <iostream>

class Screen {
public:
    unsigned int width, height;
    SDL_Surface* surface;

    Screen(unsigned int w, unsigned int h, SDL_Surface* surface)
        : width(w), height(h), surface(surface) {}

    ~Screen() {
        SDL_FreeSurface(surface);
    }

    void setSafePixel(int x, int y, Uint32 color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[y * width + x] = color;
        }
    }
};

#endif // __SCREEN_HPP__
