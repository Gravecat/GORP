// core/audio/sfxr.cpp -- Loads and plays sound effects with sfxr.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "3rdparty/sfxr/sfxr.hpp"
#include "core/audio/sfxr.hpp"
#include "core/core.hpp"
#include "util/file/filereader.hpp"
#include "util/file/fileutils.hpp"

namespace gorp {

// Constructor, loads sound effects into memory.
Sfxr::Sfxr()
{
    core().log("Loading sfxr sound files...");
    sfxr_stream_ = std::make_unique<SfxrSoundStream>();
    const std::string sfxr_dir = core().datafile("sfxr");
    std::vector<std::string> sfxr_files = fileutils::files_in_dir(sfxr_dir);

    for (unsigned int i = 0; i < sfxr_files.size(); i++)
    {
        const std::string filename = sfxr_files.at(i);
        FileReader fr(core().datafile("sfxr/" + filename));

        const int version = fr.read_data<int>();
        if (version < 100 || version > 102) throw GuruMeditation(filename + ": Invalid sfxr file version!", version, i);

        SfxrSample samp;
        samp.wave_type = fr.read_data<int>();
        if (version == 102) samp.sound_vol = fr.read_data<float>();
        samp.p_base_freq = fr.read_data<float>();
        samp.p_freq_limit = fr.read_data<float>();
        samp.p_freq_ramp = fr.read_data<float>();
        if (version >= 101) samp.p_freq_dramp = fr.read_data<float>();
        samp.p_duty = fr.read_data<float>();
        samp.p_duty_ramp = fr.read_data<float>();
        samp.p_vib_strength = fr.read_data<float>();
        samp.p_vib_speed = fr.read_data<float>();
        samp.p_vib_delay = fr.read_data<float>();
        samp.p_env_attack = fr.read_data<float>();
        samp.p_env_sustain = fr.read_data<float>();
        samp.p_env_decay = fr.read_data<float>();
        samp.p_env_punch = fr.read_data<float>();
        samp.filter_on = fr.read_data<bool>();
        samp.p_lpf_resonance = fr.read_data<float>();
        samp.p_lpf_freq = fr.read_data<float>();
        samp.p_lpf_ramp = fr.read_data<float>();
        samp.p_hpf_freq = fr.read_data<float>();
        samp.p_hpf_ramp = fr.read_data<float>();
        samp.p_pha_offset = fr.read_data<float>();
        samp.p_pha_ramp = fr.read_data<float>();
        samp.p_repeat_speed = fr.read_data<float>();
        if (version >= 101)
        {
            samp.p_arp_speed = fr.read_data<float>();
            samp.p_arp_mod = fr.read_data<float>();
        }
        sfxr_samples_.insert({filename, samp});
    }
    core().log("Sound effects loaded successfully.");
}

// Destructor, cleans up used memory.
Sfxr::~Sfxr() { sfxr_stream_.reset(nullptr); }

// Plays an sfxr sound effect.
void Sfxr::play_sound(const std::string &sound)
{
    const auto result = sfxr_samples_.find(sound + ".sfs");
    if (result == sfxr_samples_.end()) throw std::runtime_error("Missing sound file: " + sound + ".sfs");

    sfxr_stream_->stop();
    sfxr_stream_->load_settings(result->second);
    sfxr_stream_->play_sample();
}

// Easier access than calling core()->sfxr()
Sfxr& sfxr() { return core().sfxr(); }

}   // namespace gorp
