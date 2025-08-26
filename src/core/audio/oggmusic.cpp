// core/audio/oggmusic.hpp -- Handles loading and playing of looping .ogg music through SFML's audio interface.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/audio/oggmusic.hpp"
#include "core/core.hpp"
#include "util/file/fileutils.hpp"

namespace gorp
{

// Constructor, loads a specified ogg file into memory.
OggMusic::OggMusic(const std::string &filename)
{
    std::string full_filename = core().datafile("ogg/" + filename + ".ogg");
    if (!fileutils::file_exists(full_filename)) throw std::runtime_error("Missing audio file: " + filename + ".ogg");
    music_vec_ = fileutils::file_to_char_vec(full_filename);
    music_ = std::make_unique<sf::Music>();
    if (!music_->openFromMemory(music_vec_.data(), music_vec_.size())) throw std::runtime_error("Cannot load music file: " + filename + ".ogg");
}

// Destructor, cleans up used memory.
OggMusic::~OggMusic() { music_.reset(nullptr); }

// Starts the music playing.
void OggMusic::play() { music_->play(); }

// Sets whether or not this music loops.
void OggMusic::set_looping(bool looping) { music_->setLooping(looping); }

// Sets the volume of the played music.
void OggMusic::set_volume(float volume) { music_->setVolume(volume); }

// Stops the music from playing.
void OggMusic::stop() { music_->stop(); }

}
