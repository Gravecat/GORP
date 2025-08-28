// util/math/mathutils.cpp -- Miscellaneous math-related utility functions.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "util/math/mathutils.hpp"

namespace gorp {
namespace mathutils {

// Takes X,Y coordinates, and returns a flat array index for the coordinates.
uint32_t array_index(Vector2 position, Vector2 array_size)
{
    if (position.x < 0 || position.y < 0 || position.x >= array_size.x || position.y >= array_size.y)
        throw GuruMeditation("array_index given invalid coords", position.x, position.y);
    else if (array_size.x < 0 || array_size.y < 0) throw GuruMeditation("array_index given invalid array size", array_size.x, array_size.y);
    return (position.y * array_size.x) + position.x;
}

}   // namespace mathutils
}   // namespace gorp
