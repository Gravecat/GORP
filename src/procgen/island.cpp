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

namespace gorp {

// Generates a new island of the specified size.
IslandProcGen::IslandProcGen(uint16_t size, uint32_t seed) : canvas_id_(0), seed_(seed), size_(size)
{
    if (!seed) seed_ = random::get<uint32_t>(INT_MAX);
    if (size < ISLAND_SIZE_MIN || size > ISLAND_SIZE_MAX) throw GuruMeditation("Invalid island size!", size, ISLAND_SIZE_MAX);

    // This should only be used for testing and development, never for normal gameplay!
    DevCanvas* canvas_ptr = nullptr;
    if (GENERATE_DEV_MAPS)
    {
        canvas_id_ = game().add_element(std::make_unique<DevCanvas>(Vector2u(size_, size_)));
        DevCanvas& canvas_ref = static_cast<DevCanvas&>(game().element(canvas_id_));
        canvas_ptr = &canvas_ref;
    }

    // First, we need to determine the height-map for the island. We'll do this with Perlin noise, then adjust it to allow for a coast.
    std::vector<float> height_map(size * size);
    const siv::PerlinNoise perlin{seed_};
    for (unsigned int x = 0; x < size; x++)
    {
        for (unsigned int y = 0; y < size; y++)
        {
            const float noise = perlin.octave2D_01((x * 0.10), (y * 0.10), 4);
            height_map.at(mathutils::array_index({x, y}, {size, size})) = noise;
        }
    }

    // Adjust the height based on the distance from the centre of the map. Ideally, this can be tweaked to provide a coastline.
    auto get_height_modifier = [this](unsigned int x, unsigned int y)
    {
        const float centre = (size_ - 1) / 2.0f;
        const float dx = x - centre, dy = y - centre;
        const float distance = std::sqrt((dx * dx) + (dy * dy));
        const float max_distance = std::sqrt(2) * centre;
        return (distance / max_distance) * 0.6f;
    };
    for (unsigned int x = 0; x < size; x++)
    {
        for (unsigned int y = 0; y < size; y++)
        {
            const uint32_t index = mathutils::array_index({x, y}, {size, size});

            // The further to the edges of the map you go, the lower the land drops.
            height_map.at(index) -= get_height_modifier(x, y);

            // We're also gonna ensure the map border is oceans, and the next couple of tiles in are similarly lowered.
            if (!x || !y || x == size - 1u || y == size - 1u) height_map.at(index) = 0.0f;
            else if (x == 1 || y == 1 || x == size - 2u || y == size - 2u) height_map.at(index) = std::min(height_map.at(index) - 0.2f, 0.2f);
            else if (x == 2 || y == 2 || x == size - 3u || y == size - 3u) height_map.at(index) = std::min(height_map.at(index) - 0.1f, 0.3f);
        }
    }

    // Remove solitary tiles stuck in the water. This means: anything surrounded entirely by deep water becomes deep water, anything surrounded entirely by
    // shallow water becomes shallow water. Anything that's already deeper than its neighbours is ignored.
    for (unsigned int x = 1; x < size - 1u; x++)
    {
        for (unsigned int y = 1; y < size - 1u; y++)
        {
            const uint32_t index = mathutils::array_index({x, y}, {size, size});
            const float current_level = height_map.at(index);
            float highest_neighbour = 0.0f;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx == 0 && dy == 0) continue;
                    const float neighbour_tile = height_map.at(mathutils::array_index(Vector2u(x + dx, y + dy), {size, size}));
                    if (neighbour_tile > highest_neighbour) highest_neighbour = neighbour_tile;
                }
            }

            if (current_level <= highest_neighbour) continue;
            else if (highest_neighbour <= 0.1f) height_map.at(index) = 0.0f;
            else if (highest_neighbour <= 0.2f) height_map.at(index) = 0.2f;
        }
    }

    // If we're generating dev maps, at this point we'll draw the final result.
    if (GENERATE_DEV_MAPS)
    {
        for (unsigned int x = 0; x < size; x++)
        {
            for (unsigned int y = 0; y < size; y++)
            {
                const float noise = height_map.at(mathutils::array_index({x, y}, {size, size}));
                Colour col = Colour::GREEN;
                if (noise <= 0.1f) col = Colour::BLUE_DARK;
                else if (noise <= 0.2f) col = Colour::BLUE;
                else if (noise <= 0.3f) col = Colour::GREEN_LIGHT;
                else if (noise >= 0.8f) col = Colour::WHITE;
                else if (noise >= 0.7f) col = Colour::GRAY;
                else if (noise >= 0.6f) col = Colour::GREEN_DARK;
                canvas_ptr->put(Glyph::FULL_BLOCK, Vector2(x, y), col);
            }
        }
    }
}

// Destructor, cleans up after ourselves.
IslandProcGen::~IslandProcGen() { if (canvas_id_) game().delete_element(canvas_id_); }

}   // namespace gorp
