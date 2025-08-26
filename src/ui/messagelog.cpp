// ui/messagelog.cpp -- The message log window is the player's primary interface with the game world, telling the player all the information they need to know.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/game.hpp"
#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/messagelog.hpp"
#include "util/stringutils.hpp"

namespace gorp {

// Constructor, sets things up.
MessageLog::MessageLog() { recreate_window(); }

// Adds a string to the message log.
void MessageLog::message(const std::string &msg)
{
    log_unprocessed_.push_back(msg);
    while (log_unprocessed_.size() >= MAX_UNPROCESSED_MESSAGES)
        log_unprocessed_.erase(log_unprocessed_.begin());
    process_messages();
    needs_redraw(true);
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
}

// (Re)creates the render window.
void MessageLog::recreate_window()
{
    Terminal &term = terminal();
    const Vector2 term_size = term.size();
    if (window_) term.remove_window(window_);
    window_ = term.add_window(Vector2(term_size.x, term_size.y - 2), Vector2(0, 0));
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
        window_->print(log_processed_.at(i), {1, end_line}, Colour::GRAY);
        if (--end_line <= 0) break;
    }
}

// Easier access than using game().log()
MessageLog& log() { return game().log(); }

}   // namespace gorp
