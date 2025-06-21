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

void Enemy::update(float deltaTime, Entity& player, std::vector<Structure*>& structures) 
{
    applyGravity(deltaTime, structures);
    handleKb(deltaTime, structures);
    
    if (this->isAlive())
    {        
        sf::Vector2 pos = sprite.getPosition();
        sf::Vector2f playerPos = player.sprite.getPosition();

        float dx = playerPos.x - pos.x;
        float dy = playerPos.y - pos.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        facingRight = dx < 0 ? false : true;
        attacking = false;

        if (distance > 0.f && distance < 400.f)
        {
            sf::Vector2f direction = sf::Vector2(dx / distance, 0.f);
            sf::Vector2f movement = direction * speed;
            tryMove(movement, structures);
        }
        else 
        {
            velocity.x = 0.f;
        }

        if (player.getHitbox().intersects(this->getHitbox()) && !isInvincible())
        {
            if (!player.isInvincible())
            {
                sf::Vector2f kbVelocity = facingRight ? sf::Vector2f(200.f, 0.f) : sf::Vector2f(-200.f, 0.f);
                player.takeDamage(10, kbVelocity, 0.2f);
                player.setInvincible(true);
            }    
        }

        // Temporary invincible after damage
        if (isInvincible())
        {
            setInvincibleTime(deltaTime + getInvincibleTime());
            if (getInvincibleTime() >= 2.f)
            {
                setInvincible(false);
                setInvincibleTime(0.f);
            }
        }
    }
    else setInvincible(false);
}

bool Enemy::isAttacking() const
{
    return attacking;
}