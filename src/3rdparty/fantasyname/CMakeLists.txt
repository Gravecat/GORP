# Very basic minimal CMake file for building fantasyname code as its own object.
# Raine "Gravecat" Simmons, 2025.

cmake_minimum_required(VERSION 3.13)

add_library(fantasyname STATIC namegen.cpp)
