cmake_minimum_required(VERSION 3.16)

project(OS_VME_13 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(
    src
    src/core
    src/io
)

add_executable(OS_VME_13
    src/main.cpp

    src/core/page_replacement.h
    src/core/page_replacement.cpp

    src/core/fifo.cpp
    src/core/lru.cpp
    src/core/opt.cpp
    src/core/second_chance.cpp

    src/io/input_csv.h
    src/io/input_csv.cpp
)

target_link_libraries(OS_VME_13
    PRIVATE Qt6::Widgets
)
