#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
protected:
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    sf::Vector2f position;
    float width;
    std::function<void()> onClick;
    bool visible;

public:
    Button(const std::string& texturePath, sf::Vector2f pos, float buttonWidth);
    virtual ~Button() = default;

    // Виртуальные методы для полиморфизма
    virtual void setOnClick(std::function<void()> callback);
    virtual bool contains(sf::Vector2f point) const;
    virtual void render(sf::RenderWindow& window) const;
    virtual void handleClick();
    
    // Методы для управления видимостью
    void setVisible(bool isVisible);
    bool isVisible() const;
    
    // Получение позиции и размеров
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    
    // Обновление позиции
    void setPosition(sf::Vector2f newPosition);
};

#endif