cmake_minimum_required(VERSION 4.1)
project(Lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Lab3
    main.cpp
    bfs.cpp
    dfs.cpp
    ids.cpp
    astar.cpp
    path_io.cpp
    board.h
)
