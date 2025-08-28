// ui/dev-canvas.hpp -- The DevCanvas UI element provides a very simple 'canvas' window that can be moved around with the arrow keys. This is intended for use
// during development only, for previewing data or doing other testing.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"
#include "ui/element.hpp"

namespace gorp {

class DevCanvas : public Element
{
public:
            DevCanvas() = delete;       // No default constructor; must specify window size.
            DevCanvas(bool) = delete;   // Bool constructor from Element is also deleted.
            DevCanvas(Vector2u size);   // Creates a new DevCanvas of the specified size, in tiles.
    void    clear(Colour col = Colour::BLACK);  // Clears the canvas entirely.
    void    print(std::string str, Vector2 pos, Colour colour = Colour::WHITE, Font font = Font::NORMAL);   // Prints a string.
    bool    process_input(int key) override;    // Processes keyboard input from the player.
    void    put(int ch, Vector2 pos, Colour colour = Colour::WHITE, Font font = Font::NORMAL);      // Writes a character on the canvas.
    void    put(Glyph gl, Vector2 pos, Colour colour = Colour::WHITE, Font font = Font::NORMAL);    // As above, but using a Glyph enum.
    void    recreate_window() override; // (Re)creates the render window for this canvas.
    void    rect(Vector2 pos, Vector2u size = {1, 1}, Colour col = Colour::BLACK);  // Erases one or more tiles, or draws a coloured rectangle.
    void    render() override { }       // Technically re-renders this Element, but we're doing it in a nonstandard way, so it does nothing.

private:
    Vector2u    size_;  // The size of this canvas.
};

}   // namespace gorp
