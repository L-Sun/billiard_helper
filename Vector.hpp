#pragma once
#include <cmath>

struct Vec2 {
    double x, y;
    operator bool() const { return x >= 0 && y >= 0; }

    inline const Vec2 operator+(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
    inline const Vec2 operator-(const Vec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
    inline const Vec2 operator*(double num) const { return {x * num, y * num}; }
    inline const Vec2 operator/(double num) const { return {x / num, y / num}; }
    friend const Vec2 operator*(double num, const Vec2& pos) { return pos * num; }

    inline double norm() const { return std::sqrt(x * x + y * y); }
};

inline double dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}