#include "player.h"

Player::Player(float x, float y) 
{
    if (!texture.loadFromFile("assets/stickman.png"))
    {
        std::cerr << "Error: Failed to load stickman.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.5f, 0.5f);

    speed = 150.f;
    jumpStrength = 200.f;
    velocity = {0.f, 0.f};
    onGround = false;
}

void Player::handleInput() 
{
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        velocity.x = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        velocity.x = speed;
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

    const float gravity = 300.f;
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
