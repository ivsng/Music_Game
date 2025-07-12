cmake_minimum_required(VERSION 3.10)
project(Music_Game)

set(CMAKE_CXX_STANDARD 17)
include_directories(include) # Для json.hpp

add_executable(music_game src/main.cpp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)