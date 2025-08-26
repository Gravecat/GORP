// core/audio/oggsound.hpp -- Handles loading and playing of .ogg sound effects through SFML's audio interface.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"
#include "SFML/Audio.hpp"

namespace gorp {

class OggSound {
public:
            OggSound() = delete;    // No default constructor.
            OggSound(const std::string &filename);  // Constructor, loads a specified ogg file into memory.
            ~OggSound();    // Destructor, cleans up used memory.
    void    play();         // Plays the sound effect.
    void    set_playing_offset(sf::Time offset);    // Sets the offset for how far along this sample is playing.

private:
    std::unique_ptr<sf::Sound>  sound_ptr_; // The sf::Sound object, which contains additional data about the sound.
    sf::SoundBuffer sound_buffer_;          // The sound buffer -- this is where the sound data is stored in memory.
};

}   // namespace gorp
