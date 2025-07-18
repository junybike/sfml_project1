#include "entity.h"

Entity::Entity() : velocity(0.f, 0.f), onGround(false), moving(false), frameHeight(0), frameWidth(0), 
                    invincible(false), invincibleTime(0.f), guarding(false), kb(false), kbTime(0.f), kbDuration(0.f), kbVelocity(0.f,0.f) {}

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

void Entity::applyGravity(float deltaTime, std::vector<Structure*>& structures)
{
    const float gravity = 1700.f;
    velocity.y += gravity * deltaTime;

    sf::Vector2f horizontalMove(velocity.x * deltaTime, 0.f);
    tryMove(horizontalMove, structures);

    sf::Vector2f verticalMove(0.f, velocity.y * deltaTime);
    sprite.move(verticalMove);

    sf::FloatRect entityBounds = getHitbox();
    sf::FloatRect eFeet(entityBounds.left, entityBounds.top + entityBounds.height - 5.f, entityBounds.width, 5.f);

    for (const auto& s : structures)
    {
        sf::FloatRect platformBounds = s->getBounds();
        
        if (eFeet.intersects(platformBounds) && velocity.y > 0.f && s->isPlatform())
        {
            sprite.setPosition(sprite.getPosition().x, platformBounds.top - entityBounds.height);
            velocity.y = 0.f;
            setOnGround(true);
            break;
        }
    }
}

void Entity::standOnPlatform(Structure* platform)
{
    sf::FloatRect entityBounds = this->getHitbox();
    sf::FloatRect platformBounds = platform->getBounds();

    sprite.setPosition(sprite.getPosition().x, platformBounds.top - entityBounds.height);
    velocity.y = 0.f;
    this->setOnGround(true);
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

void Entity::handleKb(float deltaTime, std::vector<Structure*>& structures)
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
            sf::Vector2f movement = kbVelocity * deltaTime * (1.f - t);             
            tryMove(movement, structures);

            kbTime += deltaTime;
        }
    }   
}

bool Entity::tryMove(sf::Vector2f movement, const std::vector<Structure*>& structures)
{
    sprite.move(movement);
    for (const auto& s : structures)
    {
        if (s->isWall() && getHitbox().intersects(s->getHitbox()))
        {
            sprite.move(-movement);
            return false;
        }
    }
    return true;
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
void Entity::setInvincible(const bool option, int damage)
{
    invincible = option;
    if (option && damage) sprite.setColor(sf::Color(255, 255, 255, 128));
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

bool Entity::isGuarding() const
{
    return guarding;
}
void Entity::setGuarding(const bool option)
{
    guarding = option;
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