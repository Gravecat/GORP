// procgen/island.hpp -- Procedural generation code for generating each individual island in the game world.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {

class IslandProcGen {
public:
    IslandProcGen() = delete;   // No default constructor.
    IslandProcGen(uint16_t size, unsigned int seed = 0);    // Generates a new island of the specified size, with an optional PRNG seed.

private:
    static constexpr uint16_t   ISLAND_SIZE_MAX =   512;    // The largest allowed island size.
    static constexpr uint16_t   ISLAND_SIZE_MIN =   16;     // The smallest sllowed island size.

    uint32_t    seed_;  // The PRNG seed, used to (hopefully) generate identical islands with the same seed.
    uint16_t    size_;  // The size of this island map. Limited to uint16_t because any larger would just be ridiculous.
};

}   // namespace gorp
