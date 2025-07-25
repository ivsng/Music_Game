#include "Game.h"
#include <iostream>

Game::Game() 
    : window(sf::VideoMode({1280, 720}), "Music Game")
    , backgroundColor(189, 176, 161)
    , currentState(GameState::MAIN_MENU)
    , feedbackState(FeedbackState::NONE)
    , basePath("../") {
    
    window.setFramerateLimit(60);
    
    // Инициализация компонентов
    initializeMusic();
    initializeUI();
    
    // Создание менеджера вопросов - загружает JSON
    questionManager = std::make_unique<QuestionManager>(basePath + "assets/");
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>() || 
            (event->is<sf::Event::KeyPressed>() && 
             event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
            window.close();
        }

        if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                {mouseEvent->position.x, mouseEvent->position.y});

            // Обработка звуковой кнопки (видна всегда)
            if (soundButton->contains(mousePos)) {
                soundButton->handleClick();
            }
            // Обработка других кнопок в зависимости от состояния
            else {
                switch (currentState) {
                    case GameState::MAIN_MENU:
                        for (auto& button : menuButtons) {
                            if (button->contains(mousePos)) {
                                button->handleClick();
                                break;
                            }
                        }
                        break;
                        
                    case GameState::SHOW_RULES:
                        // Клик в любом месте закрывает правила
                        changeState(GameState::MAIN_MENU);
                        break;
                        
                    case GameState::SHOW_MODES:
                        for (auto& button : modeButtons) {
                            if (button->contains(mousePos)) {
                                button->handleClick();
                                break;
                            }
                        }
                        break;
                        
                    case GameState::QUESTION_HISTORY:
                    case GameState::QUESTION_BESTSONG:
                        // Обработка состояний обратной связи
                        if (feedbackState == FeedbackState::CORRECT_ANSWER) {
                            // Клик в любом месте переходит к следующему вопросу
                            feedbackState = FeedbackState::NONE;
                            nextQuestion();
                            break;
                        }
                        if (feedbackState == FeedbackState::WRONG_ANSWER) {
                            // Клик в любом месте убирает сообщение о неправильном ответе
                            feedbackState = FeedbackState::NONE;
                            break;
                        }
                        
                        // Проверка кнопки Home
                        if (homeButton->contains(mousePos)) {
                            homeButton->handleClick();
                            break;
                        }
                        
                        // Обработка кнопок ответов (только если нет активной обратной связи)
                        for (size_t i = 0; i < questionButtons.size(); ++i) {
                            if (questionButtons[i]->contains(mousePos)) {
                                checkAnswer(i);
                                break;
                            }
                        }
                        break;
                }
            }
        }
    }
}

void Game::update() {
    // Обновление логики игры (если необходимо)
}

void Game::render() {
    // Обновляем позиции элементов при каждой отрисовке
    updateElementPositions();
    
    window.clear(backgroundColor);
    
    switch (currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
            break;
        case GameState::SHOW_RULES:
            renderRules();
            break;
        case GameState::SHOW_MODES:
            renderModes();
            break;
        case GameState::QUESTION_HISTORY:
        case GameState::QUESTION_BESTSONG:
            renderQuestion();
            break;
    }
    
    // Звуковая кнопка отображается всегда
    soundButton->render(window);
    
    window.display();
}

void Game::initializeMusic() {
    std::string musicPath = basePath + "assets/global/audio/main_theme.ogg";
    if (!backgroundMusic.openFromFile(musicPath)) {
        std::cerr << "ERROR: Failed to load music: " << musicPath << std::endl;
        return;
    }
    backgroundMusic.setLooping(true);
    backgroundMusic.play();
}

void Game::initializeUI() {
    // Загрузка шрифта - в SFML 3.0 используется openFromFile
    if (!font.openFromFile(basePath + "assets/fonts/arial.ttf")) {
        std::cerr << "Warning: Failed to load font, using default" << std::endl;
    }
    
    // Создание звуковой кнопки
    soundButton = std::make_unique<SoundButton>(
        basePath + "assets/icons/sound_on.png",
        basePath + "assets/icons/sound_off.png",
        sf::Vector2f(window.getSize().x - 100.f, 20.f),
        80.f,
        &backgroundMusic
    );
    
    // Загрузка изображения правил
    if (rulesTexture.loadFromFile(basePath + "assets/icons/all_rules.png")) {
        rulesImage = std::make_unique<sf::Sprite>(rulesTexture);  // Создаем sprite с текстурой
        
        // Масштабирование изображения правил - используем Vector2f
        float rulesScale = std::min(
            0.7f * window.getSize().x / rulesTexture.getSize().x,
            0.7f * window.getSize().y / rulesTexture.getSize().y
        );
        rulesImage->setScale(sf::Vector2f(rulesScale, rulesScale));
        rulesImage->setPosition(sf::Vector2f(
            (window.getSize().x - rulesTexture.getSize().x * rulesScale) / 2.f,
            (window.getSize().y - rulesTexture.getSize().y * rulesScale) / 2.f
        ));
    } else {
        std::cerr << "ERROR: Failed to load rules image" << std::endl;
    }
    
    // Загрузка изображения неправильного ответа
    if (wrongTexture.loadFromFile(basePath + "assets/icons/wrong_button.png")) {
        wrongImage = std::make_unique<sf::Sprite>(wrongTexture);
        // Центрирование
        wrongImage->setPosition(sf::Vector2f(
            (window.getSize().x - wrongTexture.getSize().x) / 2.f,
            (window.getSize().y - wrongTexture.getSize().y) / 2.f
        ));
    }
    
    // Загрузка изображения правильного ответа
    if (nextTexture.loadFromFile(basePath + "assets/icons/next_button.png")) {
        nextImage = std::make_unique<sf::Sprite>(nextTexture);
        // Центрирование
        nextImage->setPosition(sf::Vector2f(
            (window.getSize().x - nextTexture.getSize().x) / 2.f,
            (window.getSize().y - nextTexture.getSize().y) / 2.f
        ));
    }
    
    setupMenuButtons();
    setupModeButtons();
    setupQuestionButtons();
}

void Game::setupMenuButtons() {
    // Кнопка Play - позиция рассчитывается динамически
    auto playButton = std::make_unique<Button>(
        basePath + "assets/icons/play_button.png",
        sf::Vector2f(0, 0), // Временная позиция
        400.f
    );
    playButton->setOnClick([this]() { changeState(GameState::SHOW_MODES); });
    
    // Кнопка Rules
    auto rulesButton = std::make_unique<Button>(
        basePath + "assets/icons/rules_button.png",
        sf::Vector2f(0, 0), // Временная позиция
        400.f
    );
    rulesButton->setOnClick([this]() { changeState(GameState::SHOW_RULES); });
    
    menuButtons.push_back(std::move(playButton));
    menuButtons.push_back(std::move(rulesButton));
}

void Game::setupModeButtons() {
    // Кнопка History
    auto historyButton = std::make_unique<Button>(
        basePath + "assets/icons/history_mode.png",
        sf::Vector2f(0, 0), // Временная позиция
        300.f
    );
    historyButton->setOnClick([this]() { 
        questionManager->filterQuestionsByMode("history");
        loadQuestion();
        changeState(GameState::QUESTION_HISTORY); 
    });
    
    // Кнопка Best Song Ever
    auto bestsongButton = std::make_unique<Button>(
        basePath + "assets/icons/bestsong_mode.png",
        sf::Vector2f(0, 0), // Временная позиция
        300.f
    );
    bestsongButton->setOnClick([this]() { 
        questionManager->filterQuestionsByMode("best_song_ever");
        loadQuestion();
        changeState(GameState::QUESTION_BESTSONG); 
    });
    
    // Кнопка Menu (возврат в главное меню)
    auto menuButton = std::make_unique<Button>(
        basePath + "assets/icons/menu_button.png",
        sf::Vector2f(0, 0), // Временная позиция
        300.f
    );
    menuButton->setOnClick([this]() { changeState(GameState::MAIN_MENU); });
    
    modeButtons.push_back(std::move(historyButton));
    modeButtons.push_back(std::move(bestsongButton));
    modeButtons.push_back(std::move(menuButton));
}

void Game::setupQuestionButtons() {
    // Создаем 4 кнопки для ответов
    for (int i = 0; i < 4; ++i) {
        auto answerButton = std::make_unique<Button>(
            basePath + "assets/icons/answer_button.png",
            sf::Vector2f(0, 0), // Временная позиция
            200.f
        );
        
        // Захватываем индекс ответа
        int answerIndex = i;
        answerButton->setOnClick([this, answerIndex]() { 
            checkAnswer(answerIndex); 
        });
        
        questionButtons.push_back(std::move(answerButton));
    }
    
    // Кнопка Home (возврат в главное меню)
    homeButton = std::make_unique<Button>(
        basePath + "assets/icons/home_button.png",
        sf::Vector2f(20.f, 20.f),  // Левый верхний угол
        60.f
    );
    homeButton->setOnClick([this]() { changeState(GameState::MAIN_MENU); });
}

void Game::updateMenuButtonPositions() {
    if (menuButtons.size() >= 2) {
        // Play кнопка
        menuButtons[0]->setPosition(sf::Vector2f(
            (window.getSize().x - 400.f) / 2.f,
            window.getSize().y / 3.f - 50.f
        ));
        
        // Rules кнопка
        menuButtons[1]->setPosition(sf::Vector2f(
            (window.getSize().x - 400.f) / 2.f,
            window.getSize().y / 2.f
        ));
    }
}

void Game::updateModeButtonPositions() {
    if (modeButtons.size() >= 3) {
        // History кнопка
        modeButtons[0]->setPosition(sf::Vector2f(
            (window.getSize().x - 300.f) / 2.f,
            window.getSize().y / 4.f
        ));
        
        // Best Song кнопка
        modeButtons[1]->setPosition(sf::Vector2f(
            (window.getSize().x - 300.f) / 2.f,
            window.getSize().y / 2.f
        ));
        
        // Menu кнопка
        modeButtons[2]->setPosition(sf::Vector2f(
            (window.getSize().x - 300.f) / 2.f,
            window.getSize().y * 3.f / 4.f
        ));
    }
}

void Game::updateQuestionButtonPositions() {
    // Обновляем позиции кнопок ответов
    for (size_t i = 0; i < questionButtons.size(); ++i) {
        questionButtons[i]->setPosition(sf::Vector2f(
            (window.getSize().x - 200.f) / 2.f,
            400.f + i * 60.f
        ));
    }
    
    // Обновляем позицию Home кнопки
    if (homeButton) {
        homeButton->setPosition(sf::Vector2f(20.f, 20.f));
    }
    
    // Обновляем позицию звуковой кнопки
    if (soundButton) {
        soundButton->setPosition(sf::Vector2f(window.getSize().x - 100.f, 20.f));
    }
}

void Game::updateElementPositions() {
    updateMenuButtonPositions();
    updateModeButtonPositions();
    updateQuestionButtonPositions();
    
    // Обновляем позиции изображений
    if (rulesImage) {
        float rulesScale = std::min(
            0.7f * window.getSize().x / rulesTexture.getSize().x,
            0.7f * window.getSize().y / rulesTexture.getSize().y
        );
        rulesImage->setScale(sf::Vector2f(rulesScale, rulesScale));
        rulesImage->setPosition(sf::Vector2f(
            (window.getSize().x - rulesTexture.getSize().x * rulesScale) / 2.f,
            (window.getSize().y - rulesTexture.getSize().y * rulesScale) / 2.f
        ));
    }
    
    // Обновляем позиции feedback изображений
    if (wrongImage) {
        wrongImage->setPosition(sf::Vector2f(
            (window.getSize().x - wrongTexture.getSize().x) / 2.f,
            (window.getSize().y - wrongTexture.getSize().y) / 2.f
        ));
    }
    
    if (nextImage) {
        nextImage->setPosition(sf::Vector2f(
            (window.getSize().x - nextTexture.getSize().x) / 2.f,
            (window.getSize().y - nextTexture.getSize().y) / 2.f
        ));
    }
}

void Game::changeState(GameState newState) {
    currentState = newState;
    feedbackState = FeedbackState::NONE; // Сбрасываем обратную связь при смене состояния
}

void Game::loadQuestion() {
    Question* question = questionManager->getCurrentQuestion();
    if (!question) return;
    
    // Загрузка изображения вопроса (только если это image тип)
    if (question->type == "image" && !question->file.empty()) {
        if (questionTexture.loadFromFile(basePath + question->file)) {
            questionImage = std::make_unique<sf::Sprite>(questionTexture);  // Создаем sprite с текстурой
            
            // Масштабирование изображения (формат 3:2) - используем Vector2f
            float scale = std::min(
                0.6f * window.getSize().x / questionTexture.getSize().x,
                0.3f * window.getSize().y / questionTexture.getSize().y
            );
            questionImage->setScale(sf::Vector2f(scale, scale));
            questionImage->setPosition(sf::Vector2f(
                (window.getSize().x - questionTexture.getSize().x * scale) / 2.f,
                50.f
            ));
        }
    }
}

void Game::checkAnswer(int answerIndex) {
    bool correct = questionManager->isAnswerCorrect(answerIndex);
    
    if (correct) {
        feedbackState = FeedbackState::CORRECT_ANSWER;
    } else {
        feedbackState = FeedbackState::WRONG_ANSWER;
    }
}

void Game::nextQuestion() {
    questionManager->nextQuestion();
    
    if (questionManager->hasMoreQuestions()) {
        loadQuestion();
    } else {
        // Вопросы закончились - возврат в меню режимов
        questionManager->resetQuestions();
        changeState(GameState::SHOW_MODES);
    }
}

void Game::renderMainMenu() {
    for (const auto& button : menuButtons) {
        button->render(window);
    }
}

void Game::renderRules() {
    if (rulesImage) {
        window.draw(*rulesImage);
    }
}

void Game::renderModes() {
    for (const auto& button : modeButtons) {
        button->render(window);
    }
}

void Game::renderQuestion() {
    Question* question = questionManager->getCurrentQuestion();
    if (!question) return;
    
    // Отрисовка изображения вопроса (только если это image тип)
    if (question->type == "image" && questionImage) {
        // Пересчитываем позицию изображения для адаптивности
        float scale = std::min(
            0.6f * window.getSize().x / questionTexture.getSize().x,
            0.3f * window.getSize().y / questionTexture.getSize().y
        );
        questionImage->setScale(sf::Vector2f(scale, scale));
        questionImage->setPosition(sf::Vector2f(
            (window.getSize().x - questionTexture.getSize().x * scale) / 2.f,
            50.f
        ));
        window.draw(*questionImage);
    }
    
    // Создаем и отрисовываем текст вопроса с динамической позицией
    sf::Text questionText(font, question->question, 24);
    questionText.setFillColor(sf::Color::Black);
    
    sf::FloatRect textBounds = questionText.getLocalBounds();
    float textY = (question->type == "image") ? 300.f : 200.f;
    questionText.setPosition(sf::Vector2f(
        (window.getSize().x - textBounds.size.x) / 2.f,
        textY
    ));
    
    window.draw(questionText);
    
    // Отрисовка кнопок ответов с текстом (только если нет активной обратной связи)
    if (feedbackState == FeedbackState::NONE) {
        for (size_t i = 0; i < questionButtons.size() && i < question->options.size(); ++i) {
            questionButtons[i]->render(window);
            
            // Отрисовка текста ответа на кнопке - создаем Text с шрифтом
            sf::Text answerText(font, question->options[i], 18);
            answerText.setFillColor(sf::Color(139, 69, 93));  // Темно-розовый цвет
            
            // Центрирование текста на кнопке
            sf::Vector2f buttonPos = questionButtons[i]->getPosition();
            sf::Vector2f buttonSize = questionButtons[i]->getSize();
            sf::FloatRect answerBounds = answerText.getLocalBounds();
            
            answerText.setPosition(sf::Vector2f(
                buttonPos.x + (buttonSize.x - answerBounds.size.x) / 2.f,
                buttonPos.y + (buttonSize.y - answerBounds.size.y) / 2.f - answerBounds.position.y
            ));
            
            window.draw(answerText);
        }
    }
    
    // Отрисовка кнопки Home
    homeButton->render(window);
    
    // Отрисовка обратной связи
    if (feedbackState == FeedbackState::WRONG_ANSWER && wrongImage) {
        window.draw(*wrongImage);
    } else if (feedbackState == FeedbackState::CORRECT_ANSWER && nextImage) {
        window.draw(*nextImage);
    }
}