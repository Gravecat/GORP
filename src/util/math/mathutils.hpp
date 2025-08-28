// util/math/mathutils.hpp -- Miscellaneous math-related utility functions.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {
namespace mathutils {

uint32_t    array_index(Vector2u position, Vector2u array_size);  // Takes X,Y coordinates, and returns a flat array index for the coordinates.

}   // namespace mathutils
}   // namespace gorp
