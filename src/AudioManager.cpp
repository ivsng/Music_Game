#include "AudioManager.h"
#include <cstdlib>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #define PLAY_CMD "start "
    #define STOP_CMD "taskkill /im Microsoft.Media.Player.exe /f >nul 2>&1"
#else
    #define PLAY_CMD "xdg-open "
    #define STOP_CMD "pkill vlc >/dev/null 2>&1"
#endif

static bool soundPlaying = false;

void AudioManager::play(const std::string& filePath) {
    if (!soundPlaying) {
        system((PLAY_CMD + filePath).c_str());
        soundPlaying = true;
    }
}

void AudioManager::stop() {
    if (soundPlaying) {
        system(STOP_CMD);
        soundPlaying = false;
    }
}

bool AudioManager::isPlaying() {
    return soundPlaying;
}