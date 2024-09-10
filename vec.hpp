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

template <typename T>
class Tvec3
{
private:
    T components[3];

public:
    Tvec3() : components{0, 0, 0}, x(components[0]), y(components[1]), z(components[2]) {}

    Tvec3(const Tvec3& cp) : Tvec3() {
        x = cp.x;
        y = cp.y;
        z = cp.z;
    }

    Tvec3(T _x, T _y, T _z) : Tvec3() {
        x = _x;
        y = _y;
        z = _z;
    }

    Tvec3& operator=(const Tvec3& cp) {
        if (this != &cp) {
            x = cp.x;
            y = cp.y;
            z = cp.z;
        }
        return *this;
    }

    Tvec3 operator+(const Tvec3& rhs) const {
        return Tvec3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Tvec3 operator-(const Tvec3& rhs) const {
        return Tvec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Tvec3 operator*(T scalar) const {
        return Tvec3(x * scalar, y * scalar, z * scalar);
    }

    T dot(const Tvec3& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Tvec3 unit() const {
        float mag = magnitude();
        return Tvec3(x / mag, y / mag, z / mag);
    }

    Tvec3 cross(const Tvec3& rhs) const {
        return Tvec3(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
    }

    T& x, &y, &z;
};

typedef Tvec3<float> vec3;
typedef Tvec3<int> ivec3;

#endif // __VEC_HPP__
