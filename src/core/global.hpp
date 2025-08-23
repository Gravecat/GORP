// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// core/global.hpp -- Core definitions that are used very frequently in GORP, enough that they warrant being put in a shared header together.
// This header also includes some very frequently-used system headers, to avoid having to include them everywhere by hand.

#pragma once

#include "core/global/enums.hpp"            // Very frequently-used enums like Colour.
#include "core/global/global-includes.hpp"  // Very frequently-used system headers.
#include "core/global/guru-exception.hpp"   // Custom std::exception variant, for providing extra information to Guru Meditation.
#include "core/global/vector2.hpp"          // Vector2 struct definition.
