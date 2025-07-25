#include "QuestionManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

QuestionManager::QuestionManager(const std::string& assetsPath) 
    : basePath(assetsPath), currentQuestionIndex(0) {
    
    // Загружаем вопросы из JSON файла
    std::string jsonPath = basePath + "questions.json";
    if (!loadQuestionsFromJSON(jsonPath)) {
        std::cerr << "ERROR: Failed to load questions from " << jsonPath << std::endl;
    }
}

bool QuestionManager::loadQuestionsFromJSON(const std::string& jsonPath) {
    std::string jsonContent = readFileToString(jsonPath);
    if (jsonContent.empty()) {
        return false;
    }
    
    return parseJSON(jsonContent);
}

std::string QuestionManager::readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open file: " << filename << std::endl;
        return "";
    }
    
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool QuestionManager::parseJSON(const std::string& jsonContent) {
    try {
        json j = json::parse(jsonContent);
        allQuestions.clear();
        
        for (const auto& item : j) {
            Question question;
            question.type = item["type"];
            question.mode = item["mode"];
            
            if (item.contains("file")) {
                question.file = item["file"];
            }
            
            question.question = item["question"];
            question.answer = item["answer"];
            question.options = item["options"];
            
            allQuestions.push_back(question);
        }
        
        // std::cout << "Loaded " << allQuestions.size() << " questions from JSON" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }
}

void QuestionManager::filterQuestionsByMode(const std::string& mode) {
    currentQuestions.clear();
    currentQuestionIndex = 0;
    
    for (const auto& question : allQuestions) {
        if (question.mode == mode) {
            currentQuestions.push_back(question);
        }
    }
    
    // std::cout << "Filtered " << currentQuestions.size() << " questions for mode: " << mode << std::endl;
}

Question* QuestionManager::getCurrentQuestion() {
    if (currentQuestionIndex >= currentQuestions.size()) {
        return nullptr;
    }
    return &currentQuestions[currentQuestionIndex];
}

bool QuestionManager::isAnswerCorrect(int answerIndex) {
    Question* question = getCurrentQuestion();
    if (!question) return false;
    
    // Конвертируем строку в число (answer хранится как строка "1", "2", etc.)
    int correctAnswer = std::stoi(question->answer) - 1; // Конвертируем в 0-based индекс
    return answerIndex == correctAnswer;
}

void QuestionManager::nextQuestion() {
    currentQuestionIndex++;
}

bool QuestionManager::hasMoreQuestions() {
    return currentQuestionIndex < currentQuestions.size();
}

void QuestionManager::resetQuestions() {
    currentQuestionIndex = 0;
}