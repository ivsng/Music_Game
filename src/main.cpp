#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include <fstream>
#include "../include/json.hpp"
using json = nlohmann::json;

// Структура для хранения вопроса
struct Question {
    std::string type;
    std::string mode;
    std::string file;
    std::string question;
    int answer;
    std::vector<std::string> options;
    sf::Texture imageTexture;
};


int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Music Game");
    window.setFramerateLimit(60);

    // Цвет фона
    sf::Color bgColor(189, 176, 161);

    // Пути к файлам
    std::string basePath = "../";
    std::string soundOnPath = basePath + "assets/icons/sound_on.png";
    std::string soundOffPath = basePath + "assets/icons/sound_off.png";
    std::string playBtnPath = basePath + "assets/icons/play_button.png";
    std::string rulesBtnPath = basePath + "assets/icons/rules_button.png";
    std::string rulesImgPath = basePath + "assets/icons/all_rules.png";
    std::string bestsongPath = basePath + "assets/icons/bestsong_mode.png";
    std::string historyPath = basePath + "assets/icons/history_mode.png";
    std::string menuBtnPath = basePath + "assets/icons/menu_button.png";
    std::string musicPath = basePath + "assets/global/audio/main_theme.ogg";

    // Загрузка текстур
    sf::Texture soundTexture, playTexture, rulesTexture, rulesImgTexture;
    sf::Texture bestsongTexture, historyTexture, menuTexture;
    
    if (!soundTexture.loadFromFile(soundOnPath) || 
        !playTexture.loadFromFile(playBtnPath) ||
        !rulesTexture.loadFromFile(rulesBtnPath) ||
        !rulesImgTexture.loadFromFile(rulesImgPath) ||
        !bestsongTexture.loadFromFile(bestsongPath) ||
        !historyTexture.loadFromFile(historyPath) ||
        !menuTexture.loadFromFile(menuBtnPath)) {
        std::cerr << "ERROR: Failed to load textures\n";
        return 1;
    }

    // Загрузка шрифта
    sf::Font font;
    if (!font.openFromFile(basePath + "assets/global/fonts/arial.ttf")) {
        std::cerr << "ERROR: Failed to load font\n";
        return 1;
    }

    // Настройка кнопки звука
    sf::Sprite soundButton(soundTexture);
    const float soundBtnSize = 80.f;
    float soundScale = soundBtnSize / soundTexture.getSize().x;
    soundButton.setScale({soundScale, soundScale});
    soundButton.setPosition({window.getSize().x - soundBtnSize - 20.f, 20.f});

    // Настройка кнопок Play и Rules
    sf::Sprite playButton(playTexture), rulesButton(rulesTexture);
    
    const float mainBtnWidth = 400.f;
    float playScale = mainBtnWidth / playTexture.getSize().x;
    float rulesScale = mainBtnWidth / rulesTexture.getSize().x;
    
    playButton.setScale({playScale, playScale});
    rulesButton.setScale({rulesScale, rulesScale});

    // Настройка изображения правил
    sf::Sprite rulesImage(rulesImgTexture);
    float rulesImgScale = std::min(
        0.7f * window.getSize().x / rulesImgTexture.getSize().x,
        0.7f * window.getSize().y / rulesImgTexture.getSize().y
    );
    rulesImage.setScale({rulesImgScale, rulesImgScale});
    rulesImage.setPosition({
        (window.getSize().x - rulesImgTexture.getSize().x * rulesImgScale) / 2.f,
        (window.getSize().y - rulesImgTexture.getSize().y * rulesImgScale) / 2.f
    });

    // Настройка новых кнопок режимов
    sf::Sprite bestsongButton(bestsongTexture), historyButton(historyTexture), menuButton(menuTexture);
    
    const float modeBtnWidth = 300.f;
    float bestsongScale = modeBtnWidth / bestsongTexture.getSize().x;
    float historyScale = modeBtnWidth / historyTexture.getSize().x;
    float menuScale = modeBtnWidth / menuTexture.getSize().x;
    
    bestsongButton.setScale({bestsongScale, bestsongScale});
    historyButton.setScale({historyScale, historyScale});
    menuButton.setScale({menuScale, menuScale});

    // Позиционирование кнопок
    playButton.setPosition({
        (window.getSize().x - playTexture.getSize().x * playScale) / 2.f,
        window.getSize().y / 3.f - playTexture.getSize().y * playScale / 2.f
    });
    
    rulesButton.setPosition({
        (window.getSize().x - rulesTexture.getSize().x * rulesScale) / 2.f,
        window.getSize().y / 2.f
    });
    
    bestsongButton.setPosition({
        (window.getSize().x - bestsongTexture.getSize().x * bestsongScale) / 2.f,
        window.getSize().y / 4.f
    });
    
    historyButton.setPosition({
        (window.getSize().x - historyTexture.getSize().x * historyScale) / 2.f,
        window.getSize().y / 2.f
    });
    
    menuButton.setPosition({
        (window.getSize().x - menuTexture.getSize().x * menuScale) / 2.f,
        window.getSize().y * 3.f / 4.f
    });

    // Состояния отображения
    enum class AppState { MAIN_MENU, SHOW_RULES, SHOW_MODES, BEST_SONG_MODE, HISTORY_MODE };
    AppState currentState = AppState::MAIN_MENU;
    bool isSoundOn = true;

    // Музыка
    sf::Music theme;
    if (!theme.openFromFile(musicPath)) {
        std::cerr << "ERROR: Failed to load music\n";
        return 1;
    }
    theme.setLooping(true);
    theme.play();

    // Загрузка вопросов из JSON
    std::vector<Question> questions;
    try {
        std::ifstream i("questions.json");
        json j;
        i >> j;
        
        for (const auto& item : j) {
            Question q;
            q.type = item["type"];
            q.mode = item["mode"];
            if (q.type == "image") {
                q.file = basePath + item["file"].get<std::string>();
                if (!q.imageTexture.loadFromFile(q.file)) {
                    std::cerr << "Failed to load image: " << q.file << "\n";
                    continue;
                }
            }
            q.question = item["question"];
            q.answer = std::stoi(std::string(item["answer"]));
            for (const auto& opt : item["options"]) {
                q.options.push_back(opt);
            }
            questions.push_back(q);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading questions: " << e.what() << "\n";
        return 1;
    }

    // Фильтрация вопросов по режимам
    std::vector<Question> bestSongQuestions, historyQuestions;
    for (const auto& q : questions) {
        if (q.mode == "best_song_ever") bestSongQuestions.push_back(q);
        else if (q.mode == "history") historyQuestions.push_back(q);
    }

    // Текущий вопрос
    int currentQuestionIndex = 0;
    bool showWrongAnswer = false;
    std::vector<Question>* currentQuestions = nullptr;
    sf::Texture emptyTexture;
    sf::Sprite questionSprite(emptyTexture);

    // Для questionText
    sf::Text questionText(font, "", 36);  // Конструктор с шрифтом, текстом и размером
    questionText.setFillColor(sf::Color::Black);
    questionText.setPosition({100.f, 100.f});
    

    // Для optionTexts
    std::vector<sf::Text> optionTexts;
    for (int i = 0; i < 4; ++i) {
        optionTexts.emplace_back(font, "", 28);  // Используем emplace_back с параметрами конструктора
        optionTexts.back().setFillColor(sf::Color::Black);
        optionTexts.back().setPosition({150.f, 200.f + i * 50.f});
    }


    // Главный цикл
    while (window.isOpen()) {
        // Обработка событий
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() || 
                (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                window.close();
            }

            if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    {mouseEvent->position.x, mouseEvent->position.y});

                if (soundButton.getGlobalBounds().contains(mousePos)) {
                    isSoundOn = !isSoundOn;
                    soundTexture.loadFromFile(isSoundOn ? soundOnPath : soundOffPath);
                    soundButton.setTexture(soundTexture);
                    isSoundOn ? theme.play() : theme.pause();
                }
                else if (currentState == AppState::MAIN_MENU) {
                    if (playButton.getGlobalBounds().contains(mousePos)) {
                        currentState = AppState::SHOW_MODES;
                    }
                    else if (rulesButton.getGlobalBounds().contains(mousePos)) {
                        currentState = AppState::SHOW_RULES;
                    }
                }
                else if (currentState == AppState::SHOW_RULES) {
                    // Клик в любом месте закрывает правила
                    currentState = AppState::MAIN_MENU;
                }
                else if (currentState == AppState::SHOW_MODES) {
                    if (menuButton.getGlobalBounds().contains(mousePos)) {
                        currentState = AppState::MAIN_MENU;
                    }
                    else if (bestsongButton.getGlobalBounds().contains(mousePos)) {
                        currentState = AppState::BEST_SONG_MODE;
                        currentQuestions = &bestSongQuestions;
                        currentQuestionIndex = 0;
                        showWrongAnswer = false;
                    }
                    else if (historyButton.getGlobalBounds().contains(mousePos)) {
                        currentState = AppState::HISTORY_MODE;
                        currentQuestions = &historyQuestions;
                        currentQuestionIndex = 0;
                        showWrongAnswer = false;
                    }
                   
                    else if (currentState == AppState::BEST_SONG_MODE || currentState == AppState::HISTORY_MODE) {
                        if (menuButton.getGlobalBounds().contains(mousePos)) {
                            currentState = AppState::SHOW_MODES;
                        }
                        else {
                            // Проверка ответов
                            for (int i = 0; i < 4; ++i) {
                                if (optionTexts[i].getGlobalBounds().contains(mousePos)) {
                                    if (i + 1 == (*currentQuestions)[currentQuestionIndex].answer) {
                                        // Правильный ответ
                                        currentQuestionIndex++;
                                        if (currentQuestionIndex >= currentQuestions->size()) {
                                            currentState = AppState::SHOW_MODES;
                                        }
                                        showWrongAnswer = false;
                                    } else {
                                        // Неправильный ответ
                                        showWrongAnswer = true;
                                    }
                                    break; 
                                }
                            }
                        }
                    }
                }
            }
        }

        // Отрисовка
        window.clear(bgColor);
        
        if (currentState == AppState::MAIN_MENU) {
            window.draw(playButton);
            window.draw(rulesButton);
        }
        else if (currentState == AppState::SHOW_RULES) {
            window.draw(rulesImage);
        }
        else if (currentState == AppState::SHOW_MODES) {
            window.draw(bestsongButton);
            window.draw(historyButton);
            window.draw(menuButton);
        }
        else if (currentState == AppState::BEST_SONG_MODE || currentState == AppState::HISTORY_MODE) {
            if (currentQuestions && currentQuestionIndex < currentQuestions->size()) {
                const Question& q = (*currentQuestions)[currentQuestionIndex];

                // Отображение вопроса
                questionText.setString(q.question);
                window.draw(questionText);

                // Отображение изображения (если есть)
                if (q.type == "image") {
                    questionSprite.setTexture(q.imageTexture);
                    float maxWidth = 600.f;
                    float maxHeight = 400.f;
                    float scale = std::min(
                        maxWidth / q.imageTexture.getSize().x,
                        maxHeight / q.imageTexture.getSize().y
                    );
                    questionSprite.setScale({scale, scale});
                    questionSprite.setPosition(
                        {(window.getSize().x - q.imageTexture.getSize().x * scale) / 2.f,
                        150.f}
                    );
                    window.draw(questionSprite);
                    
                    // Смещаем текст вопроса ниже картинки
                    questionText.setPosition({100.f, 150.f + q.imageTexture.getSize().y * scale + 20.f});
                } else {
                    questionText.setPosition({100.f, 150.f});
                }

                // Отображение вариантов ответов
                for (int i = 0; i < 4 && i < q.options.size(); ++i) {
                    optionTexts[i].setString(q.options[i]);
                    window.draw(optionTexts[i]);
                }

                // Отображение сообщения об ошибке
                if (showWrongAnswer) {
                    sf::Text wrongText(font);  // Шрифт передаётся в конструкторе
                    wrongText.setString("Wrong answer! Try again.");
                    wrongText.setCharacterSize(30);                    
                    wrongText.setFillColor(sf::Color::Red);
                    wrongText.setPosition({100.f, 50.f});
                    window.draw(wrongText);
                }
            }
            window.draw(menuButton);
        }
        
        window.draw(soundButton);
        window.display();
    }

    return 0;
}