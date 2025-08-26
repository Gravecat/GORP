// ui/input.cpp -- The input window is where the player types text commands to the game.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/input.hpp"

namespace gorp {

// Constructor, sets up the input window.
Input::Input() { recreate_window(); }

// (Re)creates the input windw.
void Input::recreate_window()
{
    Terminal &term = terminal();
    const Vector2 term_size = term.size();
    if (window_) term.remove_window(window_);
    window_ = term.add_window(Vector2(term_size.x, 3), Vector2(0, term_size.y - 3));
}

// Renders the input window.
void Input::render()
{
    window_->clear();
    window_->box();
    window_->put(Glyph::BOX_LVR, Vector2(0, 0));
    window_->put(Glyph::BOX_LVL, Vector2(window_->size().x - 1, 0));
}

}   // namespace gorp
