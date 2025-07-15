#include "Menu.h"
#include "AudioManager.h"

int main() {
    AudioManager::play("assets/global/audio/main_theme.m4a");
    
    Menu menu;
    menu.showMainMenu();
    
    AudioManager::stop();
    return 0;
}