# MY_FIRST_ENGINE

Sandbox project for practicing both legacy and modern C++ features.

### CONTENTS:

#### Legacy C and C++:

- enum flags and bitwise operators
- c string usage, manipulation, char buffers
- dynamic memory allocation

#### Design Patterns:

- Monostate(Logger)

#### C++11:

- smart pointers

#### DevOps etc.:

- built with Cmake using CMake Tools on Visual Studio Code
- Cross-platform development: (Linux(GCC), Mac(Clang), Windows(MinGW GCC))

#### Testing framework (CTest variant)

### LIBRARIES (Sub-projects):

- Logger

## Environment setup

- project setup is adapted to **64-bit** environment.
- **Visual Studio Code** as an IDE
- **Clang** as a compiler

#### VS Code extensions
- **clangd** extension - for better intellisense and Clang code checks
    - or **Microsoft C/C++ extension** for debugging. Note: Don't use both at same time.
- **CMake** extension for CMake syntax
- **CMake Tools** extension for automatized CMake environment

### Linux

### Mac

### Windows

For compiling on Clang under Windows I used MinGW-w64. Thats Clang 11.0.0 on GCC 10.2.0.
- A zip file for Win64 can be found on [WinLibs](http://winlibs.com/) website. Download and extract to **C:/mingw64** or elsewhere
- Ad binary folder to path (In our default case its **C:/mingw64/bin**) - TODO: Detailed how to add env. variable explanation
- TODO: SDL2 libraries install directory
- TODO: settings.json "cmake.preferredGenerators": ["MinGW Makefiles"]

TODO:

Engine:
    - more info logs, debug logs...
    - Texture caching
        - Program: list of static textures (frequently used ones)
        - Program: Texture cache - list of common textures (between two nieghbor programs)

Tests:
    - more tests for Logger