
cmake_minimum_required(VERSION 3.10)

project(SerialTerminal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -std=c++17")
set(GCC_COVERAGE_LINKER_FLAGS "")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

string(APPEND CMAKE_CXX_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS})
string(APPEND CMAKE_EXE_LINKER_FLAGS  ${GCC_COVERAGE_LINKER_FLAGS})

add_executable(SerialTerminal
    src/main.cpp
    src/serial/SerialPorts.cpp
)

target_link_libraries(SerialTerminal serialport)

add_custom_target(run
    COMMAND ${CMAKE_SOURCE_DIR}/bin/SerialTerminal
    DEPENDS ${CMAKE_SOURCE_DIR}/bin/SerialTerminal
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)