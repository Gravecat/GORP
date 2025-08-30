// ui/dev-canvas.cpp -- The DevCanvas UI element provides a very simple 'canvas' window that can be moved around with the arrow keys. This is intended for use
// during development only, for previewing data or doing other testing.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/game.hpp"
#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/dev-canvas.hpp"

namespace gorp {

// Creates a new DevCanvas of the specified size, in tiles.
DevCanvas::DevCanvas(Vector2u size) : Element(), size_(size)
{
    if (!size.x || !size.y) throw GuruMeditation("Invalid DevCanvas size", size.x, size.y);
    recreate_window();
}

// Clears the canvas entirely.
void DevCanvas::clear(Colour col) { window_->clear(col); needs_redraw(); }

// Prints a string.
void DevCanvas::print(std::string str, Vector2 pos, Colour colour, Font font) { window_->print(str, pos, colour, font); }

// Processes keyboard input from the player.
bool DevCanvas::process_input(int key)
{
    int move_x = 0, move_y = 0;
    switch (key)
    {
        case Key::ARROW_UP: case 'w': case 'W': move_y = -1; break;
        case Key::ARROW_DOWN: case 's': case 'S': move_y = 1; break;
        case Key::ARROW_LEFT: case 'a': case 'A': move_x -= 1; break;
        case Key::ARROW_RIGHT: case 'd': case 'D': move_x += 1; break;
        case Key::ESCAPE: game().delete_element(id()); return true;
        case Key::TAB: game().element_to_back(id(), 2); return true;
        default: return false;
    }
    window_->move(Vector2(window_->pos().x + move_x, window_->pos().y + move_y));
    return true;
}

// Writes a character on the canvas.
void DevCanvas::put(int ch, Vector2 pos, Colour colour, Font font) { window_->put(ch, pos, colour, font); }

// As above, but using a Glyph enum.
void DevCanvas::put(Glyph gl, Vector2 pos, Colour colour, Font font) { window_->put(gl, pos, colour, font); }

// (Re)creates the render window for this canvas.
void DevCanvas::recreate_window()
{
    // We're breaking protocol here. In order to keep this canvas intact, we're not gonna delete and recreate it.
    // Instead, we'll just push it to the top of the stack.
    if (!window_)
    {
        window_ = terminal().add_window(size_);
        window_->clear();
    }
    else terminal().window_to_front(window_);
}

// Erases one or more tiles, or draws a coloured rectangle.
void DevCanvas::rect(Vector2 pos, Vector2u size, Colour col) { window_->rect(pos, size, col); }

}   // namespace gorp
