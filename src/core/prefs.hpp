// core/prefs.hpp -- Allows for simple user preferences to be stored and retrieved on future executions.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"
#include "util/file/filereader.hpp"
#include "util/file/filewriter.hpp"

namespace gorp {

class Prefs : public FileReader, public FileWriter {
public:
    static constexpr uint32_t   PREFS_VERSION = 3;  // The version changes when data files are no longer compatible.

            Prefs();                        // Constructor, sets default values.
    bool    ascii() const;                  // Checks if we're using ASCII glyphs.
    bool    auto_rescale() const;           // Checks if the tile scale changes automatically when the window resizes.
    void    clear_data();                   // Clears the loaded data once it's been processed.
    void    save_prefs();                   // Saves the prefs file to disk.
    void    set_auto_rescale(bool toggle);  // Sets whether or not the tile scale auto-changes on window resize.
    void    set_shader(bool toggle);        // Sets whether or not we're using the GLSL shader.
    void    set_shader_geom(bool toggle);   // Sets whether or not the shader is using geometry deforming.
    void    set_tile_scale(int scale);      // Sets a new tile scale.
    bool    shader() const;                 // Checks if we're using the GLSL shader.
    bool    shader_geom() const;            // Checks if the shader is using geometry deforming or not.
    int     tile_scale() const;             // Retrieves the tile scaling factor.

private:
    bool    auto_rescale_;  // Are we auto-rescaling as the window size changes?
    bool    shader_;        // Are we using the GLSL shader?
    bool    shader_geom_;   // Does the shader apply curved CRT geometry?
    int     tile_scale_;    // The size that tiles are scaled on the screen.
};

Prefs& prefs(); // Easier access than calling core().prefs()

};
