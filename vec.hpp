#ifndef __VEC_HPP__
#define __VEC_HPP__

template <typename T>
class Tvec2
{
private:
    T components[2]; // array to store x and y components

public:
    // References for direct access to components
    T& x, &y;

    // Default constructor: initializes x and y to zero
    Tvec2() : components{0, 0}, x(components[0]), y(components[1]) {}

    // Parameterized constructor: initializes x and y with given values
    Tvec2(T _x, T _y) : components{_x, _y}, x(components[0]), y(components[1]) {}

    // Copy constructor: initializes x and y with values from another Tvec2 object
    Tvec2(const Tvec2& cp) : components{cp.x, cp.y}, x(components[0]), y(components[1]) {}

    // Assignment operator: assigns values from another Tvec2 object
    Tvec2& operator=(const Tvec2& cp) {
        if (this != &cp) {
            x = cp.x;
            y = cp.y;
        }
        return *this;
    }
};

typedef Tvec2<float> vec2;
typedef Tvec2<int> ivec2;

#endif // __VEC_HPP__