// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// core/global/vector2.hpp -- The Vector2 struct provides a simple way of storing and sharing 2D coordinates.

#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>

namespace gorp {

// Simple two-dimensional integer coordinate struct.
struct Vector2
{
    Vector2() : x(0), y(0) { }
    Vector2(int32_t vx, int32_t vy) : x(vx), y(vy) { }
    Vector2(std::initializer_list<int32_t> list)
    {
        auto it = list.begin();
        x = (it != list.end()) ? *it++ : 0;
        y = (it != list.end()) ? *it : 0;
    }

    Vector2     operator+(const Vector2 &other) const { return { x + other.x, y + other.y }; }
    Vector2     operator-(const Vector2 &other) const { return { x - other.x, y - other.y }; }
    Vector2     operator/(const Vector2 &other) const { return { x / other.x, y / other.y }; }
    Vector2     operator*(const Vector2 &other) const { return { x * other.x, y * other.y }; }
    Vector2     operator%(const Vector2 &other) const { return { x % other.x, y % other.y }; }
    bool        operator==(const Vector2 &other) const { return (x == other.x && y == other.y); }
    bool        operator!=(const Vector2 &other) const { return (x != other.x || y != other.y); }
    bool        operator!() const { return (x == 0 && y == 0); }
    bool        operator>(const Vector2 &other) const { return (x > other.x && y > other.y); }
    bool        operator<(const Vector2 &other) const { return (x < other.x || y < other.y); }
    explicit    operator bool() const { return (x != 0 || y != 0); }

    template<typename T> Vector2    operator+(const T other) const { return {x + other, y + other}; }
    template<typename T> Vector2    operator-(const T other) const { return {x - other, y - other}; }
    template<typename T> Vector2    operator/(const T other) const { return {x / other, y / other}; }
    template<typename T> Vector2    operator*(const T other) const { return {x * other, y * other}; }
    template<typename T> Vector2    operator%(const T other) const { return {x % other, y % other}; }

    const std::string   to_string() const { return std::to_string(x) + "," + std::to_string(y); }

    int32_t x, y;
};

}   // namespace gorp
