cmake_minimum_required(VERSION 3.0.0)
project(lazyfoo_cmaketools VERSION 0.1.0)

#flags

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
add_compile_definitions(LINUX)
endif()

#path to cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

if(WIN32)
list(APPEND CMAKE_PREFIX_PATH "C:/SDL2-2.0.14;C:/SDL2_image-2.0.5;C:/SDL2_ttf-2.0.15;")
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB SOURCES "src/*.cpp")

add_executable(lazyfoo_cmaketools main.cpp ${SOURCES})

option(ENABLE_COMPILER_WARNINGS OFF)
if(ENABLE_COMPILER_WARNINGS)
  target_compile_options(lazyfoo_cmaketools PRIVATE -Wall -Wextra -pedantic -Werror -Wno-vla-extension)
endif()

include(cmake/StaticAnalyzers.cmake)

#SDL packages

FIND_PACKAGE(SDL2 REQUIRED)
Message("")
Message( STATUS "FINDING SDL2" )
IF (${SDL2_FOUND})
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
    Message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
ELSE()
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( FATAL_ERROR "SDL2 NOT FOUND" )
ENDIF()

FIND_PACKAGE(SDL2_IMAGE)
Message("")
Message( STATUS "FINDING SDL2_IMAGE" )
Message( STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
IF (${SDL2_IMAGE_FOUND})
    Message( STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
    Message( STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
ELSE()
    Message( FATAL_ERROR "SDL2_IMAGE NOT FOUND" )
ENDIF()

FIND_PACKAGE(SDL2TTF)
Message("")
Message( STATUS "FINDING SDL2TTF" )
Message( STATUS "SDL2TTF_FOUND: " ${SDL2TTF_FOUND} )
IF (${SDL2TTF_FOUND})
    Message( STATUS "SDL2TTF_INCLUDE_DIR: " ${SDL2TTF_INCLUDE_DIR})
    Message( STATUS "SDL2TTF_LIBRARY: " ${SDL2TTF_LIBRARY})
ELSE()
    Message( FATAL_ERROR "SDL2TTF NOT FOUND" )
ENDIF()

include_directories(include)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(lazyfoo_cmaketools ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})

#tests
include(CTest)

enable_testing()
add_executable(logger_test tests/LoggerTest.cpp src/Logger.cpp)
add_test(LoggerTest logger_test)
add_executable(renderable_test tests/RenderableTest.cpp ${SOURCES})
TARGET_LINK_LIBRARIES(renderable_test ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})
add_test(RenderableTest renderable_test)