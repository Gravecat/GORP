// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// util/stringutils.hpp -- Various utility functions that deal with string manipulation/conversion.

#pragma once

#include "core/global.hpp"

namespace gorp {
namespace stringutils {

unsigned int    ansi_strlen(const std::string &input);  // Gives the length of a string, adjusted by ANSI tags.
                            // Splits an ANSI string into a vector of strings, to a given line length.
std::vector<std::string>    ansi_vector_split(const std::string &source, unsigned int line_len, bool auto_tags = true);
std::string     itoh(uint32_t num, uint8_t min_len);    // Converts an integer into a hex string.
std::string     join_words(std::vector<std::string> vec, const std::string &spacer = " ");  // Takes a vector of strings and squashes them into one string.
                // Replaces input with output, maintaining the capitalization of input (e.g. input="Meow" output="cat" result="Cat")
std::string     replace_keep_capitalization(const std::string &input, const std::string &output);
std::string     str_tolower(std::string str);   // Converts a string to lower-case.
std::string     str_toupper(std::string str);   // Converts a string to upper-case.
std::vector<std::string>    string_explode(std::string str, const std::string &separator);  // String split/explode function.
std::string     strip(const std::string &str, char to_remove);  // Strips all instances of to_remove out of a string.
unsigned int    word_count(const std::string &str, const std::string &word);    // Returns a count of the amount of times a string is found in a parent string.

} } // namespace stringutils, gorp
