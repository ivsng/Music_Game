#pragma once
#include "Question.h"

class VideoQuestion : public Question {
public:
    VideoQuestion(const std::string& text, const std::vector<std::string>& opts, 
                  const std::string& answer, const std::string& md, int lvl, const std::string& file);
    void display() const override;
    void openMedia() const override;
    void closeMedia() const override;
};