#include "structure.h"

Structure::Structure(float x, float y, float width, float height, sf::Color color)
{
    shape.setPosition(x, y);
    shape.setSize({width, height});
    shape.setFillColor(color);

    s_height = height;
    s_width = width;
}

void Structure::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Structure::getBounds() const 
{
    return shape.getGlobalBounds();
}

sf::FloatRect Structure::getHitbox() const 
{
    sf::Vector2f pos = shape.getPosition();
    return sf::FloatRect(pos.x, pos.y, s_width, s_height);
}