#pragma once
#include "Question.h"

class TextQuestion : public Question {
public:
    TextQuestion(const std::string& text, const std::vector<std::string>& opts, 
                 const std::string& answer, const std::string& md, int lvl);
    void display() const override;
    void openMedia() const override;
    void closeMedia() const override;
};