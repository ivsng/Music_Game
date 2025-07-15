#include "Menu.h"
#include <iostream>
#include <limits>

bool Menu::soundEnabled = true;

Menu::Menu() {
    quiz.loadQuestions("questions.json");
}

void Menu::showMainMenu() {
    if (soundEnabled) {
        AudioManager::play("assets/global/audio/main_theme.m4a");
    }

    while (true) {
        std::cout << "\n=== One Direction Music Game ===\n";
        std::cout << "1. Play Game\n";
        std::cout << "2. Rules\n";
        std::cout << "3. Score: " << quiz.getScore() << "/" << quiz.getTotalLevels() << "\n";
        std::cout << "4. New Game\n";
        std::cout << "5. " << (soundEnabled ? "Turn Off Sound" : "Turn On Sound") << "\n";
        std::cout << "6. Exit\n";
        std::cout << "Choose an option: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                quiz.startGame();
                break;
            case 2:
                quiz.showRules();
                break;
            case 3:
                std::cout << "Your current score: " << quiz.getScore() << "/" << quiz.getTotalLevels() << "\n";
                break;
            case 4:
                quiz.resetScore();
                std::cout << "Game reset. Ready to start new game!\n";
                break;
            case 5:
                toggleSound();
                break;
            case 6:
                AudioManager::stop();
                return;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::toggleSound() {
    soundEnabled = !soundEnabled;
    if (soundEnabled) {
        AudioManager::play("assets/global/audio/main_theme.m4a");
    } else {
        AudioManager::stop();
    }
}

bool Menu::isSoundEnabled() {
    return soundEnabled;
}