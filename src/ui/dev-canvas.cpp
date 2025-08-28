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

// (Re)creates the render window for this UI element.
void DevCanvas::recreate_window()
{
    Terminal &term = terminal();
    if (window_) term.remove_window(window_);
    window_ = term.add_window(size_);
}

}   // namespace gorp
