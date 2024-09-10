#ifndef __VEC_HPP__
#define __VEC_HPP__

#include <cmath> // for sqrt function

template <typename T>
class Tvec2
{
private:
    T components[2];

public:
    Tvec2() : components{0, 0}, x(components[0]), y(components[1]) {}

    Tvec2(const Tvec2& cp) : Tvec2() {
        x = cp.x;
        y = cp.y;
    }

    Tvec2(T _x, T _y) : Tvec2() {
        x = _x;
        y = _y;
    }

    Tvec2& operator=(const Tvec2& cp) {
        if (this != &cp) {
            x = cp.x;
            y = cp.y;
        }
        return *this;
    }

    Tvec2 operator+(const Tvec2& rhs) const {
        return Tvec2(x + rhs.x, y + rhs.y);
    }

    Tvec2 operator-(const Tvec2& rhs) const {
        return Tvec2(x - rhs.x, y - rhs.y);
    }

    Tvec2 operator*(T scalar) const {
        return Tvec2(x * scalar, y * scalar);
    }

    T dot(const Tvec2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Tvec2 unit() const {
        float mag = magnitude();
        return Tvec2(x / mag, y / mag);
    }

    T& x, &y;
};

typedef Tvec2<float> vec2;
typedef Tvec2<int> ivec2;

#endif // __VEC_HPP__
