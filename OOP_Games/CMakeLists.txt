cmake_minimum_required(VERSION 3.16)
project(OOP_Games)

set(CMAKE_CXX_STANDARD 17)

# Add the executable with the new file names
add_executable(
        OOP_Games
        NumTTT_Board.cpp
        NumTTT_UI.cpp
        main.cpp
)