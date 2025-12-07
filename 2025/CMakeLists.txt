cmake_minimum_required(VERSION 3.5)

project(AoC-2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB MAIN_SRCS "*-main.cpp")

foreach(MAIN_SRC ${MAIN_SRCS})
    # Extract day number string from main file
    get_filename_component(MAIN_FILE ${MAIN_SRC} NAME)
    string(REGEX MATCH "[0-9]+" DAY_STR ${MAIN_FILE})
    file (GLOB DAY_FILES "${DAY_STR}-*.txt")

    add_executable(${DAY_STR} ${MAIN_SRC} ${DAY_FILES})
    foreach(DAY_FILE ${DAY_FILES})
        file(COPY ${DAY_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    endforeach()
endforeach()

