#include "entity.h"

Entity::Entity() : velocity(0.f, 0.f), onGround(false) {}

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Entity::getHitbox() const 
{
    return sprite.getGlobalBounds();
}
