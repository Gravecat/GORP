// util/file/binpath.cpp -- Some very nasty platform-specific code for determining the path of the binary file at runtime.
// Based on and modified from code I found on StackOverflow: https://stackoverflow.com/questions/1528298/get-path-of-executable

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef GORP_TARGET_WINDOWS
#include <windows.h>
#include <psapi.h>      // EnumProcesses(), GetModuleBaseName()
#include <shlwapi.h>    // PathRemoveFileSpecA(), PathCombineA()
#endif

#ifdef GORP_TARGET_LINUX
#include <libgen.h>     // dirname()
#if defined(__sun)
    #define PROC_SELF_EXE "/proc/self/path/a.out"
#else
    #define PROC_SELF_EXE "/proc/self/exe"
#endif
#endif

#ifdef GORP_TARGET_MACOS
#include <libgen.h>         // dirname()
#include <mach-o/dyld.h>    // _NSGetExecutablePath()
#endif

#include <cstring>

#include "util/file/binpath.hpp"

namespace gorp {

std::string BinPath::exe_dir;   // The path to the binary.

// Given a path or filename, combines it with the current executable path and returns the combined, full path. At least, that's the hope.
std::string BinPath::game_path(const std::string &path)
{
    if (!exe_dir.size()) exe_dir = get_executable_dir();
    return merge_paths(exe_dir, path);
}

#ifdef GORP_TARGET_WINDOWS
// Platform-agnostic way to find this binary's runtime path.
std::string BinPath::get_executable_path()
{
    char rawPathName[MAX_PATH];
    GetModuleFileNameA(NULL, rawPathName, MAX_PATH);
    return std::string(rawPathName);
}

std::string BinPath::get_executable_dir()
{
    std::string executablePath = get_executable_path();
    char* exePath = new char[executablePath.length() + 1];
    strcpy(exePath, executablePath.c_str());
    PathRemoveFileSpecA(exePath);
    std::string directory = std::string(exePath);
    delete[] exePath;
    return directory;
}

// Merges two path strings together.
std::string BinPath::merge_paths(const std::string &pathA, const std::string &pathB)
{
    char combined[MAX_PATH];
    PathCombineA(combined, pathA.c_str(), pathB.c_str());
    std::string mergedPath(combined);
    return mergedPath;
}
#endif  // GORP_TARGET_WINDOWS

#ifdef GORP_TARGET_LINUX
std::string BinPath::get_executable_path()
{
    char rawPathName[PATH_MAX];
#pragma GCC diagnostic ignored "-Wunused-result"
    realpath(PROC_SELF_EXE, rawPathName);
#pragma GCC diagnostic pop
    return std::string(rawPathName);
}

std::string BinPath::get_executable_dir()
{
    const std::string& executablePath = get_executable_path();
    char *executablePathStr = new char[executablePath.length() + 1];
    strcpy(executablePathStr, executablePath.c_str());
    char* executableDir = dirname(executablePathStr);
    const std::string exec_dir = std::string(executableDir);
    delete[] executablePathStr;
    return exec_dir;
}

std::string BinPath::merge_paths(const std::string &pathA, const std::string &pathB) { return pathA + "/" + pathB; }
#endif  // GORP_TARGET_LINUX

#ifdef GORP_TARGET_MACOS
std::string BinPath::get_executable_path()
{
    char rawPathName[PATH_MAX];
    char realPathName[PATH_MAX];
    uint32_t rawPathSize = (uint32_t)sizeof(rawPathName);

#pragma GCC diagnostic ignored "-Wunused-result"
    if(!_NSGetExecutablePath(rawPathName, &rawPathSize)) realpath(rawPathName, realPathName);
#pragma GCC diagnostic pop
    return std::string(realPathName);
}

std::string BinPath::get_executable_dir()
{
    const std::string& executablePath = get_executable_path();
    char *executablePathStr = new char[executablePath.length() + 1];
    strcpy(executablePathStr, executablePath.c_str());
    char* executableDir = dirname(executablePathStr);
    const std::string exec_dir = std::string(executableDir);
    delete[] executablePathStr;
    return exec_dir;
}

std::string BinPath::merge_paths(const std::string& pathA, const std::string& pathB) { return pathA + "/" + pathB; }
#endif  // GORP_TARGET_MACOS

}   // namespace gorp
