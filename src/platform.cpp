#include "platform.h"

Platform::Platform(float x, float y, float width, float height)
{
    shape.setSize({width, height});
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color(100, 100, 100));
}

void Platform::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::FloatRect Platform::getBounds() const 
{
    return shape.getGlobalBounds();
}