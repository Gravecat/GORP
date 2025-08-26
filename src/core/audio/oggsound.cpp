// core/audio/oggsound.cpp -- Handles loading and playing of .ogg sound effects through SFML's audio interface.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/audio/oggsound.hpp"
#include "core/core.hpp"
#include "util/file/fileutils.hpp"

namespace gorp {

// Constructor, loads a specified ogg file into memory.
OggSound::OggSound(const std::string &filename)
{
    std::string full_filename = core().datafile("ogg/" + filename + ".ogg");
    if (!fileutils::file_exists(full_filename)) throw std::runtime_error("Missing audio file: " + filename + ".ogg");
    std::vector<char> ogg_data = fileutils::file_to_char_vec(full_filename);
    if (!sound_buffer_.loadFromMemory(ogg_data.data(), ogg_data.size())) throw std::runtime_error("Could not load audio file: " + filename + ".ogg");
    sound_ptr_ = std::make_unique<sf::Sound>(sound_buffer_);
    ogg_data.clear();
}

// Destructor, cleans up used memory.
OggSound::~OggSound()
{
    sound_ptr_->stop();
    sound_ptr_.reset(nullptr);
}

// Plays the sound effect.
void OggSound::play() { sound_ptr_->play(); }

// Sets the offset for how far along this sample is playing.
void OggSound::set_playing_offset(sf::Time offset) { sound_ptr_->setPlayingOffset(offset); }

}   // namespace gorp
