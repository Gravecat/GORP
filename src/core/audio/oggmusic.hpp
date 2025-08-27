// core/audio/oggmusic.hpp -- Handles loading and playing of looping .ogg music through SFML's audio interface.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"
#include "SFML/Audio/Music.hpp"

namespace gorp {

class OggMusic {
public:
            OggMusic() = delete;    // No default constructor.
            OggMusic(const std::string &filename);  // Constructor, loads a specified ogg file into memory.
            ~OggMusic();    // Destructor, cleans up used memory.
    void    play();         // Starts the music playing.
    void    set_looping(bool looping);  // Sets whether or not this music loops.
    void    set_volume(float volume);   // Sets the volume of the played music.
    void    stop();         // Stops the music from playing.

private:
    std::unique_ptr<sf::Music>  music_; // A music file loaded into sf::Music.
};

}   // namespace gorp
