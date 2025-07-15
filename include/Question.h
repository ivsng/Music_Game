#pragma once
#include <string>
#include <vector>
#include <iostream>

class Question {
protected:
    std::string questionText;
    std::vector<std::string> options;
    std::string correctAnswer;  // Это поле уже существует
    std::string mode;
    int level;
    std::string filePath;
    mutable bool mediaOpened;

public:
    Question(const std::string& text, const std::vector<std::string>& opts, 
             const std::string& answer, const std::string& md, int lvl, const std::string& file);
    virtual ~Question();

    virtual void display() const = 0;
    virtual void openMedia() const;
    virtual void closeMedia() const;
    bool checkAnswer(const std::string& userAnswer) const;
    std::string getMode() const;
    int getLevel() const;
    
    // Добавляем новый метод здесь:
    std::string getCorrectAnswer() const { return correctAnswer; }
};