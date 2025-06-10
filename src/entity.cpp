#include "entity.h"

Entity::Entity() : velocity(0.f, 0.f), onGround(false), moving(false), frameHeight(0), frameWidth(0), 
                    invincible(false), invincibleTime(0.f) {}

Entity::~Entity() {};

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Entity::update(float deltaTime, std::vector<Platform>& platforms)
{

}

sf::FloatRect Entity::getHitbox() const 
{
    sf::Vector2f pos = sprite.getPosition();
    return sf::FloatRect(pos.x, pos.y, frameWidth, frameHeight);
}

void Entity::applyGravity(float deltaTime, std::vector<Platform>& platforms)
{
    const float gravity = 1700.f;
    velocity.y += gravity * deltaTime;
    sprite.move(velocity * deltaTime);

    if (velocity.y > 0.f)
    {
        sf::FloatRect entityBounds = this->getHitbox();
        sf::FloatRect eFeet(entityBounds.left, entityBounds.top + entityBounds.height - 5.f, entityBounds.width, 5.f);
        
        for (const auto& p : platforms)
        {
            sf::FloatRect platformBounds = p.getBounds();
            
            if (eFeet.intersects(platformBounds))
            {
                sprite.setPosition(sprite.getPosition().x, platformBounds.top - entityBounds.height);
                velocity.y = 0.f;
                this->setOnGround(true);
                break;
            }
        }
    }
}

// ============================================================================
//                              Getter and Setter 
// ============================================================================

int Entity::getFrameWidth() const
{
    return frameWidth;
}
void Entity::setFrameWidth(const int w)
{
    frameWidth = w;
}
int Entity::getFrameHeight() const
{
    return frameHeight;
}
void Entity::setFrameHeight(const int h)
{
    frameHeight = h;
} 

bool Entity::isOnGround() const
{
    return onGround;
}
void Entity::setOnGround(const bool option)
{
    onGround = option;
}
bool Entity::isMoving() const
{
    return moving;
}
void Entity::setMoving(const bool option)
{
    moving = option;
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