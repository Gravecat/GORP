// ui/messagelog.cpp -- The message log window is the player's primary interface with the game world, telling the player all the information they need to know.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/audio/sfxr.hpp"
#include "core/game.hpp"
#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/messagelog.hpp"
#include "util/text/stringutils.hpp"

namespace gorp {

// Constructor, sets things up.
MessageLog::MessageLog() { recreate_window(); }

// Adds a string to the message log.
void MessageLog::message(const std::string &msg)
{
    offset_ = 0;
    log_unprocessed_.push_back(msg);
    while (log_unprocessed_.size() >= MAX_UNPROCESSED_MESSAGES)
        log_unprocessed_.erase(log_unprocessed_.begin());
    process_messages();
    needs_redraw(true);
}

// Processes keyboard input from the player.
bool MessageLog::process_input(int key)
{
    unsigned int scroll_amount = 1;

    switch (key)
    {
        case Key::ARROW_DOWN:
        case Key::PAGE_DOWN:
        case Key::END:
            if (offset_)
            {
                if (key == Key::PAGE_DOWN) scroll_amount = PAGE_SCROLL;
                else if (key == Key::END) scroll_amount = 1000000;
                if (offset_ < scroll_amount) offset_ = 0;
                else offset_ -= scroll_amount;
                needs_redraw(true);
            }
            return true;
        case Key::ARROW_UP:
        case Key::PAGE_UP:
        case Key::HOME:
            if (offset_ < max_offset_)
            {
                if (key == Key::PAGE_UP) scroll_amount = PAGE_SCROLL;
                else if (key == Key::HOME) scroll_amount = 1000000;
                if (offset_ + scroll_amount >= max_offset_) offset_ = max_offset_;
                else offset_ += scroll_amount;
                needs_redraw(true);
            }
            return true;
    }
    return false;
}

// Formats the messages in the log to fit in the window.
void MessageLog::process_messages()
{
    log_processed_.clear();

    for (auto str : log_unprocessed_)
    {
        std::vector<std::string> split = stringutils::ansi_vector_split(str, window_->size().x - 2);
        if (!split.empty()) log_processed_.insert(log_processed_.end(), split.begin(), split.end());
    }

    if (log_processed_.size() <= static_cast<unsigned int>(window_->size().y) - 2) max_offset_ = 0;
    else max_offset_ = log_processed_.size() - (window_->size().y - 2);
}

// (Re)creates the render window.
void MessageLog::recreate_window()
{
    offset_ = 0;
    Terminal &term = terminal();
    const Vector2 term_size = term.size();
    if (window_) term.remove_window(window_);
    window_ = term.add_window(Vector2(std::max(5, term_size.x), std::max(3, term_size.y - 2)), Vector2(0, 0));
    process_messages();
}

// Renders the message log window.
void MessageLog::render()
{
    const uint16_t window_h = window_->size().y;
    window_->clear();
    window_->box();
    window_->put(Glyph::BOX_LVR, Vector2(0, window_->size().y - 1));
    window_->put(Glyph::BOX_LVL, Vector2(window_->size().x - 1, window_->size().y - 1));

    int end_line = window_h - 2;
    if (log_processed_.size() < static_cast<unsigned int>(end_line)) end_line = log_processed_.size();
    for (int i = log_processed_.size() - 1; i >= 0; i--)
    {
        window_->print(log_processed_.at(i - offset_), {1, end_line}, Colour::GRAY);
        if (--end_line <= 0) break;
    }
}

// Easier access than using game().log().message()
void msg(const std::string &str) { game().log().message(str); }

}   // namespace gorp
