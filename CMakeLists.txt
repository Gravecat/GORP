# CMakeLists.txt -- CMake build file for GORP, intended for use with GCC.
# For Linux builds, make a build folder and just `cmake ..`
# For Windows builds, use the apt package mingw-w64 and cross-compile from Linux or Ubuntu WSL, using `cmake .. -DCROSS_COMPILE_WINDOWS=1`

# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.13)
project(gorp)

# Automatically default to debug builds if a type is not specified.
# This code borrowed from BearLibTerminal, copyright (c) 2013-2017 Cfyz.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Build type was not specified, switching to 'Debug'.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()

# Determine target platform. No support for MacOS yet, as I don't have access to a Mac for testing. There's some Windows/Linux-specific code in the project,
# so this isn't as simple as just updating CMakeLists.
if(WIN32)
  set(TARGET_WINDOWS 1)
elseif(MINGW)
  set(TARGET_WINDOWS 1)
  set(TARGET_MINGW 1)
elseif(UNIX AND NOT APPLE)
  set(TARGET_LINUX 1)
endif()

# Detect if MinGW is being used to cross-compile to Windows. There are simpler ways, but this way guarantees success when using WSL.
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
  #ifdef __MINGW32__
    int main() { return 0; }
  #else
    #error Not MinGW
  #endif
" HAVE_MINGW_MACRO)
if (HAVE_MINGW_MACRO)
  set(TARGET_WINDOWS 1)
  set(TARGET_MINGW 1)
  unset(TARGET_LINUX)
endif()

# An extra marker for the source files, to tell what kind of build type we're using.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(GORP_BUILD_RELEASE)
  set(RELEASE_BUILD 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_compile_definitions(GORP_BUILD_DEBUG)
  set(DEBUG_BUILD 1)
else()
  message(FATAL_ERROR "Unrecognized build type!")
endif()

# Automatically generate version strings used in-game and in the Windows resource file, from this specified game version.
set(GORP_VERSION "0.0.0.1")
string(REPLACE "." "," GORP_VERSION_COMMA "${GORP_VERSION}")
execute_process(COMMAND date "+%y%m%d%H%M" OUTPUT_VARIABLE BUILD_TIMESTAMP OUTPUT_STRIP_TRAILING_WHITESPACE)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/version.hpp @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/resource.rc.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/resource.rc @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/source.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/source.hpp @ONLY)

# Non-platform-specific stuff.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(__STDC_LIMIT_MACROS)
add_compile_definitions(_USE_MATH_DEFINES)
set(GORP_BIN "$<TARGET_FILE:gorp>")

# Compiler-specific options. These are primarily for GCC right now; feel free to add other compilers if you know how they work.
# GCC:
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wextra -pedantic -pedantic-errors)
  if(RELEASE_BUILD)
    add_compile_options(-O2)
    add_link_options(-s)
  else()
    add_compile_options(-g -Og)
  endif(RELEASE_BUILD)
# Microsoft Visual Studio:
elseif(MSVC)
  add_compile_options(/W4 /permissive-)
endif()

# Platform-specific options.
# Windows:
if(TARGET_WINDOWS)
  add_compile_definitions(GORP_TARGET_WINDOWS)
  add_compile_definitions(WIN32_LEAN_AND_MEAN)
  set(OS_LIBRARIES mingw32 shlwapi ntdll)
  if(DEBUG_BUILD)
    set(SFML_LIBRARIES sfml-graphics-d sfml-window-d sfml-audio-d sfml-system-d)
  else()
    set(SFML_LIBRARIES sfml-graphics sfml-window sfml-audio sfml-system)
  endif()
  set(GORP_BIN "$<TARGET_FILE:gorp>.exe")

  # Windows resource file, includes application icon.
  find_program(WINDRES_EXECUTABLE NAMES x86_64-w64-mingw32-windres windres DOC "Path to the MinGW windres resource compiler")
  if (WINDRES_EXECUTABLE)
    set(CMAKE_RC_COMPILER "${WINDRES_EXECUTABLE}" CACHE FILEPATH "Resource compiler for Windows targets")
  else()
    message(FATAL_ERROR "Could not locate windres for resource compilation")
  endif()
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  set(GORP_RC "${CMAKE_CURRENT_BINARY_DIR}/cmake/resource.rc")

# Linux:
elseif(TARGET_LINUX)
  add_compile_definitions(GORP_TARGET_LINUX)
  set(OS_LIBRARIES X11 Xrandr Xi Xcursor udev FLAC vorbis vorbisenc vorbisfile ogg)
  if(DEBUG_BUILD)
    set(SFML_LIBRARIES sfml-graphics-s-d sfml-window-s-d sfml-audio-s-d sfml-system-s-d)
  else()
    set(SFML_LIBRARIES sfml-graphics-s sfml-window-s sfml-audio-s sfml-system-s)
  endif()

else()
  message(FATAL_ERROR "Unknown/unsupported build environment.")
endif()

# Source files.
set(GORP_CPPS
  src/core/audio/oggmusic.cpp
  src/core/audio/oggsound.cpp
  src/core/audio/sfxr.cpp
  src/core/core.cpp
  src/core/game.cpp
  src/core/guru.cpp
  src/core/prefs.cpp
  src/core/terminal/colour-maps.cpp
  src/core/terminal/terminal.cpp
  src/core/terminal/window.cpp
  src/ui/element.cpp
  src/ui/input.cpp
  src/ui/messagelog.cpp
  src/ui/title.cpp
  src/util/file/binpath.cpp
  src/util/file/filereader.cpp
  src/util/file/fileutils.cpp
  src/util/file/filewriter.cpp
  src/util/file/yaml.cpp
  src/util/system/process.cpp
  src/util/system/winver.cpp
  src/util/text/namegen.cpp
  src/util/text/stringutils.cpp
  src/world/codex.cpp
)

# Binary file. GORP_RC should be blank for non-Windows builds.
add_executable(gorp ${GORP_CPPS} ${GORP_RC})

# Extra link options for Windows, to disable the console window.
if(TARGET_WINDOWS)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(gorp PRIVATE -mwindows)
  elseif(MSVC)
    target_link_options(gorp PRIVATE /SUBSYSTEM:WINDOWS)
  endif()
endif(TARGET_WINDOWS)

# Include directories.
target_include_directories(gorp PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/src/3rdparty/sfml/include"
  "${CMAKE_CURRENT_BINARY_DIR}"
)

# Platform-specific library directories.
if(TARGET_WINDOWS)
  target_link_directories(gorp PRIVATE "${CMAKE_SOURCE_DIR}/windows/lib")
elseif(TARGET_LINUX)
  target_link_directories(gorp PRIVATE "${CMAKE_SOURCE_DIR}/linux/lib")
endif()

# Link libraries. Platform-specific stuff should be set in the main platform-specific section near the top.
target_link_libraries(gorp
  ${SFML_LIBRARIES}
  ${OS_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  fantasyname
  rapidyaml
  sfxr
)

# Post-build, make a 'bin' folder and copy the binary file in there.
add_custom_command(TARGET gorp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
  COMMAND ${CMAKE_COMMAND} -E copy "${GORP_BIN}" "${CMAKE_BINARY_DIR}/bin"
)

# Build some third-party code as separate binaries to be linked in.
add_subdirectory(src/3rdparty/fantasyname)
add_subdirectory(src/3rdparty/rapidyaml)
add_subdirectory(src/3rdparty/sfxr)
