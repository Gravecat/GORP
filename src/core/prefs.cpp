// core/prefs.cpp -- Allows for simple user preferences to be stored and retrieved on future executions.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/core.hpp"
#include "core/prefs.hpp"

namespace gorp {

// Constructor, sets default values.
Prefs::Prefs() : FileReader("userdata/prefs.dat", true), FileWriter(), auto_rescale_(true), shader_mode_(1), tile_scale_(2)
{
    if (!data_.size())  // No prefs file right now, so go with default values.
    {
        save_prefs();
        return;  
    }

    bool header_good = false;
    char header = read_data<char>();
    if (header == 'K')
    {
        header = read_data<char>();
        if (header == '8') header_good = true;
    }
    if (!header_good)
    {
        core().log("prefs.dat corrupted or invalid.");
        clear_data();
        save_prefs();
        return;
    }

    const uint32_t file_ver = read_data<uint32_t>();
    if (file_ver != PREFS_VERSION)  // If the version doesn't match, we'll just go back to defaults.
    {
        clear_data();
        save_prefs();
        return;
    }

    const uint8_t flags_a = read_data<uint8_t>();
    auto_rescale_ = flags_a & 1;

    shader_mode_ = read_data<uint8_t>();
}

// Checks if the tile scale changes automatically when the window resizes.
bool Prefs::auto_rescale() const { return auto_rescale_; }

// Clears the loaded data once it's been processed.
void Prefs::clear_data() { data_.clear(); }

// Saves the prefs file to disk.
void Prefs::save_prefs()
{
    open_file("userdata/prefs.dat");
    write_data<char>('K');
    write_data<char>('8');
    write_data<uint32_t>(PREFS_VERSION);

    uint8_t flags_a = (auto_rescale_ ? 1 : 0);
    write_data<uint8_t>(flags_a);

    write_data<uint8_t>(shader_mode_);

    close_file();
}

// Easier access than calling core().prefs()
Prefs& prefs() { return core().prefs(); }

// Sets whether or not the tile scale auto-changes on window resize.
void Prefs::set_auto_rescale(bool toggle) { auto_rescale_ = toggle; save_prefs(); }

// Sets the current shader mode (see shader_mode_ below for values).
void Prefs::set_shader_mode(uint8_t mode)
{
    if (mode > 2) throw GuruMeditation("Invalid shader mode requested", mode, 2);
    shader_mode_ = mode;
}

// Sets a new tile scale.
void Prefs::set_tile_scale(int scale)
{
    if (scale < 1 || scale > 255) throw std::runtime_error("Invalid tile scale factor!");
    tile_scale_ = scale;
}

// Returns the shader mode (see shader_mode_ in prefs.hpp for values).
uint8_t Prefs::shader_mode() const { return shader_mode_; }

// Retrieves the tile scaling factor.
int Prefs::tile_scale() const { return tile_scale_; }

};
