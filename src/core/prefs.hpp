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
    static constexpr uint32_t   PREFS_VERSION = 4;  // The version changes when data files are no longer compatible.

            Prefs();                        // Constructor, sets default values.
    bool    ascii() const;                  // Checks if we're using ASCII glyphs.
    bool    auto_rescale() const;           // Checks if the tile scale changes automatically when the window resizes.
    void    clear_data();                   // Clears the loaded data once it's been processed.
    void    save_prefs();                   // Saves the prefs file to disk.
    void    set_auto_rescale(bool toggle);  // Sets whether or not the tile scale auto-changes on window resize.
    void    set_shader_mode(uint8_t mode);  // Sets the current shader mode (see shader_mode_ below for values).
    void    set_tile_scale(int scale);      // Sets a new tile scale.
    uint8_t shader_mode() const;            // Returns the shader mode (see shader_mode_ below for values).
    int     tile_scale() const;             // Retrieves the tile scaling factor.

private:
    bool    auto_rescale_;  // Are we auto-rescaling as the window size changes?
    uint8_t shader_mode_;   // The current shader mode (0 = no shader, 1 = full shader with bezel, 2 = full shader without bezel, 2 = shader with no CRT
                            // geometry distortion, vignette or bezel)
    int     tile_scale_;    // The size that tiles are scaled on the screen.
};

Prefs& prefs(); // Easier access than calling core().prefs()

};
