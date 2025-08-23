// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// core/global/guru-exception.hpp -- A custom type of std::exception which can be caught and handled by Guru Meditation, providing extra information.

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace gorp {

// Custom exception type, which records error codes that can be rendered in Guru::halt()
class GuruMeditation : public std::runtime_error {
public:
            GuruMeditation(const std::string &what_arg, int32_t code_a = 0, int32_t code_b = 0);
    int32_t error_a() const, error_b() const;

private:
    int32_t error_a_, error_b_; // Error codes passed to Guru::halt().
};

}   // namespace gorp
