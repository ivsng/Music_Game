#ifndef SOUNDBUTTON_H
#define SOUNDBUTTON_H

#include "Button.h"
#include <SFML/Audio.hpp>

class SoundButton : public Button {  // Наследование от Button
private:
    sf::Texture soundOnTexture;
    sf::Texture soundOffTexture;
    bool soundEnabled;
    sf::Music* music;  // Указатель на музыку для управления

public:
    SoundButton(const std::string& soundOnPath, const std::string& soundOffPath, 
                sf::Vector2f pos, float buttonWidth, sf::Music* gameMusic);
    
    // Переопределение методов базового класса (полиморфизм)
    void handleClick() override;
    
    // Специфические методы для звука
    void toggleSound();
    void updateTexture();
    bool isSoundEnabled() const;
    
    // Переопределение метода позиционирования
    void setPosition(sf::Vector2f newPosition);
};

#endif