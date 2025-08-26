// ui/title.hpp -- The title screen, displays the main menu and such.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {

class OggSound; // defined in core/audio/oggsound.hpp
class OggMusic; // defined in core/audio/oggmusic.hpp
class Window;   // defined in core/terminal/window.hpp

class TitleScreen {
public:
    enum class TitleOption : uint8_t { NEW_GAME, LOAD_GAME, QUIT };

                TitleScreen();  // Initializes the title screen by loading static data.
                ~TitleScreen(); // Destructor, cleans up used memory.
    TitleOption render();       // Renders the title screen, and returns the user's chosen action.

private:
    void    load_title_data();  // Loads the backronym and random phrase for the title screen.
    void    redraw();           // Redraws the title screen.
    void    render_test();      // Render speed test.

    std::string                 backronym_;             // The randomly-assembled 'backronym' for GORP chosen this time around.
    bool                        blinking_;              // Whether or not the title-screen dragon is blinking.
    bool                        floppy_played_;         // Has the floppy-disk sound effect played yet?
    std::unique_ptr<OggSound>   floppy_sound_;          // The floppy-disk loading sound.
    std::unique_ptr<OggMusic>   music_;                 // The title-screen music.
    std::string                 phrase_;                // The randomly-chosen phrase for the title screen.
    Window*                     title_screen_window_;   // The window where we render the title screen.
};

}   // namespace gorp
