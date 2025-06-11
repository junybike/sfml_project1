#include "player.h"

Player::Player(float x, float y) 
{
    if (!texture.loadFromFile("assets/stickman.png"))
    {
        std::cerr << "Error: Failed to load stickman.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(1.f, 1.f);

    this->setFrameWidth(sprite.getTexture()->getSize().x);
    this->setFrameHeight(sprite.getTexture()->getSize().y);

    facingRight = true;
    canAttack = true;

    speed = 150.f;
    jumpStrength = 700.f;

    maxHealth = 100;
    health = 100;
    healthBar.setSize(sf::Vector2f(50.f, 5.f));
    healthBar.setFillColor(sf::Color::Green);
}

void Player::handleInput(std::vector<Entity*>& entities, sf::RenderWindow& window) 
{
    velocity.x = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        velocity.x = -speed;
        sprite.setScale(-1.f, 1.f);
        sprite.setOrigin(106, 0);
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        velocity.x = speed;
        sprite.setScale(1.f, 1.f);
        sprite.setOrigin(0, 0);
        facingRight = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->isOnGround()) 
    {
        velocity.y = -jumpStrength;
        this->setOnGround(false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->attackHit(entities, window);
    }
}

void Player::update(float deltaTime, std::vector<Platform>& platforms, std::vector<Entity*>& entities, sf::RenderWindow& window) 
{
    handleInput(entities, window);
    applyGravity(deltaTime, platforms);

    // Temporary invincible after damage
    if (this->isInvincible())
    {
        this->setInvincibleTime(deltaTime + getInvincibleTime());
        if (getInvincibleTime() >= 2.f)
        {
            this->setInvincible(false);
            setInvincibleTime(0.f);
        }
    }
    this->handleKb(deltaTime);
}

void Player::attackHit(std::vector<Entity*>& entities, sf::RenderWindow& window)
{
    if (!canAttack || cooldownClock.getElapsedTime().asSeconds() < 0.5f) return;
    canAttack = false;

    cooldownClock.restart();
    sf::FloatRect attackZone;
    sf::Vector2f pos = sprite.getPosition();

    float attackWidth = 50.f;
    float attackHeight = sprite.getGlobalBounds().height;

    if (facingRight) attackZone = sf::FloatRect(pos.x + sprite.getGlobalBounds().width, pos.y, attackWidth, attackHeight);
    else attackZone = sf::FloatRect(pos.x - attackWidth, pos.y, attackWidth, attackHeight);

    for (auto& e : entities)
    {
        if (attackZone.intersects(e->getHitbox()))
        {
            sf::Vector2f kbVelocity = facingRight ? sf::Vector2f(200.f, 0.f) : sf::Vector2f(-200.f, 0.f);
            e->takeDamage(20, kbVelocity, 0.2f);
        }
    }

    canAttack = true;
}