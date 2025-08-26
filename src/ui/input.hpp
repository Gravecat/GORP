// ui/input.hpp -- The input window is where the player types text commands to the game.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <SFML/System.hpp>
#include "core/global.hpp"
#include "ui/element.hpp"

namespace gorp {

class Input : public Element
{
public:
            Input();    // Constructor, sets up the input window.
    bool    process_input(int key) override;    // Processes keyboard input from the player.
    void    recreate_window() override; // (Re)creates the input windw.
    void    render() override;          // Renders the input window.

private:
    static constexpr int    MAX_INPUT_LENGTH =  255;    // The maximum length of input that the player can type.

    sf::Clock   blink_timer_;   // The timer for the cursor-blink.
    bool        cursor_blink_;  // Is the cursor-blink currently on?
    std::string input_;         // The current input from the player, as it's typed.
};

}   // namespace gorp
