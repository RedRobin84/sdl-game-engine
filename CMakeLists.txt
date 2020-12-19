cmake_minimum_required(VERSION 3.0.0)
project(lazyfoo_cmaketools VERSION 0.1.0)

#flags

set(CMAKE_CXX_STANDARD 20)

#path to cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

#SDL packages

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

file(GLOB SOURCES "src/*.cpp")

add_executable(lazyfoo_cmaketools ${SOURCES})

target_link_libraries(lazyfoo_cmaketools SDL2::Main SDL2::Image SDL2::TTF)

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET lazyfoo_cmaketools POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:lazyfoo_cmaketools>)
    endforeach()
endif()

#tests

enable_testing()
add_executable(logger_test tests/LoggerTest.cpp src/Logger.cpp)
add_test(LoggerTest logger_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include(CTest)
