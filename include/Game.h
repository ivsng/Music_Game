#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include "Button.h"
#include "SoundButton.h"
#include "QuestionManager.h"

class Game {
private:
    // Инкапсуляция - все поля приватные
    sf::RenderWindow window;
    sf::Color backgroundColor;
    sf::Music backgroundMusic;
    
    // Состояния игры
    enum class GameState { 
        MAIN_MENU, 
        SHOW_RULES, 
        SHOW_MODES, 
        QUESTION_HISTORY, 
        QUESTION_BESTSONG 
    };
    GameState currentState;
    
    // Состояния обратной связи
    enum class FeedbackState { NONE, WRONG_ANSWER, CORRECT_ANSWER };
    FeedbackState feedbackState;
    
    // UI элементы
    std::unique_ptr<SoundButton> soundButton;
    std::vector<std::unique_ptr<Button>> menuButtons;
    std::vector<std::unique_ptr<Button>> modeButtons;
    std::vector<std::unique_ptr<Button>> questionButtons;
    std::unique_ptr<Button> homeButton;
    
    // Изображения
    sf::Texture rulesTexture;
    std::unique_ptr<sf::Sprite> rulesImage;
    
    // Система вопросов
    std::unique_ptr<QuestionManager> questionManager;
    sf::Texture questionTexture;
    std::unique_ptr<sf::Sprite> questionImage;
    sf::Font font;
    
    // Обратная связь (правильный/неправильный ответ)
    std::unique_ptr<sf::Sprite> wrongImage;
    sf::Texture wrongTexture;
    std::unique_ptr<sf::Sprite> nextImage;
    sf::Texture nextTexture;
    
    // Пути к ресурсам
    std::string basePath;
    
public:
    Game();
    ~Game() = default;
    
    // Основные методы игры
    void run();
    void handleEvents();
    void update();
    void render();
    
private:
    // Приватные методы для инкапсуляции
    void initializeUI();
    void initializeMusic();
    void setupMenuButtons();
    void setupModeButtons();
    void setupQuestionButtons();
    
    // Методы для смены состояний
    void changeState(GameState newState);
    void loadQuestion();
    void checkAnswer(int answerIndex);
    void nextQuestion();
    
    // Рендер для разных состояний
    void renderMainMenu();
    void renderRules();
    void renderModes();
    void renderQuestion();
    
    // Методы для обновления позиций элементов
    void updateMenuButtonPositions();
    void updateModeButtonPositions();
    void updateQuestionButtonPositions();
    void updateElementPositions();
};

#endif