#include "entity.h"

Entity::Entity() : velocity(0.f, 0.f), onGround(false), isMoving(false), frameHeight(0), frameWidth(0), 
                    invincible(false), invincibleTime(0.f) {}

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Entity::getHitbox() const 
{
    sf::Vector2f pos = sprite.getPosition();
    return sf::FloatRect(pos.x, pos.y, frameWidth, frameHeight);
}

bool Entity::isInvincible() const
{
    return invincible;
}
void Entity::setInvincible(const bool option)
{
    invincible = option;
    if (option) sprite.setColor(sf::Color(255, 255, 255, 128));
    else sprite.setColor(sf::Color::White);
}
float Entity::getInvincibleTime() const
{
    return invincibleTime;
}
void Entity::setInvincibleTime(const float dt)
{
    invincibleTime = dt;
}