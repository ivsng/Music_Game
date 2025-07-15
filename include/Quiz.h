#pragma once
#include <vector>
#include <memory>
#include "Question.h"

class Quiz {
    std::vector<std::unique_ptr<Question>> questions;
    int score;
    int totalLevels;

public:
    Quiz();
    void loadQuestions(const std::string& jsonPath);
    void startGame();
    void showRules() const;
    int getScore() const;
    int getTotalLevels() const;
    void resetScore();
};