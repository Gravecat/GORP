// util/file/fileutils.cpp -- Various utility functions that deal with creating, deleting, and manipulating files in a platform-agnostic way.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <dirent.h>     // DIR, dirent, opendir(), readdir(), closedir()
#include <fstream>
#include <sstream>
#include <sys/stat.h>   // stat(), mkdir()
#include <unistd.h>     // unlink()

#include "util/file/fileutils.hpp"
#include "util/text/stringutils.hpp"

namespace gorp {
namespace fileutils {

// Deletes a specified file. Simple enough, but we'll keep this function around in case there's any platform-specific weirdness that needs to be worked in.
void delete_file(const std::string &filename) { unlink(filename.c_str()); }

// Check if a directory exists.
bool directory_exists(const std::string &dir)
{
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) return false;
    if (info.st_mode & S_IFDIR) return true;
    return false;
}

// Checks if a file exists.
bool file_exists(const std::string &file)
{
    struct stat info;
    return (stat(file.c_str(), &info) == 0);
}

// Returns a list of files in a given directory. If an extension filter is specified, only returns files that end in the given extension.
std::vector<std::string> files_in_dir(const std::string &directory, std::string extension_filter, bool recursive)
{
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> files;
    if (!(dir = opendir(directory.c_str()))) throw std::runtime_error("Cannot open directory: " + directory);
    while ((ent = readdir(dir)))
    {
        std::string filename = std::string(ent->d_name);
        if (filename == "." || filename == "..") continue;
        struct stat s;
        if (stat((directory + "/" + filename).c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)
            {
                if (recursive)
                {
                    std::vector<std::string> result = files_in_dir(directory + "/" + filename, extension_filter, true);
                    for (unsigned int i = 0; i < result.size(); i++)
                        result.at(i) = filename + "/" + result.at(i);
                    files.reserve(files.size() + result.size());
                    files.insert(files.end(), result.begin(), result.end());
                }
            }
            else if (s.st_mode & S_IFREG) files.push_back(filename);
        }
    }
    closedir(dir);

    if (extension_filter.size() && files.size())
    {
        // Remove the '.' from extension_filter, if needed (e.g. ".png" becomes "png"), and make sure it's lower-case.
        extension_filter = stringutils::str_tolower(extension_filter);
        if (extension_filter.at(0) == '.') extension_filter = extension_filter.substr(1);
        for (unsigned int i = 0; i < files.size(); i++)
        {
            std::string f = files.at(i);
            auto pos = f.find_last_of('.');
            if (pos == std::string::npos || pos == f.size() - 1) continue;
            std::string ext = stringutils::str_tolower(f.substr(pos + 1));
            if (ext.compare(extension_filter))
            {
                files.erase(files.begin() + i);
                i--;
            }
        }
    }

    return files;
}

// Loads a binary file into a char vector.
std::vector<char> file_to_char_vec(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

    // Get the size of the file.
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Create a vector to store the file content.
    std::vector<char> buffer(size);

    // Read the entire file into the vector.
    if (!file.read(buffer.data(), size)) throw std::runtime_error("Error reading file: " + filename);

    return buffer;
}

// Loads a text file into an std::string.
std::string file_to_string(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

// Loads a text file into a vector, one string for each line of the file.
std::vector<std::string> file_to_vec(const std::string &filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
        lines.push_back(line);
    }
    file.close();
    return lines;
}

// Makes a new directory, if it doesn't already exist.
void make_dir(const std::string &dir)
{
    if (directory_exists(dir) || file_exists(dir)) return;

#ifdef GORP_TARGET_WINDOWS
    mkdir(dir.c_str());
#else
    mkdir(dir.c_str(), 0777);
#endif
}

} } // namespace fileutils, gorp
