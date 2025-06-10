#include "enemy.h"

Enemy::Enemy(float x, float y)
{
    if (!texture.loadFromFile("assets/enemy1.png"))
    {
        std::cerr << "Error: Failed to load enemy1.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(1.f, 1.f);

    frameWidth = 80;
    frameHeight = 80;

    speed = 1.f;
    velocity = {0.f, 0.f};
    attacking = false;
    onGround = false;
}

void Enemy::update(float deltaTime, Entity& player) 
{
    sf::Vector2 pos = sprite.getPosition();
    sf::Vector2f playerPos = player.sprite.getPosition();

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

    if (player.getHitbox().intersects(this->getHitbox()))
    {
        if (!player.isInvincible())
        {
            std::cout << "Collision" << std::endl;
            player.setInvincible(true);
        }    
    }

    update(deltaTime);
}

void Enemy::update(float deltaTime)
{
    // Gravity
    const float gravity = 1500.f;
    velocity.y += gravity * deltaTime;
    sprite.move(velocity * deltaTime);

    float groundY = 550.f;
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