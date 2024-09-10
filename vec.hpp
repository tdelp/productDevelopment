#ifndef __VEC_HPP__
#define __VEC_HPP__

template <typename T>
class Tvec2
{
private:
    T components[2];

public:
    T& x, &y;

    Tvec2() : components{0, 0}, x(components[0]), y(components[1]) {}

    Tvec2(T _x, T _y) : components{_x, _y}, x(components[0]), y(components[1]) {}

    Tvec2(const Tvec2& cp) : components{cp.x, cp.y}, x(components[0]), y(components[1]) {}

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
};

typedef Tvec2<float> vec2;
typedef Tvec2<int> ivec2;

#endif // __VEC_HPP__
