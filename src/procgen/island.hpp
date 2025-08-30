// procgen/island.hpp -- Procedural generation code for generating each individual island in the game world.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {

class DevCanvas;    // defined ui/dev-canvas.hpp

class IslandProcGen {
public:
    IslandProcGen() = delete;   // No default constructor.
    IslandProcGen(uint16_t size, unsigned int seed = 0);    // Generates a new island of the specified size, with an optional PRNG seed.

private:
    void    determine_sub_islands();        // Determines which land-masses are contiguous, and defines these as sub-islands.
    void    floodfill_sub_islands(Vector2u start, unsigned int id, DevCanvas* canvas_ptr);  // Flood-fills an area starting at the coords with the specified ID.
    void    generate_heightmap();           // Generates the heightmap of the island, based on Perlin noise followed by some other tweaks.

    static constexpr bool       GENERATE_DEV_MAPS =         true;   // Used for visual feedback during development/debugging/testing.

    static constexpr uint16_t   ISLAND_SIZE_MAX =           512;    // The largest allowed island size.
    static constexpr uint16_t   ISLAND_SIZE_MIN =           16;     // The smallest sllowed island size.

    static constexpr float      BORDER_MODIFIER_INNER =     0.1f;   // The fixed reduction in height for the inner border of the map.
    static constexpr float      BORDER_MODIFIER_OUTER =     0.2f;   // As above, but for the outer border.
    static constexpr float      ISLAND_HEIGHT_MODIFIER =    0.6f;   // The distance-from-centre modifier that adjusts the island height, providing a coatline.

    static constexpr float      PERLIN_OCTAVES =            4;      // The number of Perlin noise octaves.
    static constexpr float      PERLIN_ZOOM =               0.1f;   // The Perlin noise zoom level.

    static constexpr float      HEIGHT_MAP_DEEP_WATER =     0.1f;   // Any tile heights at this point or below are deep water.
    static constexpr float      HEIGHT_MAP_WATER =          0.2f;   // As above, but for regular water.
    static constexpr float      HEIGHT_MAP_LOWLAND =        0.3f;   // The lowest level of terrain above water.
    static constexpr float      HEIGHT_MAP_HIGHLAND =       0.6f;   // At this point or above, we get into the highlands.
    static constexpr float      HEIGHT_MAP_MOUNTAIN =       0.7f;   // Mountains begin at this level.
    static constexpr float      HEIGHT_MAP_MOUNTAIN_PEAK =  0.8f;   // Mountain peaks at the highest levels.

    static constexpr int        SUB_ISLAND_ID_UNDEFINED =   -1;     // Sub-island ID has not yet been set.
    static constexpr int        SUB_ISLAND_ID_WATER =       -2;     // This tile is water, and cannot belong to a sub-island.

    std::vector<float>  height_map_;    // The height map of the island, which determines the terrain.
    uint32_t    seed_;          // The PRNG seed, used to (hopefully) generate identical islands with the same seed.
    uint16_t    size_;          // The size of this island map. Limited to uint16_t because any larger would just be ridiculous.
    std::vector<std::vector<Vector2u>>  sub_island_coords_; // Coordinates for each sub-island on the generated map.
    std::vector<int>    sub_island_id_; // Sub-island ID markers for each coordinate on the map.
};

}   // namespace gorp
