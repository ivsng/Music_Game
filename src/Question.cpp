#include "Question.h"
#include <cstdlib> // Для system()
#include <thread>  // Для std::this_thread
#include <chrono>  // Для std::chrono::milliseconds

Question::Question(const std::string& text, const std::vector<std::string>& opts, 
                   const std::string& answer, const std::string& md, int lvl, const std::string& file)
    : questionText(text), options(opts), correctAnswer(answer), mode(md), level(lvl), filePath(file), mediaOpened(false) {}

Question::~Question() {
    closeMedia();
}

void Question::openMedia() const {
    if (!filePath.empty()) {
        #ifdef _WIN32
            system(("start " + filePath).c_str());
        #else
            system(("xdg-open " + filePath).c_str());
        #endif
        mediaOpened = true;
    }
}

void Question::closeMedia() const {
    if (mediaOpened) {
        #ifdef _WIN32
            system("taskkill /im Microsoft.Photos.exe /f >nul 2>&1");
            system("taskkill /im vlc.exe /f >nul 2>&1");
            system("taskkill /im wmplayer.exe /f >nul 2>&1");
        #else
            system("pkill eog >/dev/null 2>&1");
            system("pkill vlc >/dev/null 2>&1");
        #endif
        mediaOpened = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

bool Question::checkAnswer(const std::string& userAnswer) const {
    return userAnswer == correctAnswer;
}

std::string Question::getMode() const {
    return mode;
}

int Question::getLevel() const {
    return level;
}