// 3rpdarty/sam/dictionary.hpp -- SAM sometimes mispronounces words. This provides a lookup table where it can replace words with known pronunciation issues,
// with replacement words spelled in a more phonetic way that SAM can use correctly.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <map>

#include "core/global.hpp"

namespace gorp {
namespace sam {

class SAMDict
{
public:
    static void load_strings();
    static void replace_string(std::string &str);

private:
    static std::map<std::string, std::string>  dictionary_words;
};

} } // namespace sam, gorp
