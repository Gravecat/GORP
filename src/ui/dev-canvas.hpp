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
    void    clear();                    // Clears the canvas entirely.
    void    recreate_window() override; // (Re)creates the render window for this canvas.

private:
    Vector2u    size_;  // The size of this canvas.
};

}   // namespace gorp
