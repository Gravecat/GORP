// procgen/island.cpp -- Procedural generation code for generating each individual island in the game world.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "3rdparty/PerlinNoise/PerlinNoise.hpp"
#include "procgen/island.hpp"
#include "util/math/mathutils.hpp"
#include "util/math/random.hpp"

namespace gorp {

// Generates a new island of the specified size.
IslandProcGen::IslandProcGen(uint16_t size, uint32_t seed) : seed_(seed), size_(size)
{
    if (!seed) seed_ = random::get<uint32_t>(UINT_MAX);
    if (size < ISLAND_SIZE_MIN || size > ISLAND_SIZE_MAX) throw GuruMeditation("Invalid island size!", size, ISLAND_SIZE_MAX);

    // First, we need to determine the height-map for the island. We'll do this with Perlin noise, then adjust it to allow for a coast.
    std::vector<float> height_map(size * size);
    const siv::PerlinNoise perlin{seed_};
    for (unsigned int x = 0; x < size; x++)
    {
        for (unsigned int y = 0; y < size; y++)
        {
            const float noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
            height_map.at(mathutils::array_index({x, y}, {size, size})) = noise;
        }
    }
}

}   // namespace gorp
