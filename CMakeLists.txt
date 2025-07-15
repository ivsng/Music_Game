cmake_minimum_required(VERSION 3.10)
project(Music_Game)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(music_game
    src/main.cpp
    src/Question.cpp
    src/TextQuestion.cpp
    src/ImageQuestion.cpp
    src/VideoQuestion.cpp
    src/Quiz.cpp
    src/Menu.cpp
    src/AudioManager.cpp
)