// misc/title.cpp -- The title screen, displays the main menu and such.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "cmake/version.hpp"
#include "core/core.hpp"
#include "core/game.hpp"
#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/title.hpp"
#include "util/file/fileutils.hpp"
#include "util/file/yaml.hpp"
#include "util/math/random.hpp"

namespace gorp {

// Simple constructor, sets things up.
TitleScreen::TitleScreen() : blinking_(false), title_screen_window_(nullptr)
{
    YAML title_data(core().datafile("misc/title.yml"));
    if (!title_data.is_map()) throw GuruMeditation("misc/title/yml: Invalid file format");

    // Load the backronym and random phrase for the title screen.
    std::vector<std::string> g_words = title_data.get_seq("g_words");
    std::vector<std::string> r_words = title_data.get_seq("r_words");
    std::vector<std::string> p_words = title_data.get_seq("p_words");
    std::vector<std::string> phrases = title_data.get_seq("phrases");
    backronym_ = g_words.at(random::get<int>(0, g_words.size() - 1)) + " of " + r_words.at(random::get<int>(0, r_words.size() - 1)) + " " +
        p_words.at(random::get<int>(0, p_words.size() - 1));
    phrase_ = phrases.at(random::get<int>(0, phrases.size() - 1));
}

// Destructor, cleans up used memory.
TitleScreen::~TitleScreen() { if (title_screen_window_) terminal().remove_window(title_screen_window_); }

// Renders the title screen, and returns the user's chosen action.
TitleScreen::TitleOption TitleScreen::render()
{
    Terminal &term = terminal();
    sf::Clock blink_timer;
    int next_blink = random::get<int>(2000, 10000);
    redraw();

    int result;
    while(true)
    {
        if (blinking_ && blink_timer.getElapsedTime().asMilliseconds() > 200)
        {
            blink_timer.restart();
            blinking_ = false;
            redraw();
        }
        else if (!blinking_ && blink_timer.getElapsedTime().asMilliseconds() > next_blink)
        {
            blink_timer.restart();
            blinking_ = true;
            next_blink = random::get<int>(2000, 10000);
            redraw();
        }

        result = term.get_key();
        switch(result)
        {
            case '1': return TitleOption::NEW_GAME;
            case '3': return TitleOption::QUIT;
            case Key::RESIZE: redraw(); break;
            case Key::F12: render_test(); redraw(); break;
        }
    }
    return TitleOption::QUIT;
}

// Redraws the title screen.
void TitleScreen::redraw()
{
    Terminal &term = terminal();
    if (title_screen_window_) term.remove_window(title_screen_window_);
#if defined(GORP_BUILD_DEBUG)
    const unsigned int title_height = 29;
#else
    const unsigned int title_height = 27;
#endif
    title_screen_window_ = term.add_window({43, title_height});
    title_screen_window_->clear();

    title_screen_window_->print(phrase_, {5, 0}, Colour::GRAY_DARK, Font::TRIHOOK_HALF);

    title_screen_window_->print("{r}_______  {K}_______  {g}______    {u}_______", {3, 1});
    title_screen_window_->print("{r}|       |{K}|       |{g}|    _ |  {u}|       |", {2, 2});
    title_screen_window_->print("{r}|    ___|{K}|   _   |{g}|   | ||  {u}|    _  |", {2, 3});
    title_screen_window_->print("{r}|   | __ {K}|  | |  |{g}|   |_||_ {u}|   |_| |", {2, 4});
    title_screen_window_->print("{r}|   ||  |{K}|  |_|  |{g}|    __  |{u}|    ___|", {2, 5});
    title_screen_window_->print("{r}|   |_| |{K}|       |{g}|   |  | |{u}|   |", {2, 6});
    title_screen_window_->print("{r}|_______|{K}|_______|{g}|___|  |_|{u}|___|", {2, 7});

    title_screen_window_->print("/\\/\\", {18, 14}, Colour::GREEN);
    title_screen_window_->print("{G}|   _oo", {18, 15});
    title_screen_window_->print("{G}/\\  {g}/\\   {G}/ (_{W},,,{G})", {8, 16});
    title_screen_window_->print("{G}) /^\\{g}) ^\\{G}/ {Y}_)", {7, 17});
    title_screen_window_->print("{G})   /^\\/   {Y}_)", {7, 18});
    title_screen_window_->print("{G})   _ /  / {Y}_)", {7, 19});
    title_screen_window_->print("{g}/\\ {G})/\\/ ||  | {Y})_)", {4, 20});
    title_screen_window_->print("{g}<  >     {G}|({W},,{G}) {Y})__)", {3, 21});
    title_screen_window_->print("{g}||      {G}/   \\{Y})___){g}\\", {4, 22});
    title_screen_window_->print("{g}| \\____{G}(     {Y})___){g})__", {4, 23});
    title_screen_window_->print("{g}\\______{G}(_____{W};;  {g}__{w};;", {5, 24});

    if (blinking_) title_screen_window_->put('-', {21, 15}, Colour::GREEN_DARK);
    else title_screen_window_->put('@', {21, 15}, Colour::RED_DARK);

    int backronym_pos = title_screen_window_->get_middle().x - ((backronym_.size() + 2) / 2);
    if (backronym_pos < 0) backronym_pos = 0;
    title_screen_window_->print("(" + backronym_ + ")", {backronym_pos, 11}, Colour::GRAY_DARK);
    title_screen_window_->put('o', {25, 12}, Colour::GRAY_DARK);
    title_screen_window_->put('o', {23, 13}, Colour::GRAY_DARK);

    std::string build_str = " {u} build " + version::BUILD_TIMESTAMP;
#if defined(GORP_BUILD_DEBUG)
    build_str += "D";
#endif
    title_screen_window_->print("{r}version " + version::VERSION_STRING + build_str, {4, 9});
    title_screen_window_->print("Copyright   2025 Raine \"Gravecat\" Simmons", {1, 26}, Colour::BLUE);
    title_screen_window_->put(255, {11, 26}, Colour::BLUE);
#if defined(GORP_BUILD_DEBUG)
    title_screen_window_->print("debug build - not for public distribution", {1, 28}, Colour::RED_DARK);
#endif

    title_screen_window_->print("{W}({g}1{W}) New Game", {27, 17});
    title_screen_window_->print("{K}(2) {w}Load Game", {27, 19});
    title_screen_window_->print("{W}({g}3{W}) Quit Game", {27, 21});

    title_screen_window_->move(term.get_middle() - title_screen_window_->get_middle());
}

// Render speed test.
void TitleScreen::render_test()
{
    Terminal &term = terminal();
    sf::Clock clock;
    Vector2u screen_size = term.size();
    term.set_frame_limit(false);
    int frame_count = 0;
    while (clock.getElapsedTime().asSeconds() < 10)
    {
        title_screen_window_->clear();
        for (unsigned int x = 0; x < screen_size.x; x++)
            for (unsigned int y = 0; y < screen_size.y; y++)
                title_screen_window_->put(random::get<int>(0, 255), Vector2(x, y), static_cast<Colour>(random::get<int>(1, 25)));
        term.get_key();
        frame_count++;
    }
    term.set_frame_limit(true);
    title_screen_window_->clear();
    title_screen_window_->print("Frames per second: " + std::to_string(static_cast<float>(frame_count) / 10.0f), {1, 1});
    int key = 0;
    while (!key) { key = term.get_key(); }
}

}   // namespace gorp
