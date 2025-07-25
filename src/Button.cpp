#include "Button.h"
#include <iostream>

Button::Button(const std::string& texturePath, sf::Vector2f pos, float buttonWidth) 
    : position(pos), width(buttonWidth), visible(true) {
    
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "ERROR: Failed to load texture: " << texturePath << std::endl;
        return;
    }
    
    // Создаем спрайт ПОСЛЕ загрузки текстуры
    sprite = std::make_unique<sf::Sprite>(texture);
    
    // Масштабирование до нужной ширины - SFML 3.0 синтаксис
    float scale = width / static_cast<float>(texture.getSize().x);
    sprite->setScale(sf::Vector2f(scale, scale));  // Используем ->
    sprite->setPosition(position);
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

bool Button::contains(sf::Vector2f point) const {
    return sprite ? sprite->getGlobalBounds().contains(point) : false;
}

void Button::render(sf::RenderWindow& window) const {
    if (visible && sprite) {
        window.draw(*sprite);  // Разыменовываем указатель
    }
}

void Button::handleClick() {
    if (onClick) {
        onClick();
    }
}

void Button::setVisible(bool isVisible) {
    visible = isVisible;
}

bool Button::isVisible() const {
    return visible;
}

sf::Vector2f Button::getPosition() const {
    return position;
}

sf::Vector2f Button::getSize() const {
    if (!sprite) return sf::Vector2f(0, 0);
    sf::FloatRect bounds = sprite->getGlobalBounds();  // Используем ->
    return sf::Vector2f(bounds.size.x, bounds.size.y);
}

void Button::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
    if (sprite) {
        sprite->setPosition(position);
    }
}