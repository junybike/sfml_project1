#include "entity.h"

Entity::Entity() : velocity(0.f, 0.f), onGround(false), isMoving(false), frameHeight(0), frameWidth(0) {}

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Entity::getHitbox() const 
{
    sf::Vector2f pos = sprite.getPosition();
    return sf::FloatRect(pos.x, pos.y, frameWidth, frameHeight);
}
