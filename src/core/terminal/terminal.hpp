// core/terminal/terminal.hpp -- The Terminal class handles rendering a faux-terminal using SFML.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

#include <map>

namespace gorp {

class SfxrSample;       // defined in 3rdparty/sfxr/sfxr.hpp
class SfxrSoundStream;  // defined in 3rdparty/sfxr/sfxr.hpp
class Window;           // defined in core/terminal/window.hpp

class Terminal {
public:
    // Don't fuck with this, it could have very, very disastrous effects. Just leave it alone forever.
    static constexpr int    TILE_SIZE = 8;  // The size of the font/tiles used in the game.

            Terminal();     // Constructor, sets up default values but does not initialize the faux-terminal.
            ~Terminal();    // Destructor, ensures memory is freed in a predictable order.
    int     get_key();      // Gets keyboard input from the user.
    Vector2 get_middle() const; // Gets the central column and row of the screen.
    void    play_sound(const std::string &sound);   // Plays an sfxr sound effect.
    void    set_frame_limit(bool enable);   // Enables or disables the frame-limiting on rendering.
    Vector2 size() const;   // Determines the size of the screen, in character width and height, taking tiles obscured by the shader into account.
    Vector2 size_pixels() const;    // Gets the raw size of the screen in pixels, without any adjustments.

    Window* add_window(Vector2 new_size, Vector2 new_pos = {0, 0});    // Adds a new Window to the stack.
    void    remove_window(Window* win); // Removes a Window from the stack. This is called automatically from Window's destructor.

private:
    // Internal rendering code, called by Window::print() and Window::put(), with the complex part handled by Terminal.
    void        print(sf::RenderTexture &tex, std::string str, Vector2 pos, Colour colour, Font font = Font::NORMAL);
    void        put(sf::RenderTexture &tex, int ch, Vector2 pos, Colour colour, Font font = Font::NORMAL);

    // Other functions that are only used internally by Terminal.
    void        flip(bool update_screen = true);    // Refreshes the terminal after rendering. This is called automatically before the event loop.
    sf::Image   load_png(const std::string &filename);  // Loads a PNG from the data files.
    void        load_sfxr();        // Loads the sfxr sound samples.
    void        load_sprites();     // Load the sprites from the static data.
    void        recreate_frames();  // Recreates the frame textures, after the window has resized.
    Vector2     render_offset() const;  // Applies an offset to rendering, to avoid useless (obscured by the bevel) tiles.
    void        set_shader_mode(uint8_t mode);  // Sets a new shader mode (see shader_mode_ in prefs.hpp for the valid modes).
    bool        shader_geom() const;    // Returns true if the current shader mode affects the screen geometry, false if not.

    std::unique_ptr<sf::RenderTexture>  current_frame_, previous_frame_; // This is where we render updates to the screen, before applying the shader.
    std::unique_ptr<sf::Sound>  degauss_sound_; // The processed CRT degauss sound effect. Requires the buffer below.
    sf::SoundBuffer             degauss_sound_buffer_;  // The CRT degauss sound effect.
    sf::RenderWindow            main_window_;   // The main render window.
    std::unique_ptr<SfxrSoundStream>    sfxr_;  // The SfxrSoundStream, which uses sfxr and SFML to play simple audio.
    std::map<std::string, SfxrSample>   sfxr_samples_;  // The stored sfxr sample data.
    sf::Shader                  shader_;        // The CRT shader.
    uint32_t                    sprite_max_;    // The maximim valid ID for a sprite on the sprite sheet.
    sf::Texture                 sprite_sheet_;  // The sprite sheet texture.
    Vector2                     sprite_sheet_size_; // The size of the sprite sheet, in pixels.
    Vector2                     window_pixels_; // The main window size, in pixels.
    std::vector<std::unique_ptr<Window> >        window_stack_;  // The current stack of Windows to render.

friend class Window;
};

// Easier access than calling core()->terminal()
Terminal& terminal();

}   // namespace gorp
