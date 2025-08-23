// util/file/filewriter.hpp -- The FileWriter class handles writing binary data to various data files.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <fstream>

#include "core/global.hpp"

namespace gorp {

class FileWriter {
public:
    void    close_file();                           // Closes the binary file.
    void    open_file(std::string filename);        // Opens a file for writing.
    void    write_char_vec(std::vector<char> vec);  // Writes binary data (in the form of an std::vector<char>) to the binary file.
    void    write_string(std::string str);          // Writes a string to the file.

    // Writes a basic data type (integer, float, etc.) to the file.
    template<typename T> void   write_data(T data)
    { file_out_.write(reinterpret_cast<const char*>(&data), sizeof(T)); }

protected:
    std::ofstream   file_out_;  // File handle for writing into the binary data file.
};

}   // namespace gorp
