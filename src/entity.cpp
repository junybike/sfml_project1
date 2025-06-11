#include "entity.h"

Entity::Entity() : velocity(0.f, 0.f), onGround(false), moving(false), frameHeight(0), frameWidth(0), 
                    invincible(false), invincibleTime(0.f), kb(false), kbTime(0.f), kbDuration(0.f), kbVelocity(0.f,0.f) {}

Entity::~Entity() {};

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(sprite);

    sf::Vector2f pos = sprite.getPosition();
    healthBar.setPosition(pos.x, pos.y - 10.f);
    float healthPercent = std::max(0.f, (float) health / maxHealth);
    healthBar.setSize(sf::Vector2(50.f * healthPercent, 5.f));

    window.draw(healthBar);
}

void Entity::update(float deltaTime, std::vector<Platform>& platforms)
{

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

sf::FloatRect Entity::getHitbox() const 
{
    sf::Vector2f pos = sprite.getPosition();
    return sf::FloatRect(pos.x, pos.y, frameWidth, frameHeight);
}

void Entity::takeDamage(int dmg, sf::Vector2f kbV, float kbD)
{
    health -= dmg;
    if (health < 0) health = 0;

    kb = true;
    kbTime = 0.f;
    this->kbVelocity = kbV;
    this->kbDuration = kbD;
}

void Entity::handleKb(float deltaTime)
{
    if (kb)
    {
        float t = kbTime / kbDuration;
        if (t >= 1.f)
        {
            kb = false;
            kbVelocity = {0.f, 0.f};
        }
        else
        {
            sprite.move(kbVelocity * deltaTime * (1.f - t));
            kbTime += deltaTime;
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

bool Entity::isAlive() const
{
    return health > 0;
}
int Entity::getHealth() const
{
    return health;
}

bool Entity::isKb() const
{
    return kb;
}
void Entity::setKb(const bool option)
{
    kb = option;
}
float Entity::getKbTime() const
{
    return kbTime;
}
void Entity::setKbTime(const float t)
{
    kbTime = t;
}
float Entity::getKbDuration() const
{
    return kbDuration;
}
void Entity::setkbDuration(const float t)
{
    kbDuration = t;
}
sf::Vector2f Entity::getKbVelocity() const
{
    return kbVelocity;
}
void Entity::setKbVelocity(const sf::Vector2f v)
{
    kbVelocity = v;
}