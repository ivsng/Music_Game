# Music Game - One Direction Quiz
Викторина по песням и истории группы One Direction

## ВНИМАНИЕ!! Открывается только на маленьком окне!

Интерактивная игра-викторина с двумя режимами: **History** (интервью, дневники, факты о группе) и **Best Song Ever** (песни, альбомы, клипы). Игра включает графический интерфейс, музыкальное сопровождение и систему вопросов с изображениями.

## Требования

### Системные требования
- **ОС**: Windows 10/11 (64-bit)
- **Компилятор**: MinGW-w64 (GCC 14.2.0)
- **Система сборки**: CMake 3.10+

### Библиотеки
- **SFML 3.0.0** - для графики, аудио и оконного управления
- **nlohmann/json** - для парсинга JSON файлов с вопросами (включен в проект)

## Установка зависимостей

### 1. Установка компилятора
Скачайте и установите [WinLibs UCRT 14.2.0](https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.1-12.0.0-ucrt-r2/winlibs-x86_64-posix-seh-gcc-14.2.0-mingw-w64ucrt-12.0.0-r2.7z)

### 2. Установка SFML
Скачайте и распакуйте [SFML 3.0 для MinGW](https://www.sfml-dev.org/files/SFML-3.0.0-windows-gcc-14.2.0-mingw-64-bit.zip) в `C:\SFML-3.0.0\`

### 3. Настройка путей
Убедитесь, что MinGW и CMake добавлены в системную переменную PATH.

## Структура проекта

```
Music_Game/
├── include/           
│   ├── Button.h       
│   ├── SoundButton.h  # Звуковая кнопка
│   ├── Game.h         # Главный класс игры
│   ├── QuestionManager.h  # Управление вопросами
│   └── json.hpp       # Библиотека для работы с JSON
├── src/               # Исходные файлы (.cpp)
│   ├── main.cpp       # Точка входа
│   ├── Button.cpp     # Базовый класс кнопки
│   ├── SoundButton.cpp # Звуковая кнопка
│   ├── Game.cpp       # Основная логика игры
│   └── QuestionManager.cpp # Работа с вопросами
├── assets/            # Ресурсы
│   ├── questions.json # База данных вопросов
│   ├── icons/         # Иконки
│   ├── best_song_ever/image/ 
│   ├── history/image/  
│   ├── global/audio/  
│   └── fonts/         # Шрифт
├── CMakeLists.txt     
└── README.md          # Документация проекта
```

## Сборка проекта

### Первоначальная сборка
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
```

### Быстрая пересборка
```bash
cmake --build build --config Release
или
cmake --build .
```

## Запуск игры
```bash
./music_game.exe
```