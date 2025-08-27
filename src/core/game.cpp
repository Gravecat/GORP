// core/game.cpp -- The Game object is the central game manager, handling the main loop, saving/loading, and starting of new games.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>  // EXIT_SUCCESS

#include "core/audio/oggsound.hpp"
#include "core/audio/oggmusic.hpp"
#include "core/core.hpp"
#include "core/game.hpp"
#include "core/terminal/terminal.hpp"
#include "ui/element.hpp"
#include "ui/input.hpp"
#include "ui/messagelog.hpp"
#include "ui/title.hpp"
#include "util/math/random.hpp"
#include "world/codex.hpp"

namespace gorp {

Game::Game() : codex_ptr_(nullptr), ui_element_id_counter_(0), ui_input_(0), ui_msglog_(0) { }

// Destructor, cleans up attached classes.
Game::~Game()
{
    for (unsigned int i = 0; i < ui_elements_.size(); i++)
        ui_elements_.at(i).reset(nullptr);
    codex_ptr_.reset(nullptr);
}

// Adds a new UI element to the screen.
uint32_t Game::add_element(std::unique_ptr<Element> element)
{
    const uint32_t id = element.get()->id();
    ui_elements_.push_back(std::move(element));
    return id;
}

// Starts the game, in the form of a title screen followed by the main game loop.
void Game::begin()
{
    codex_ptr_ = std::make_unique<Codex>();
    auto title_screen_ptr = std::make_unique<TitleScreen>();

    const auto result = title_screen_ptr->render();
    title_screen_ptr.reset(nullptr);
    switch(result)
    {
        case TitleScreen::TitleOption::QUIT:
        case TitleScreen::TitleOption::LOAD_GAME:   // Load game is not yet implemented.
            leave_game();
            break;
        case TitleScreen::TitleOption::NEW_GAME:
            new_game();
            break;
    }
    main_loop();
}

// Clears all UI elements.
void Game::clear_elements()
{
    ui_input_ = ui_msglog_ = 0;
    for (unsigned int i = 0; i < ui_elements_.size(); i++)
        ui_elements_.at(i)->destroy_window();
    ui_elements_.clear();
}

// Returns a reference to the Codex object.
Codex& Game::codex() const
{
    if (!codex_ptr_) throw std::runtime_error("Attempt to access null Codex pointer!");
    return *codex_ptr_;
}

// Deletes a specified UI element.
void Game::delete_element(uint32_t id)
{
    for (unsigned int i = 0; i < ui_elements_.size(); i++)
    {
        if (ui_elements_.at(i).get()->id() == id)
        {
            ui_elements_.erase(ui_elements_.begin() + i);
            return;
        }
    }
    throw std::runtime_error("Attempt to delete invalid UI element!");
}

// Retrieves a specified UI element.
Element& Game::element(uint32_t id) const
{
    for (unsigned int i = 0; i < ui_elements_.size(); i++)
    {
        Element* element = ui_elements_.at(i).get();
        if (!element) throw std::runtime_error("Null element on UI stack!");
        if (element->id() == id) return *element;
    }
    throw std::runtime_error("Invalid UI element requested!");
}

// Shuts things down cleanly and exits the game.
void Game::leave_game() { core().destroy_core(EXIT_SUCCESS); }

// Returns a reference to the MessageLog object.
MessageLog& Game::log() const
{
    if (!ui_msglog_) throw std::runtime_error("Attempt to access undefined message log pointer!");
    MessageLog* msglog = dynamic_cast<MessageLog*>(&element(ui_msglog_));
    if (!msglog) throw std::runtime_error("UI elements vector corrupted!");
    return *msglog;
}

// brøether, may i have the lööps
void Game::main_loop()
{
    ui_msglog_ = add_element(std::make_unique<MessageLog>());
    ui_input_ = add_element(std::make_unique<Input>());
    msg("{G}Welcome, brave adventurer to the perilous realms of {C}GORP{G}!");
    msg();
    msg("{R}Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi ultricies, felis et ultricies malesuada, quam felis bibendum nulla, in gravida nulla orci quis purus. Nullam sollicitudin id mi sed fermentum. Proin at dolor aliquam, fermentum arcu quis, commodo nisl. In a est elit. Proin egestas nibh eget viverra commodo. Aenean vitae tristique justo. Aliquam tincidunt aliquam neque, eu suscipit ante. Integer vel quam lacinia, viverra erat ac, tincidunt risus.");
    msg();
    msg("{Y}Cras luctus purus vitae semper vulputate. Aliquam congue lorem rhoncus pharetra commodo. Donec aliquam enim lacus, sit amet pulvinar purus tristique vel. Duis mattis mollis accumsan. Donec metus metus, mollis nec lectus ac, elementum efficitur enim. Nam sodales viverra purus, quis aliquet tortor lobortis quis. Aenean varius vel erat tincidunt faucibus. Aliquam eleifend nec justo sed lobortis. Morbi id maximus odio. Mauris id auctor arcu. Mauris mattis consectetur magna eget tincidunt. Maecenas fringilla felis sit amet velit tristique, sit amet consectetur odio vulputate. Cras tempus faucibus ex non egestas.");
    msg();
    msg("{G}In augue nulla, imperdiet eu faucibus vel, cursus elementum felis. Curabitur lacus ligula, pellentesque sit amet libero sit amet, tempor interdum justo. Duis eleifend nunc eu urna fringilla, eu molestie ipsum commodo. Suspendisse in purus dui. In hendrerit orci leo, quis consequat mi aliquet sit amet. Mauris neque risus, tempus sed nisi ac, varius accumsan erat. Pellentesque sagittis nulla ipsum, sed tristique erat fringilla at. Vestibulum ipsum sem, feugiat at congue sit amet, venenatis in arcu. Maecenas vel mi a est mollis accumsan. Mauris convallis justo interdum, pretium ligula ut, posuere tortor. Aenean sollicitudin sem ac auctor rhoncus. ");

    int key = 0;
    while(true)
    {
        // Redraw all UI elements, as needed.
        for (unsigned int i = 0; i < ui_elements_.size(); i++)
        {
            Element* el = ui_elements_.at(i).get();
            if (el->check_if_needs_redraw())
            {
                el->render();
                el->needs_redraw(false);
            }
        }

        key = terminal().get_key();
        switch(key)
        {
            case 0: break;      // Do nothing when no key is pressed.

            case Key::RESIZE:   // Resize 'key' is always handled by the game manager.
                for (unsigned int i = 0; i < ui_elements_.size(); i++)
                {
                    Element *el = ui_elements_.at(i).get();
                    el->recreate_window();
                    el->needs_redraw(true);
                }
                break;

            default:    // For all other keys, cycle through UI from top to bottom, stopping if any UI element has processed the pressed key.
                for (int i = ui_elements_.size() - 1; i >= 0; i--)
                {
                    Element *el = ui_elements_.at(i).get();
                    if (el->process_input(key)) break;
                }
                break;
        }
    }
}

// Sets up for a new game!
void Game::new_game()
{
}

// Processes input from the player.
void Game::process_input(const std::string &input)
{
    log().message("");
    log().message("{G}> " + input);
    log().message("");
}

// Returns a new, unique UI element ID.
uint32_t Game::unique_ui_id() { return ++ui_element_id_counter_; }

// A shortcut instead of using core().game()
Game& game() { return core().game(); }

}   // namespace gorp
