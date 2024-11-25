#ifndef __TVEC3_HPP__
#define __TVEC3_HPP__

#include <cmath> // for sqrt function
#include <stdexcept> // for exception handling

template <typename T>
class Tvec3
{
private:
    T components[3];  // Array to store vector components (x, y, z)

public:
    // Default constructor initializing vector to (0, 0, 0)
    Tvec3() : components{0, 0, 0}, x(components[0]), y(components[1]), z(components[2]) {}

    // Copy constructor
    Tvec3(const Tvec3& cp) : Tvec3() {
        x = cp.x;
        y = cp.y;
        z = cp.z;
    }

    // Constructor with given x, y, z values
    Tvec3(T _x, T _y, T _z) : Tvec3() {
        x = _x;
        y = _y;
        z = _z;
    }

    // Assignment operator
    Tvec3& operator=(const Tvec3& cp) {
        if (this != &cp) {
            x = cp.x;
            y = cp.y;
            z = cp.z;
        }
        return *this;
    }

    // Vector addition
    Tvec3 operator+(const Tvec3& rhs) const {
        return Tvec3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    // Vector subtraction
    Tvec3 operator-(const Tvec3& rhs) const {
        return Tvec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    // Scalar multiplication
    Tvec3 operator*(T scalar) const {
        return Tvec3(x * scalar, y * scalar, z * scalar);
    }

    // Dot product of two vectors
    T dot(const Tvec3& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    // Calculate the magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Return the unit vector (normalized vector)
    Tvec3 unit() const {
        float mag = magnitude();
        if (mag == 0) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        return Tvec3(x / mag, y / mag, z / mag);
    }

    // Specialized method to handle zero vectors:
    // Returns a zero vector if magnitude is zero, instead of normalizing
    Tvec3 safeUnit() const {
        float mag = magnitude();
        if (mag == 0) {
            return *this;  // Return the zero vector itself
        }
        return Tvec3(x / mag, y / mag, z / mag);
    }

    // Cross product of two vectors
    Tvec3 cross(const Tvec3& rhs) const {
        return Tvec3(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
    }

    // In-place vector addition
    Tvec3& operator+=(const Tvec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // In-place vector subtraction
    Tvec3& operator-=(const Tvec3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    // In-place scalar multiplication
    Tvec3& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // x, y, z are references to the elements of the array
    T& x, &y, &z;
};

// Typedef for common use cases (float and int vectors)
typedef Tvec3<float> vec3;
typedef Tvec3<int> ivec3;

#endif // __TVEC3_HPP__
