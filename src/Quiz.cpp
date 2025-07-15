#include "Quiz.h"
#include "Menu.h"
#include "AudioManager.h"
#include "TextQuestion.h"
#include "ImageQuestion.h"
#include "VideoQuestion.h"
#include "json.hpp" // Добавляем заголовочный файл для работы с JSON
#include <fstream>  // Для std::ifstream
#include <thread>
#include <chrono>
#include <iostream> // Для std::cout

using json = nlohmann::json; // Определяем псевдоним для пространства имен

Quiz::Quiz() : score(0), totalLevels(0) {}

void Quiz::loadQuestions(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open questions file: " << jsonPath << std::endl;
        return;
    }

    try {
        json data = json::parse(file);

        for (auto& q : data) {
            std::string type = q["type"];
            std::string mode = q["mode"];
            std::string question = q["question"];
            std::vector<std::string> options = q["options"].get<std::vector<std::string>>();
            std::string answer = q["answer"];
            int level = q["level"];
            std::string filePath = q.value("file", "");

            if (type == "text") {
                questions.push_back(std::unique_ptr<Question>(
                    new TextQuestion(question, options, answer, mode, level)));
            } else if (type == "image") {
                questions.push_back(std::unique_ptr<Question>(
                    new ImageQuestion(question, options, answer, mode, level, filePath)));
            } else if (type == "video") {
                questions.push_back(std::unique_ptr<Question>(
                    new VideoQuestion(question, options, answer, mode, level, filePath)));
            }

            totalLevels = std::max(totalLevels, level);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

void Quiz::startGame() {
    AudioManager::stop();
    
    for (auto& question : questions) {
        question->display();
        
        std::string userAnswer;
        std::cin >> userAnswer;
        
        question->closeMedia();
        
        if (question->checkAnswer(userAnswer)) {
            std::cout << "Correct!\n";
            score++;
        } else {
            std::cout << "Wrong! Correct answer was: " << question->getCorrectAnswer() << "\n";
        }
    }
    
    if (Menu::isSoundEnabled()) {
        AudioManager::play("assets/global/audio/main_theme.m4a");
    }
}

void Quiz::showRules() const {
    std::cout << "=== Game Rules ===\n";
    std::cout << "1. Answer questions about One Direction\n";
    std::cout << "2. For media questions, the file will open automatically\n";
    std::cout << "3. Enter the number of the correct answer\n";
}

int Quiz::getScore() const {
    return score;
}

int Quiz::getTotalLevels() const {
    return totalLevels;
}

void Quiz::resetScore() {
    score = 0;
}