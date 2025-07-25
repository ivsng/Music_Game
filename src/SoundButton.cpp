#include "SoundButton.h"
#include <iostream>

SoundButton::SoundButton(const std::string& soundOnPath, const std::string& soundOffPath, 
                        sf::Vector2f pos, float buttonWidth, sf::Music* gameMusic)
    : Button(soundOnPath, pos, buttonWidth), soundEnabled(true), music(gameMusic) {
    
    // Загружаем обе текстуры
    if (!soundOnTexture.loadFromFile(soundOnPath)) {
        std::cerr << "ERROR: Failed to load sound on texture: " << soundOnPath << std::endl;
    }
    
    if (!soundOffTexture.loadFromFile(soundOffPath)) {
        std::cerr << "ERROR: Failed to load sound off texture: " << soundOffPath << std::endl;
    }
    
    // Устанавливаем callback для переключения звука
    setOnClick([this]() { toggleSound(); });
}

void SoundButton::handleClick() {
    // Переопределяем метод базового класса (полиморфизм)
    toggleSound();
}

void SoundButton::toggleSound() {
    soundEnabled = !soundEnabled;
    updateTexture();
    
    if (music) {
        if (soundEnabled) {
            music->play();
        } else {
            music->pause();
        }
    }
}

void SoundButton::updateTexture() {
    if (soundEnabled) {
        sprite = std::make_unique<sf::Sprite>(soundOnTexture);  // Пересоздаем sprite с новой текстурой
    } else {
        sprite = std::make_unique<sf::Sprite>(soundOffTexture);  // Пересоздаем sprite с новой текстурой
    }
    
    // Пересчитываем масштаб для новой текстуры
    const sf::Texture& currentTexture = soundEnabled ? soundOnTexture : soundOffTexture;
    float scale = width / static_cast<float>(currentTexture.getSize().x);
    sprite->setScale(sf::Vector2f(scale, scale));  // Используем Vector2f
    sprite->setPosition(position);
}

bool SoundButton::isSoundEnabled() const {
    return soundEnabled;
}

void SoundButton::setPosition(sf::Vector2f newPosition) {
    Button::setPosition(newPosition);
    position = newPosition;
    updateTexture(); // Пересчитываем позицию при смене текстуры
}