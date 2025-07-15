#include "ImageQuestion.h"
#include <iostream>

ImageQuestion::ImageQuestion(const std::string& text, const std::vector<std::string>& opts, 
                             const std::string& answer, const std::string& md, int lvl, const std::string& file)
    : Question(text, opts, answer, md, lvl, file) {}

void ImageQuestion::display() const {
    std::cout << "\n" << questionText << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i+1 << ". " << options[i] << "\n";
    }
    openMedia();
}

void ImageQuestion::openMedia() const {
    Question::openMedia();
}

void ImageQuestion::closeMedia() const {
    Question::closeMedia();
}