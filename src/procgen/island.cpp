// procgen/island.cpp -- Procedural generation code for generating each individual island in the game world.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>

#include "3rdparty/PerlinNoise/PerlinNoise.hpp"
#include "core/game.hpp"
#include "procgen/island.hpp"
#include "ui/dev-canvas.hpp"
#include "util/math/mathutils.hpp"
#include "util/math/random.hpp"

#include "core/core.hpp"

namespace gorp {

// Generates a new island of the specified size.
IslandProcGen::IslandProcGen(uint16_t size, uint32_t seed) : seed_(seed), size_(size)
{
    if (!seed) seed_ = random::get<uint32_t>(INT_MAX);
    if (size < ISLAND_SIZE_MIN || size > ISLAND_SIZE_MAX) throw GuruMeditation("Invalid island size!", size, ISLAND_SIZE_MAX);

    generate_heightmap();
    determine_sub_islands();
}

// Determines which land-masses are contiguous, and defines these as sub-islands.
void IslandProcGen::determine_sub_islands()
{
    // If we're generating dev maps, we need to create a new canvas here.
    DevCanvas* canvas_ptr = nullptr;
    if (GENERATE_DEV_MAPS)
    {
        uint32_t canvas_id = game().add_element(std::make_unique<DevCanvas>(Vector2u(size_, size_)));
        DevCanvas& canvas_ref = static_cast<DevCanvas&>(game().element(canvas_id));
        canvas_ptr = &canvas_ref;
    }
    
    sub_island_id_.resize(size_ * size_, SUB_ISLAND_ID_UNDEFINED);
    unsigned int current_sub_id = 0;
    for (unsigned int x = 0; x < size_; x++)
    {
        for (unsigned int y = 0; y < size_; y++)
        {
            const uint32_t index = mathutils::array_index({x, y}, {size_, size_});
            const float height = height_map_.at(index);
            if (height <= HEIGHT_MAP_WATER)
            {
                sub_island_id_.at(index) = SUB_ISLAND_ID_WATER;
                continue;
            }
            else if (sub_island_id_.at(index) == SUB_ISLAND_ID_UNDEFINED) floodfill_sub_islands({x, y}, current_sub_id++, canvas_ptr);
        }
    }
}

// Flood-fills an area starting at the coordinates, with the specified ID.
void IslandProcGen::floodfill_sub_islands(Vector2u start, unsigned int id, DevCanvas* canvas_ptr)
{
    const uint32_t index = mathutils::array_index({start.x, start.y}, {size_, size_});
    if (sub_island_id_.at(index) != SUB_ISLAND_ID_UNDEFINED) return;    // Do nothing if this tile has already been flood-filled.
    if (height_map_.at(index) <= HEIGHT_MAP_WATER) return;  // Do nothing if this tile is water level or below.

    sub_island_id_.at(index) = id;  // Set the sub-island ID for this tile.

    // If an existing sub_island_coords_ vector does not yet exist for this ID, make a new one.
    if (sub_island_coords_.size() < id + 1)
    {
        std::vector<Vector2u> new_vec;
        new_vec.push_back(start);
        sub_island_coords_.push_back(new_vec);
    }
    // Otherwise, just insert our current coordinates into the existing vector.
    else sub_island_coords_.at(id).push_back(start);

    // If we're generating dev maps, now's a good time to draw the islands.
    if (GENERATE_DEV_MAPS && canvas_ptr)
    {
        Colour colour = Colour::GRAY_DARK;
        switch(id)
        {
            case 0: colour = Colour::RED; break;
            case 1: colour = Colour::ORANGE; break;
            case 2: colour = Colour::YELLOW; break;
            case 3: colour = Colour::GREEN; break;
            case 4: colour = Colour::CYAN; break;
            case 5: colour = Colour::BLUE; break;
            case 6: colour = Colour::PURPLE; break;
            case 7: colour = Colour::BROWN; break;
            case 8: colour = Colour::RED_LIGHT; break;
            case 9: colour = Colour::ORANGE_LIGHT; break;
            case 10: colour = Colour::YELLOW_LIGHT; break;
            case 11: colour = Colour::GREEN_LIGHT; break;
            case 12: colour = Colour::CYAN_LIGHT; break;
            case 13: colour = Colour::BLUE_LIGHT; break;
            case 14: colour = Colour::PURPLE_LIGHT; break;
            case 15: colour = Colour::BROWN_LIGHT; break;
            case 16: colour = Colour::RED_DARK; break;
            case 17: colour = Colour::ORANGE_DARK; break;
            case 18: colour = Colour::YELLOW_DARK; break;
            case 19: colour = Colour::GREEN_DARK; break;
            case 20: colour = Colour::CYAN_DARK; break;
            case 21: colour = Colour::BLUE_DARK; break;
            case 22: colour = Colour::PURPLE_DARK; break;
            case 23: colour = Colour::BROWN_DARK; break;
            case 24: colour = Colour::GRAY; break;
            case 25: colour = Colour::WHITE; break;
        }
        canvas_ptr->put(Glyph::FULL_BLOCK, Vector2(start.x, start.y), colour);
    }

    // Check neighbouring tiles.
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (x == 0 && y == 0) continue;
            if (static_cast<int>(start.x) + x < 0 || start.x + x >= size_) continue;
            if (static_cast<int>(start.y) + y < 0 || start.y + y >= size_) continue;
            floodfill_sub_islands(Vector2u(start.x + x, start.y + y), id, canvas_ptr);
        }
    }
}

// Generates the heightmap of the island, based on Perlin noise followed by some other tweaks.
void IslandProcGen::generate_heightmap()
{
    // First, we need to determine the height-map for the island. We'll do this with Perlin noise, then adjust it to allow for a coast.
    height_map_.resize(size_ * size_);
    const siv::PerlinNoise perlin{seed_};
    for (unsigned int x = 0; x < size_; x++)
    {
        for (unsigned int y = 0; y < size_; y++)
        {
            const float noise = perlin.octave2D_01((x * PERLIN_ZOOM), (y * PERLIN_ZOOM), 4);
            height_map_.at(mathutils::array_index({x, y}, {size_, size_})) = noise;
        }
    }

    // Adjust the height based on the distance from the centre of the map. Ideally, this can be tweaked to provide a coastline.
    auto get_height_modifier = [this](unsigned int x, unsigned int y)
    {
        const float centre = (size_ - 1) / 2.0f;
        const float dx = x - centre, dy = y - centre;
        const float distance = std::sqrt((dx * dx) + (dy * dy));
        const float max_distance = std::sqrt(2) * centre;
        return (distance / max_distance) * ISLAND_HEIGHT_MODIFIER;
    };
    for (unsigned int x = 0; x < size_; x++)
    {
        for (unsigned int y = 0; y < size_; y++)
        {
            const uint32_t index = mathutils::array_index({x, y}, {size_, size_});

            // The further to the edges of the map you go, the lower the land drops.
            height_map_.at(index) -= get_height_modifier(x, y);

            // We're also gonna ensure the map border is oceans, and the next couple of tiles in are similarly lowered.
            if (!x || !y || x == size_ - 1u || y == size_ - 1u) height_map_.at(index) = 0.0f;   // The outer border is always minimum-height.
            else if (x == 1 || y == 1 || x == size_ - 2u || y == size_ - 2u) height_map_.at(index) = std::min(height_map_.at(index) - BORDER_MODIFIER_OUTER,
                HEIGHT_MAP_WATER);
            else if (x == 2 || y == 2 || x == size_ - 3u || y == size_ - 3u) height_map_.at(index) = std::min(height_map_.at(index) - BORDER_MODIFIER_INNER,
                HEIGHT_MAP_LOWLAND);
        }
    }

    // Remove solitary tiles stuck in the water. This means: anything surrounded entirely by deep water becomes deep water, anything surrounded entirely by
    // shallow water becomes shallow water. Anything that's already deeper than its neighbours is ignored.
    for (unsigned int x = 1; x < size_ - 1u; x++)
    {
        for (unsigned int y = 1; y < size_ - 1u; y++)
        {
            const uint32_t index = mathutils::array_index({x, y}, {size_, size_});
            const float current_level = height_map_.at(index);
            float highest_neighbour = 0.0f;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx == 0 && dy == 0) continue;
                    const float neighbour_tile = height_map_.at(mathutils::array_index(Vector2u(x + dx, y + dy), {size_, size_}));
                    if (neighbour_tile > highest_neighbour) highest_neighbour = neighbour_tile;
                }
            }

            if (current_level <= highest_neighbour) continue;
            else if (highest_neighbour <= HEIGHT_MAP_DEEP_WATER) height_map_.at(index) = HEIGHT_MAP_DEEP_WATER;
            else if (highest_neighbour <= HEIGHT_MAP_WATER) height_map_.at(index) = HEIGHT_MAP_WATER;
        }
    }

    // If we're generating dev maps, at this point we'll draw the final result.
    if (GENERATE_DEV_MAPS)
    {
        uint32_t canvas_id = game().add_element(std::make_unique<DevCanvas>(Vector2u(size_, size_)));
        DevCanvas& canvas_ref = static_cast<DevCanvas&>(game().element(canvas_id));
        for (unsigned int x = 0; x < size_; x++)
        {
            for (unsigned int y = 0; y < size_; y++)
            {
                const float noise = height_map_.at(mathutils::array_index({x, y}, {size_, size_}));
                Colour col = Colour::GREEN;
                if (noise <= HEIGHT_MAP_DEEP_WATER) col = Colour::BLUE_DARK;
                else if (noise <= HEIGHT_MAP_WATER) col = Colour::BLUE;
                else if (noise <= HEIGHT_MAP_LOWLAND) col = Colour::GREEN_LIGHT;
                else if (noise >= HEIGHT_MAP_MOUNTAIN_PEAK) col = Colour::WHITE;
                else if (noise >= HEIGHT_MAP_MOUNTAIN) col = Colour::GRAY;
                else if (noise >= HEIGHT_MAP_HIGHLAND) col = Colour::GREEN_DARK;
                canvas_ref.put(Glyph::FULL_BLOCK, Vector2(x, y), col);
            }
        }
    }
}

}   // namespace gorp
