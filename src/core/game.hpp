// core/game.hpp -- The Game object is the central game manager, handling the main loop, saving/loading, and starting of new games.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {

class Codex;        // defined in world/codex.hpp
class Element;      // defined in ui/element.hpp
class MessageLog;   // defined in ui/messagelog.hpp

class Game {
public:
                Game();             // Constructor, sets up the game manager.
                ~Game();            // Destructor, cleans up attached classes.
    uint32_t    add_element(std::unique_ptr<Element> element);  // Adds a new UI element to the screen.
    void        begin();            // Starts the game, in the form of a title screen followed by the main game loop.
    Codex&      codex() const;      // Returns a reference to the Codex object.
    void        delete_element(uint32_t id);    // Deletes a specified UI element.
    Element&    element(uint32_t id) const;     // Retrieves a specified UI element.
    void        element_to_back(uint32_t id, unsigned int ignore = 0);  // Moves a UI element to the back of the screen, optionally ignoring a number of others.
    void        element_to_front(uint32_t id);  // Moves a UI element to the front of the screen.
    void        leave_game();       // Shuts things down cleanly and exits the game.
    MessageLog& log() const;        // Returns a reference to the MessageLog object.
    void        process_input(const std::string &input);    // Processes input from the player.
    uint32_t    unique_ui_id();     // Returns a new, unique UI element ID.

private:
    void    main_loop();        // brøether, may i have the lööps
    void    clear_elements();   // Clears all UI elements.
    void    new_game();         // Sets up for a new game!

    std::unique_ptr<Codex>  codex_ptr_; // The Codex object, which stores all the static game data in memory, and generates copies of said data.
    std::vector<std::unique_ptr<Element>>   ui_elements_;   // The UI elements on screen right now.
    uint32_t    ui_element_id_counter_; // The counter for generating unique UI element IDs.
    uint32_t    ui_input_;  // The vector ID of the Input stored in ui_elements_.
    uint32_t    ui_msglog_; // The vector ID of the MessageLog stored in ui_elements_.
};

Game&   game(); // A shortcut instead of using core().game()

}   // namespace gorp
