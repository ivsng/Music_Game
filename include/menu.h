#pragma once
#include "Quiz.h"
#include "AudioManager.h"

class Menu {
    Quiz quiz;
    static bool soundEnabled;

public:
    Menu();
    void showMainMenu();
    static void toggleSound();
    static bool isSoundEnabled();
};