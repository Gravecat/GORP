# Very basic minimal CMake file for building SAM code as its own object.

# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.13)

# SAM source files
set(SAM_C createtransitions.c processframes.c reciter.c render.c sam.c)
set(SAM_CPP dictionary.cpp sam.cpp)

# We'll compile the SAM code as a static library, and link it into the project.
add_library(sam STATIC ${SAM_C} ${SAM_CPP})

target_include_directories(sam PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/src/3rdparty"
  "${CMAKE_SOURCE_DIR}/src/3rdparty/sam"
  "${CMAKE_SOURCE_DIR}/src/3rdparty/sfml/include"
)

target_link_libraries(sam
  ${SFML_LIBRARIES}
  ${OS_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)
