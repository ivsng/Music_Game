#pragma once
#include <string>

class AudioManager {
public:
    static void play(const std::string& filePath);
    static void stop();
    static bool isPlaying();
};