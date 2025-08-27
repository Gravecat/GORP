# Build Instructions

## Important Note!

This is very early days for this project. Long-term, I plan on adjusting the CMake build files to accomodate many compilers. Right now, it only has full
support for GCC (either on Linux, or cross-compiling to Windows with MinGW) and has not been tested on Visual Studio or other toolchains. Please see
[here](https://github.com/Gravecat/gorp/issues/1) for more information on MSVC, and [here](https://github.com/Gravecat/gorp/issues/2) for more on MacOS.

## Compiling SFML

GORP requires the compiled static library files (.a or .lib, *not* .so) for [SFML 3](https://github.com/SFML/SFML). You should place these files in either
`lib/linux` or `lib/windows` (depending on your target platform).

## Compiling GORP

### Windows

To compile via MinGW in WSL from VSCode, ensure the VSCode CMake settings are set to x86_64-w64-mingw32, it should set the rest up automatically. Visual Studio
support will be added eventually.

### Linux

For Linux binaries, just `cmake .. -DCMAKE_BUILD_TYPE=Debug` (or `Release`, or `RelWithDebInfo`) will suffice.
