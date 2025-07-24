#include "popup.h"
#include <iostream>

Popup::Popup() : timer(0.f), displayDuration(3.f), visible(false)
{
    if (!font.loadFromFile("assets/ARIAL.TTF")) std::cerr << "popup: Failed to load ARIAL.TTF" << std::endl;

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(200.f, 50.f);
}

void Popup::show(const std::string& message, float duration)
{
    text.setString(message);
    displayDuration = duration;
    timer = 0.f;
    visible = true;
}

void Popup::update(float deltaTime)
{
    if (visible)
    {
        timer += deltaTime;
        if (timer >= displayDuration) visible = false;
    }
}

void Popup::draw(sf::RenderWindow& window)
{
    if (visible) window.draw(text);
}

sf::Clock Popup::getClock()
{
    return clock;
}