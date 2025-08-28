// core/terminal/window.hpp -- Used in conjunction with Terminal to provide virtual 'windows', rendering surfaces that can be painted on and moved around.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

#include "core/global.hpp"

namespace gorp {

class Window {
    public:
                    Window() = delete;  // No default constructor.
                    Window(Vector2u new_size, Vector2 new_pos = {0, 0}); // Creates a new Window of the specified size and position.
                    ~Window();  // Destructor, explicitly frees memory used.
        void        box(Colour colour = Colour::WHITE); // Draws a box around a Window.
        void        clear(Colour col = Colour::BLACK);  // Clears/fills a Window.
        Vector2u    get_middle() const; // Gets the central column and row of this Window.
        void        move(Vector2 new_pos);  // Moves this Window to new coordinates.
        void        move(Vector2u new_pos); // As above, with an unsigned Vector2.
        Vector2     pos() const;    // Read-only access to the Window's position.
        void        print(std::string str, Vector2 pos, Colour colour = Colour::WHITE, Font font = Font::NORMAL);   // Prints a string.
        void        put(int ch, Vector2 pos, Colour colour = Colour::WHITE, Font font = Font::NORMAL);      // Writes a character on the Window.
        void        put(Glyph gl, Vector2 pos, Colour colour = Colour::WHITE, Font font = Font::NORMAL);    // As above, but using a Glyph enum.
        void        rect(Vector2 pos, Vector2u size = {1, 1}, Colour col = Colour::BLACK);  // Erases one or more tiles, or draws a coloured rectangle.
        sf::RenderTexture&  render_texture();   // Retrieves the SFML render texture for this Window.
        Vector2u    size() const;   // Read-only access to the Window's size.
    
    private:
        Vector2 pos_;   // The position of this Window on the screen.
        std::unique_ptr<sf::RenderTexture>  render_texture_;    // The SFML render texture for this Window.
        Vector2u size_; // The width and height of this Window, in tiles.
    };

}   // namespace gorp
