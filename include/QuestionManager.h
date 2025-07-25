#ifndef QUESTIONMANAGER_H
#define QUESTIONMANAGER_H

#include <vector>
#include <string>

struct Question {
    std::string type;        // "text" или "image"
    std::string mode;        // "history" или "best_song_ever"
    std::string file;        // путь к изображению (если type == "image")
    std::string question;    // текст вопроса
    std::string answer;      // правильный ответ (индекс как строка)
    std::vector<std::string> options; // варианты ответов
};

class QuestionManager {
private:
    // Инкапсуляция - данные скрыты
    std::vector<Question> allQuestions;
    std::vector<Question> currentQuestions;
    int currentQuestionIndex;
    std::string basePath;

public:
    QuestionManager(const std::string& assetsPath);
    ~QuestionManager() = default;
    
    // Публичные методы для работы с вопросами
    bool loadQuestionsFromJSON(const std::string& jsonPath);
    void filterQuestionsByMode(const std::string& mode);
    Question* getCurrentQuestion();
    bool isAnswerCorrect(int answerIndex);
    void nextQuestion();
    bool hasMoreQuestions();
    void resetQuestions();
    
private:
    // Приватные методы для парсинга JSON
    std::string readFileToString(const std::string& filename);
    bool parseJSON(const std::string& jsonContent);
};

#endif