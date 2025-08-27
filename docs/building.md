# Build Instructions

## Important Note!

This is very early days for this project. Long-term, I plan on adjusting the CMake build files to accomodate many compilers. Right now, it only has full
support for GCC (either on Linux, or cross-compiling to Windows with MinGW) and has not been tested on Visual Studio or other toolchains.

## Compiling SFML

GORP requires the headers and compiled static library files (.a or .lib) for [SFML 3](https://github.com/SFML/SFML). However you choose to acquire these files,
you should place the `include` folder in the `src/3rdparty/sfml` folder of the GORP repo, and the library files (.a or .lib) in either `lib/linux` or
`lib/windows` (depending on your target platform).

## Compiling GORP

### Windows

To compile via MinGW in WSL from VSCode, ensure the VSCode CMake settings are set to x86_64-w64-mingw32, it should set the rest up automatically. Visual Studio
support will be added eventually.

### Linux

For Linux binaries, just `cmake .. -DCMAKE_BUILD_TYPE=Debug` (or `Release`, or `RelWithDebInfo`) will suffice.
