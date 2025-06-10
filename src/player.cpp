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

    frameWidth = sprite.getTexture()->getSize().x;
    frameHeight = sprite.getTexture()->getSize().y;

    speed = 150.f;
    jumpStrength = 500.f;
}

void Player::handleInput() 
{
    velocity.x = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        velocity.x = -speed;
        sprite.setScale(-1.f, 1.f);
        sprite.setOrigin(106, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        velocity.x = speed;
        sprite.setScale(1.f, 1.f);
        sprite.setOrigin(0, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onGround) 
    {
        velocity.y = -jumpStrength;
        onGround = false;
    }
}

void Player::update(float deltaTime) 
{
    handleInput();

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
}