MY_FIRST_ENGINE

Study project for practicing both legacy and modern C++ features.

PRACTICE CONTENTS:

Legacy C and C++:

- enum flags and bitwise operators
- c string usage, manipulation, char buffers
- dynamic memory allocation

Design Patterns:

- Monostate(Logger)

C++11:

- smart pointers

DevOps etc.:

- built with Cmake using CMake Tools on Visual Studio Code
- Cross-platform development: (Linux(GCC), Mac(Clang), Windows(MinGW GCC))

Testing framework (CTest variant)

TODO:

Engine:
    - more info logs, debug logs...
    - Texture caching
        - Program: list of static textures (frequently used ones)
        - Program: Texture cache - list of common textures (between two nieghbor programs)

Tests:
    - more tests for Logger