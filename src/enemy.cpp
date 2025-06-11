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

    this->setFrameWidth(sprite.getTexture()->getSize().x);
    this->setFrameHeight(sprite.getTexture()->getSize().y);

    speed = 1.f;
    velocity = {0.f, 0.f};
    attacking = false;

    maxHealth = 50;
    health = 50;
    healthBar.setSize(sf::Vector2f(50.f, 5.f));
    healthBar.setFillColor(sf::Color::Red);
}

void Enemy::update(float deltaTime, Entity& player, std::vector<Platform>& platforms) 
{
    if (this->isAlive())
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
    }

    applyGravity(deltaTime, platforms);
    handleKb(deltaTime);
}

bool Enemy::isAttacking() const
{
    return attacking;
}