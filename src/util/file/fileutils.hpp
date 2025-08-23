// util/file/fileutils.hpp -- Various utility functions that deal with creating, deleting, and manipulating files in a platform-agnostic way.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "core/global.hpp"

namespace gorp {
namespace fileutils {

void        delete_file(const std::string &filename);       // Deletes a specified file.
bool        directory_exists(const std::string &dir);       // Check if a directory exists.
bool        file_exists(const std::string &file);           // Checks if a file exists.
// Returns a list of files in a given directory. If an extension filter is specified, only returns files that end in the given extension.
std::vector<std::string>    files_in_dir(const std::string &directory, std::string extension_filter = "", bool recursive = false);
std::vector<char>           file_to_char_vec(const std::string &filename);  // Loads a binary file into a char vector.
std::string file_to_string(const std::string &filename);    // Loads a text file into an std::string.
std::vector<std::string>    file_to_vec(const std::string &filename);   // Loads a text file into a vector, one string for each line of the file.
void        make_dir(const std::string &dir);               // Makes a new directory, if it doesn't already exist.

} } // fileutils, gorp namespaces
