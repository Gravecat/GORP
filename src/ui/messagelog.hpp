// ui/messagelog.cpp -- The message log window is the player's primary interface with the game world, telling the player all the information they need to know.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"
#include "ui/element.hpp"

namespace gorp {

class MessageLog : public Element {
public:
            MessageLog();   // Constructor, sets up the message log window.
    void    message(const std::string &str);    // Adds a string to the message log.
    bool    process_input(int key) override;    // Processes keyboard input from the player.
    void    recreate_window() override;         // (Re)creates the render window.
    void    render() override;                  // Renders the message log window.

private:
    void    process_messages(); // Formats the messages in the log to fit in the window.

    static constexpr int    MAX_UNPROCESSED_MESSAGES = 200; // The maximum amount of unprocessed lines before we start deleting older ones.
    static constexpr int    PAGE_SCROLL =   8;      // How many lines of text are scrolled with PageUp/PageDown.

    std::vector<std::string>    log_processed_;     // The formatted strings from log_unprocessed_ below.
    std::vector<std::string>    log_unprocessed_;   // The unprocessed/unformatted lines in the message log.
    unsigned int                max_offset_;        // The precalculated maximum offset value for the message log scrolling.
    unsigned int                offset_;            // The offset position of the message log.
};

void msg(const std::string &str = "");  // Easier access than using game().log().message()

}   // namespace gorp
