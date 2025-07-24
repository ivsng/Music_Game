cmake_minimum_required(VERSION 3.10)
project(music_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

# Для SFML 3.0
set(SFML_DIR "C:/SFML-3.0.0/lib/cmake/SFML")
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Audio)

add_executable(music_game src/main.cpp)

target_link_libraries(music_game PRIVATE
    SFML::Graphics
    SFML::Audio
)

target_include_directories(music_game PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Копирование всех необходимых ресурсов
add_custom_command(TARGET music_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:music_game>/assets/global/audio
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:music_game>/assets/icons
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/assets/global/audio/main_theme.ogg
        $<TARGET_FILE_DIR:music_game>/assets/global/audio/
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/assets/icons/sound_on.png
        $<TARGET_FILE_DIR:music_game>/assets/icons/
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/assets/icons/sound_off.png
        $<TARGET_FILE_DIR:music_game>/assets/icons/
    COMMAND ${CMAKE_COMMAND} -E copy
        "C:/SFML-3.0.0/bin/sfml-graphics-3.dll"
        "C:/SFML-3.0.0/bin/sfml-audio-3.dll"
        "C:/SFML-3.0.0/bin/sfml-window-3.dll"
        "C:/SFML-3.0.0/bin/sfml-system-3.dll"
        $<TARGET_FILE_DIR:music_game>
    COMMENT "Copying game resources and DLLs..."
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/questions.json
        $<TARGET_FILE_DIR:music_game>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/best_song_ever
        $<TARGET_FILE_DIR:music_game>/assets/best_song_ever
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/history
        $<TARGET_FILE_DIR:music_game>/assets/history
)