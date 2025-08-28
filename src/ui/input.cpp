// ui/input.cpp -- The input window is where the player types text commands to the game.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/game.hpp"
#include "core/prefs.hpp"
#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/input.hpp"

namespace gorp {

// Constructor, sets up the input window.
Input::Input() : cursor_blink_(true), input_("> ")
{
    always_redraw(true);
    recreate_window();
}

// Processes keyboard input from the player.
bool Input::process_input(int key)
{
    bool alphanumeric = (key >= ' ' && key <= '~');
    if (key == '{' || key == '}') alphanumeric = false;
    if (alphanumeric)
    {
        if (input_.size() < MAX_INPUT_LENGTH && !(key == ' ' && input_.at(input_.size() - 1) == ' ')) input_ += static_cast<char>(key);
        return true;
    }
    else switch(key)
    {
        case Key::BACKSPACE:
            if (input_.size() > 2) input_ = input_.substr(0, input_.size() - 1);
            return true;
        case Key::ENTER:
            if (input_.size() > 2)
            {
                if (input_.at(input_.size() - 1) == ' ') game().process_input(input_.substr(2, input_.size() - 3));
                else game().process_input(input_.substr(2));
                input_ = "> ";
            }
            return true;
        default: return false;
    }
}

// (Re)creates the input windw.
void Input::recreate_window()
{
    Terminal &term = terminal();
    const Vector2u term_size = term.size();
    if (window_) term.remove_window(window_);
    window_ = term.add_window(Vector2u(std::max(5u, term_size.x), 3), Vector2(0, term_size.y - 3));
}

// Renders the input window.
void Input::render()
{
    const Colour box_colour = (prefs().shader() ? Colour::WHITE : Colour::GRAY);

    window_->clear();
    window_->box(box_colour);
    window_->put(Glyph::BOX_LVR, Vector2(0, 0), box_colour);
    window_->put(Glyph::BOX_LVL, Vector2(window_->size().x - 1, 0), box_colour); 

    const unsigned int max_width = window_->size().x - 4;
    unsigned int cursor_pos = input_.size() + 1;
    unsigned int input_begin = 0;
    if (input_.size() >= max_width + 1)
    {
        input_begin = input_.size() - max_width - 1;
        cursor_pos = window_->size().x - 2;
    }
    std::string output = input_.substr(input_begin);
    window_->print(output, Vector2(1, 1), Colour::GREEN);

    if (cursor_blink_) window_->put(Glyph::FULL_BLOCK, Vector2(cursor_pos, 1), Colour::GREEN);
    int cursor_blink_time = (cursor_blink_ ? 1000 : 500);
    if (blink_timer_.getElapsedTime().asMilliseconds() > cursor_blink_time)
    {
        cursor_blink_ = !cursor_blink_;
        blink_timer_.restart();
    }
}

}   // namespace gorp
