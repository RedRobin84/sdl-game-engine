# MY_FIRST_ENGINE

Sandbox project for practicing both legacy and modern C++ features. Although application is fully operational, it's not meant to be put on production.

## CONTENTS:

### Engine parts:

#### Dynamic asset pool
TODO: desc

### Language features:

#### Legacy C and C++: (TODO: Add Filenames and line numbers)

- enum flags and bitwise operators
- c string usage, manipulation, char buffers
- dynamic memory allocation

#### Design Patterns:

- **Monostate** - Logger
- **Singleton** - Renderer - Although I'd rather pass renderer by reference (using pointers) I wanted to try famous (infamous) Singleton to get knowledge about it's advantages and disadvantages in practice.
- **Factory** - AssetFactory and ProgramFactory for creating class instances

#### C++11:

- smart pointers

#### DevOps etc.:

- built with Cmake using CMake Tools on Visual Studio Code
- Cross-platform development: (Linux(GCC), Mac(Clang), Windows(MinGW GCC))

#### Testing framework (CTest variant)

### LIBRARIES (Sub-projects):

- Logger (TODO: needs more info and description)

## Environment setup

- project setup is adapted to **64-bit** environment.
- **Visual Studio Code** as an IDE
- **Clang** as a compiler
- **Clang tools (clang-tidy, clang-format)** 
- **cppcheck** static analyzer

#### VS Code extensions
- **clangd** extension - for better intellisense and Clang code checks
    - or **Microsoft C/C++ extension** for debugging. Note: Don't use both at same time.
- **CMake** extension for CMake syntax
- **CMake Tools** extension for automatized CMake environment

### Linux

### Mac

### Windows

## Some inspiration from...

- [cpp-starter-project](https://github.com/lefticus/cpp_starter_project) by lefticus(Jason Turner) et.al

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

    Separate ProgramTypeEnum states (TERMINUS) to new Enum to new ProgramState enum with ADD, REPLACE, RESET, TERMINUS states.
ADD -> default (add program to stack)
REPLACE -> replace current ending program with desired one
RESET -> clear Registry and then Program stack