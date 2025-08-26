// util/system/winver.hpp -- Some ugly code to determine what version of Windows (if any) the user is running.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {
namespace winver {

bool    is_windows_11();    // Returns true if the user is running Windows 11.

}   // namespace winver
}   // namespace gorp