#include "player.h"

Player::Player(float x, float y) 
{
    if (!texture.loadFromFile("assets/stickman.png"))
    {
        std::cerr << "Error: Failed to load stickman.png\n";
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.5f, 0.5f);
    velocity = sf::Vector2f(0.f, 0.f);
    onGround = false;
}

void Player::handleInput() 
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        sprite.move(-5.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        sprite.move(5.f, 0.f);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onGround) 
    {
        velocity.y = -12.f;
        onGround = false;
    }
}

void Player::applyGravity(float groundY) 
{
    velocity.y += 0.75f; // gravity
    sprite.move(0.f, velocity.y);

    if (sprite.getPosition().y + sprite.getGlobalBounds().height >= groundY) 
    {
        sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
        velocity.y = 0.f;
        onGround = true;
    }
}

void Player::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);
}
