// 3rpdarty/sam/dictionary.cpp -- SAM sometimes mispronounces words. This provides a lookup table where it can replace words with known pronunciation issues,
// with replacement words spelled in a more phonetic way that SAM can use correctly.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "core/core.hpp"
#include "dictionary.hpp"
#include "util/file/yaml.hpp"
#include "util/stringutils.hpp"

namespace gorp {
namespace sam {

std::map<std::string, std::string> SAMDict::dictionary_words;

void SAMDict::load_strings()
{
    YAML yaml_file(core().datafile("misc/sam.yml"));
    if (!yaml_file.is_map()) throw GuruMeditation("misc/sam.yml -- Invalid file format!");
    std::vector<std::string> children = yaml_file.keys();
    for (auto key : children)
        dictionary_words.insert( { key, yaml_file.val(key) } );
}

void SAMDict::replace_string(std::string &str)
{
    if (!str.size()) return;
    std::string punctuation;
    while(true)
    {
        char last_char = str[str.size() - 1];
        if (!((last_char >= 'a' && last_char <= 'z') || (last_char >= 'A' && last_char <= 'Z')))
        {
            punctuation = std::string(1, last_char) + punctuation;
            str = str.substr(0, str.size() - 1);
        }
        else break;
    }

    auto result = dictionary_words.find(stringutils::str_tolower(str));
    if (result != dictionary_words.end()) str = stringutils::replace_keep_capitalization(str, result->second);
    else
    {
        // Check if we're using a plural (e.g. the correction for "gallow" should work the same as the correction for "gallows")
        if (str[str.size() - 1] == 's' || str[str.size() - 1] == 'S')
        {
            result = dictionary_words.find(stringutils::str_tolower(str.substr(0, str.size() - 1)));
            if (result != dictionary_words.end()) str = stringutils::replace_keep_capitalization(str, result->second) + str[str.size() - 1];
        }
    }
    str += punctuation;
}

} } // namespace sam, gorp
