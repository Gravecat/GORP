# Very basic minimal CMake file for building sfxr code as its own object.

# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.13)

# We'll compile the sfxr code as a static library, and link it into the project.
add_library(sfxr STATIC sfxr.cpp)
target_include_directories(sfxr PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/src/3rdparty"
  "${CMAKE_SOURCE_DIR}/src/3rdparty/sfml/include"
)
target_link_libraries(sfxr
  ${SFML_LIBRARIES}
  ${OS_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)
