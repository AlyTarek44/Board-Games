cmake_minimum_required(VERSION 3.16)
project(OOP_Games)

set(CMAKE_CXX_STANDARD 17)

# Add the executable with the new file names
add_executable(
        OOP_Games
        main.cpp
        NUMTTT_classes.cpp
        XO_Classes.cpp
        FourInRow_Classes.cpp
        Misere_Classes.cpp
        FiveXFiveTTT_Classes.cpp
        SUS_Classes.h
        SUS_Classes.cpp
)