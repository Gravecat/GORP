// core/audio/sfxr.hpp -- Loads and plays sound effects with sfxr.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <map>

#include "core/global.hpp"

namespace gorp {

class SfxrSample;       // defined in 3rdparty/sfxr/sfxr.hpp
class SfxrSoundStream;  // defined in 3rdparty/sfxr/sfxr.hpp

class Sfxr {
public:
            Sfxr();     // Constructor, loads sound effects into memory.
            ~Sfxr();    // Destructor, cleans up used memory.
    void    play_sound(const std::string &sound);   // Plays an sfxr sound effect.

private:
    std::unique_ptr<SfxrSoundStream>    sfxr_stream_;   // The SfxrSoundStream, which uses sfxr and SFML to play simple audio.
    std::map<std::string, SfxrSample>   sfxr_samples_;  // The stored sfxr sample data.
};

Sfxr& sfxr();   // Easier access than calling core()->sfxr()

}   // namespace gorp
