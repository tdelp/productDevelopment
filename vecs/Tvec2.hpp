#ifndef __TVEC2_HPP__
#define __TVEC2_HPP__

#include <cmath> // for sqrt function
#include <stdexcept> // for exception handling

template <typename T>
class Tvec2
{
private:
    T components[2];  // Array to store vector components (x, y)

public:
    // Default constructor initializing vector to (0, 0)
    Tvec2() : components{0, 0}, x(components[0]), y(components[1]) {}

    // Copy constructor
    Tvec2(const Tvec2& cp) : Tvec2() {
        x = cp.x;
        y = cp.y;
    }

    // Constructor with given x, y values
    Tvec2(T _x, T _y) : Tvec2() {
        x = _x;
        y = _y;
    }

    // Assignment operator
    Tvec2& operator=(const Tvec2& cp) {
        if (this != &cp) {
            x = cp.x;
            y = cp.y;
        }
        return *this;
    }

    // Vector addition
    Tvec2 operator+(const Tvec2& rhs) const {
        return Tvec2(x + rhs.x, y + rhs.y);
    }

    // Vector subtraction
    Tvec2 operator-(const Tvec2& rhs) const {
        return Tvec2(x - rhs.x, y - rhs.y);
    }

    // Scalar multiplication
    Tvec2 operator*(T scalar) const {
        return Tvec2(x * scalar, y * scalar);
    }

    // Dot product of two vectors
    T dot(const Tvec2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    // Calculate the magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Return the unit vector (normalized vector)
    Tvec2 unit() const {
        float mag = magnitude();
        if (mag == 0) {
            throw std::runtime_error("Cannot normalize a vector with magnitude 0");
        }
        return Tvec2(x / mag, y / mag);
    }

    // Specialized method to handle zero vectors:
    // Returns a zero vector if magnitude is zero, instead of normalizing
    Tvec2 safeUnit() const {
        float mag = magnitude();
        if (mag == 0) {
            return *this;  // Return the zero vector as it is
        }
        return Tvec2(x / mag, y / mag);
    }

    // In-place vector addition
    Tvec2& operator+=(const Tvec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    // In-place vector subtraction
    Tvec2& operator-=(const Tvec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    // In-place scalar multiplication
    Tvec2& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // x and y are references to the first and second elements of the array
    T& x, &y;
};

// Typedef for common use cases (float and int vectors)
typedef Tvec2<float> vec2;
typedef Tvec2<int> ivec2;

#endif // __TVEC2_HPP__
