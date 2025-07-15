#include "TextQuestion.h"
#include <iostream>

TextQuestion::TextQuestion(const std::string& text, const std::vector<std::string>& opts, 
                           const std::string& answer, const std::string& md, int lvl)
    : Question(text, opts, answer, md, lvl, "") {}

void TextQuestion::display() const {
    std::cout << "\n" << questionText << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i+1 << ". " << options[i] << "\n";
    }
}

void TextQuestion::openMedia() const {
    // Для текстовых вопросов медиа не открывается
}

void TextQuestion::closeMedia() const {
    // Для текстовых вопросов медиа не закрывается
}