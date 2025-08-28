// core/terminal/terminal.hpp -- The Terminal class handles rendering a faux-terminal using SFML.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <map>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "core/global.hpp"

namespace gorp {

class Window;           // defined in core/terminal/window.hpp

class Terminal {
public:
    // Don't fuck with this, it could have very, very disastrous effects. Just leave it alone forever.
    static constexpr int    TILE_SIZE = 8;  // The size of the font/tiles used in the game.

                Terminal();     // Constructor, sets up default values but does not initialize the faux-terminal.
                ~Terminal();    // Destructor, ensures memory is freed in a predictable order.
    int         get_key();      // Gets keyboard input from the user.
    Vector2u    get_middle() const; // Gets the central column and row of the screen.
    void        set_frame_limit(bool enable);   // Enables or disables the frame-limiting on rendering.
    Vector2u    size() const;   // Determines the size of the screen, in character width and height, taking tiles obscured by the shader into account.
    Vector2u    size_pixels() const;    // Gets the raw size of the screen in pixels, without any adjustments.

    Window* add_window(Vector2u new_size, Vector2 new_pos = {0, 0});    // Adds a new Window to the stack.
    void    remove_window(Window* win);     // Removes a Window from the stack. This is called automatically from Window's destructor.
    void    window_to_front(Window* win);   // Pushes a window to the top of the stack.

private:
    // Internal rendering code, called by Window::print() and Window::put(), with the complex part handled by Terminal.
    void        print(sf::RenderTexture &tex, std::string str, Vector2 pos, Colour colour, Font font = Font::NORMAL);
    void        put(sf::RenderTexture &tex, int ch, Vector2 pos, Colour colour, Font font = Font::NORMAL);

    // Other functions that are only used internally by Terminal.
    void        flip(bool update_screen = true);    // Refreshes the terminal after rendering. This is called automatically before the event loop.
    sf::Image   load_png(const std::string &filename);  // Loads a PNG from the data files.
    void        load_sprites();     // Load the sprites from the static data.
    void        recreate_frames();  // Recreates the frame textures, after the window has resized.

    std::unique_ptr<sf::RenderTexture>  current_frame_, previous_frame_; // This is where we render updates to the screen, before applying the shader.
    sf::RenderWindow            main_window_;   // The main render window.
    sf::Shader                  shader_;        // The CRT shader.
    uint32_t                    sprite_max_;    // The maximim valid ID for a sprite on the sprite sheet.
    sf::Texture                 sprite_sheet_;  // The sprite sheet texture.
    Vector2u                    sprite_sheet_size_; // The size of the sprite sheet, in pixels.
    Vector2u                    window_pixels_; // The main window size, in pixels.
    std::vector<std::unique_ptr<Window> >        window_stack_;  // The current stack of Windows to render.

friend class Window;
};

Terminal& terminal();   // Easier access than calling core()->terminal()

}   // namespace gorp
