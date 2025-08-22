# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# CMakeLists.txt
# 
# CMake build file for GORP, intended for use with GCC.
# For Linux builds, make a build folder and just `cmake ..`
# For Windows builds, use the apt package mingw-w64 and cross-compile from Linux or Ubuntu WSL, using `cmake .. -DCROSS_COMPILE_WINDOWS=1`

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

# Platform-specific defaults/overrides
if(CROSS_COMPILE_WINDOWS)
  if(WIN32)
    message(FATAL_ERROR "For Windows builds, please cross-compile from Linux/WSL with MinGW.")
  else()
    message(STATUS "Cross-compilation mode for Windows enabled.")
    set(TARGET_WINDOWS 1)
    add_definitions(-DGORP_TARGET_WINDOWS)
  endif(WIN32)
else()
  if(WIN32)
    message(FATAL_ERROR "For Windows builds, please cross-compile from Linux/WSL with MinGW.")
  else()
    set(TARGET_LINUX 1)
    add_definitions(-DGORP_TARGET_LINUX)
  endif(WIN32)
endif(CROSS_COMPILE_WINDOWS)

# Builds are 64-bit by default, but we can specify a 32-bit build.
if(BUILD_I686)
  message(STATUS "Building for i686 (32-bit) architecture.")
  set(TARGET_I686 1)
endif(BUILD_I686)

# An extra marker for the source files, to tell what kind of build type we're using.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions(-DGORP_BUILD_RELEASE)
  set(RELEASE_BUILD 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_definitions(-DGORP_BUILD_DEBUG)
  set(DEBUG_BUILD 1)
else()
  message(FATAL_ERROR "Unrecognized build type!")
endif()

# Optionally, compile extra-strict with warnings as errors.
option(WARNINGS_AS_ERRORS "WARNINGS_AS_ERRORS" OFF)

# The rest of the meat and bones of the build script is in the src folder.
add_subdirectory(src)
