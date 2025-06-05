#include "enemy.h"

Enemy::Enemy(float x, float y)
{
    if (!texture.loadFromFile("assets/enemy1.jpeg"))
    {
        std::cerr << "Error: Failed to load enemy1.jpeg" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f);

    speed = 1.f;
    velocity = {0.f, 0.f};
    attacking = false;
    onGround = false;
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPos) 
{
    sf::Vector2 pos = sprite.getPosition();
    float dx = playerPos.x - pos.x;
    float dy = playerPos.y - pos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    attacking = false;

    if (distance > 20.f && distance < 400.f)
    {
        sf::Vector2f direction = sf::Vector2(dx / distance, 0.f);
        sprite.move(direction * speed);
    }
    else 
    {
        velocity.x = 0.f;
    }
    
    if (distance <= 20.f) 
    {
        attacking = true;
        std::cout << "ENEMY ATTACKING" << std::endl;
    }
}

void Enemy::update(float deltaTime)
{
    const float gravity = 20.f;
    velocity.y += gravity * deltaTime;

    sprite.move(velocity * deltaTime);

    float groundY = 600.f;
    if (sprite.getPosition().y + sprite.getGlobalBounds().height >= groundY)
    {
        sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
        velocity.y = 0.f;
        onGround = true;
    }
}

bool Enemy::isAttacking() const
{
    return attacking;
}

sf::FloatRect Enemy::getHitbox() const 
{
    sf::Vector2f pos = sprite.getPosition();
    return sf::FloatRect(pos.x + 8.f, pos.y + 5.f, 36.f, 60.f);
}