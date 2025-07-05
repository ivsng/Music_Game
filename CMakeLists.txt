# cmake_minimum_required(VERSION 3.10.0)
# project(Music_Game_oned VERSION 0.1.0 LANGUAGES C CXX)

# add_executable(Music_Game_oned main.cpp)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

cmake_minimum_required(VERSION 3.10)
project(Music_Game)

set(CMAKE_CXX_STANDARD 17)
add_executable(music_game main.cpp)