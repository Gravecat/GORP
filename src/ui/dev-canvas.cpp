// ui/dev-canvas.cpp -- The DevCanvas UI element provides a very simple 'canvas' window that can be moved around with the arrow keys. This is intended for use
// during development only, for previewing data or doing other testing.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/terminal/terminal.hpp"
#include "core/terminal/window.hpp"
#include "ui/dev-canvas.hpp"

namespace gorp {

// Creates a new DevCanvas of the specified size, in tiles.
DevCanvas::DevCanvas(Vector2u size) : Element(), size_(size)
{
    if (!size.x || !size.y) throw GuruMeditation("Invalid DevCanvas size", size.x, size.y);
}

// Clears the canvas entirely.
void DevCanvas::clear()
{
    window_->clear();
    needs_redraw();
}

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

}   // namespace gorp
