cmake_minimum_required(VERSION 3.5)

project(03.guess_the_number)

add_executable(guess_the_number 
    main.cpp
    GameResult.h
    GameResult.cpp
    Play.h
    Play.cpp
    Records.h
    Records.cpp
)

set_target_properties(guess_the_number PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)