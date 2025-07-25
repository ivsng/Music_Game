cmake_minimum_required(VERSION 3.10)
project(MusicGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем директории заголовков
include_directories(include)
include_directories("C:/SFML-3.0.0/include")

# Собираем все исходные файлы
file(GLOB SOURCES 
    "src/*.cpp"
)

file(GLOB HEADERS 
    "include/*.h"
    "include/*.hpp"
)

# Создаем исполняемый файл
add_executable(music_game ${SOURCES} ${HEADERS})

# Линкуем библиотеки SFML (полные пути к файлам)
target_link_libraries(music_game 
    "C:/SFML-3.0.0/lib/libsfml-graphics.a"
    "C:/SFML-3.0.0/lib/libsfml-window.a"
    "C:/SFML-3.0.0/lib/libsfml-system.a"
    "C:/SFML-3.0.0/lib/libsfml-audio.a"
)

# Копируем assets и DLL файлы при сборке
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/SFML-3.0.0/bin"
    ${CMAKE_BINARY_DIR}
    COMMENT "Copying assets and SFML DLLs"
)