// util/stringutils.hpp -- Various utility functions that deal with string manipulation/conversion.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <algorithm>    // std::transform, std::remove
#include <numeric>      // std::accumulate
#include <sstream>

#include "util/stringutils.hpp"

namespace gorp {
namespace stringutils {

// Gives the length of a string, adjusted by ANSI tags.
unsigned int ansi_strlen(const std::string &input)
{
    unsigned int len = input.size();

    // Count any ANSI tags.
    const int openers = std::count(input.begin(), input.end(), '{');
    const int symbols = std::count(input.begin(), input.end(), '^');
    if (openers) len -= openers * 3;
    if (symbols) len -= (symbols / 2) * 5;

    // Return the result.
    return len;
}

// Splits an ANSI string into a vector of strings, to a given line length.
std::vector<std::string> ansi_vector_split(const std::string &source, unsigned int line_len, bool auto_tags)
{
    std::vector<std::string> output;

    // Check to see if the line of text has the no-split tag at the start.
    if (source.size() >= 5)
    {
        if (!source.substr(0, 5).compare("^000^"))
        {
            output.push_back(source);
            return output;
        }
    }

    // Check to see if the line is too short to be worth splitting.
    if (source.size() <= line_len)
    {
        output.push_back(source);
        return output;
    }

    // Split the string into individual words.
    std::vector<std::string> words = string_explode(source, " ");

    // Keep track of the current line and our position on it.
    unsigned int current_line = 0, line_pos = 0;
    std::string last_ansi = "{w}";  // The last ANSI tag we encountered; white by default.

    // Start with an empty string.
    output.push_back("");

    for (auto word : words)
    {
        if (word == "{/}")  // Check for new-line marker.
        {
            if (line_pos > 0)
            {
                line_pos = 0;
                current_line += 2;
                output.push_back(" ");
                if (auto_tags) output.push_back(last_ansi); else output.push_back("");
            }
        }
        else if (word == "{\\}")
        {
            if (line_pos > 0)
            {
                line_pos = 0;
                current_line += 1;
                if (auto_tags) output.push_back(last_ansi); else output.push_back("");
            }
        }
        else
        {
            unsigned int length = word.length();    // Find the length of the word.

            // If the word includes high-ASCII tags, adjust the length.
            size_t htag_pos = word.find("^");
            bool high_ascii = false;
            if (htag_pos != std::string::npos)
            {
                if (word.size() > htag_pos + 4)
                {
                    if (word.at(htag_pos + 4) == '^')
                    {
                        length -= word_count(word, "^") * 2;
                        high_ascii = true;
                    }
                }
            }

            const int ansi_count = word_count(word, "{");   // Count the ANSI tags.
            if (ansi_count) length -= (ansi_count * 3);     // Reduce the length if one or more ANSI tags are found.
            if (length + line_pos >= line_len)  // Is the word too long for the current line?
            {
                line_pos = 0; current_line++;   // CR;LF
                if (auto_tags) output.push_back(last_ansi); // Start the line with the last ANSI tag we saw.
                else output.push_back("");
            }
            if (ansi_count)
            {
                // Duplicate the last-used ANSI tag.
                const std::string::size_type flo = word.find_last_of("{");
                if (flo != std::string::npos)
                {
                    if (word.size() >= flo + 3)
                    {
                        const std::string last_test = word.substr(flo, 3);
                        if (last_test.compare("{/}") || last_test.compare("{\\}")) last_ansi = last_test;
                    }
                }
            }
            if (line_pos != 0)  // NOT the start of a new line?
            {
                length++;
                output.at(current_line) += " ";
            }
            // Is the word STILL too long to fit over a single line?
            // Don't attempt this on high-ASCII words.
            while (length > line_len && !high_ascii)
            {
                const std::string trunc = word.substr(0, line_len);
                word = word.substr(line_len);
                output.at(current_line) += trunc;
                line_pos = 0; current_line++;
                if (auto_tags) output.push_back(last_ansi); // Start the line with the last ANSI tag we saw.
                else output.push_back("");
                length = word.size();   // Adjusts the length for what we have left over.
            }
            output.at(current_line) += word; line_pos += length;
        }
    }

    return output;
}

// Converts an integer into a hex string.
std::string itoh(uint32_t num, uint8_t min_len)
{
    std::stringstream ss;
    ss << std::hex << num;
    std::string hex = ss.str();
    while (min_len && hex.size() < min_len) hex = "0" + hex;
    return hex;
}

// Takes a vector of strings and squashes them into one string.
std::string join_words(std::vector<std::string> vec, const std::string &spacer)
{
    return std::accumulate(vec.begin() + 1, vec.end(), vec[0],
        [&spacer](const std::string& a, const std::string& b) {
            return a + spacer + b;
        });
}

// Replaces input with output, maintaining the capitalization of input (e.g. input="Meow" output="cat" result="Cat")
std::string replace_keep_capitalization(const std::string &input, const std::string &output)
{
    if (!input.size() || !output.size()) return "";
    std::string result = str_tolower(output);

    bool first_letter_caps = (input[0] >= 'A' && input[0] <= 'Z');
    bool all_caps = (input.size() > 2 && input[1] >= 'A' && input[1] <= 'Z');

    if (all_caps) return str_toupper(result);
    else if (first_letter_caps && result[0] >= 'a' && result[0] <= 'z') result[0] -= 32;

    return result;
}

// Converts a string to lower-case.
std::string str_tolower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Converts a string to upper-case.
std::string str_toupper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// String split/explode function.
std::vector<std::string> string_explode(std::string str, const std::string &separator)
{
    std::vector<std::string> results;

    std::string::size_type pos = str.find(separator, 0);
    const size_t pit = separator.length();

    while(pos != std::string::npos)
    {
        if (pos == 0) results.push_back("");
        else results.push_back(str.substr(0, pos));
        str.erase(0, pos + pit);
        pos = str.find(separator, 0);
    }
    results.push_back(str);

    return results;
}

// Strips all instances of to_remove out of a string.
std::string strip(const std::string &str, char to_remove)
{
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), to_remove), result.end());
    return result;
}

// Returns a count of the amount of times a string is found in a parent string.
unsigned int word_count(const std::string &str, const std::string &word)
{
    unsigned int count = 0;
    std::string::size_type word_pos = 0;
    while(word_pos != std::string::npos)
    {
        word_pos = str.find(word, word_pos);
        if (word_pos != std::string::npos)
        {
            count++;
            word_pos += word.length();
        }
    }
    return count;
}

} } // namespace stringutils, gorp
