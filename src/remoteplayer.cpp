#include "remoteplayer.h"

RemotePlayer::RemotePlayer()
{
    if (!texture.loadFromFile("assets/stickman/stickman.png")) std::cerr << "Error: Failed to load stickman.png" << std::endl;
    if (!animation.loadFromFile("assets/stickman/stickman_ani.png")) std::cerr << "Error: Failed to load stickman_ani.png" << std::endl;
    sprite.setTexture(texture);
    animationTime = 0.f;
}

void RemotePlayer::update(float dt)
{
    animationTime += dt;
    sprite.setTexture(animation);

    if (state.curAnimation == "Idle") sprite.setTextureRect(sf::IntRect(static_cast<int>(animationTime / 0.15f) % 8 * 122, 1 * 171, 122, 171));
    else if (state.curAnimation == "Run") sprite.setTextureRect(sf::IntRect(static_cast<int>(animationTime / 0.15f) % 8 * 122, 3 * 171, 122, 171));
    else if (state.curAnimation == "Slide") sprite.setTextureRect(sf::IntRect(static_cast<int>(animationTime / 0.0625f) % 4 * 142, 5 * 171, 142, 171));
    else if (state.curAnimation == "AttackHit") sprite.setTextureRect(sf::IntRect(static_cast<int>(animationTime / 0.05f) % 2 * 157, 0 * 171, 157, 171));
    else if (state.curAnimation == "AttackKick") sprite.setTextureRect(sf::IntRect(static_cast<int>(animationTime / 0.15f) % 2 * 122, 2 * 171, 122, 171));
    else if (state.curAnimation == "guardShield") sprite.setTextureRect(sf::IntRect(static_cast<int>(animationTime / 0.15f) % 2 * 122, 4 * 171, 122, 171));

    if (state.facingRight)
    {
        sprite.setScale(1.f, 1.f);
        sprite.setOrigin(0, 0);
    }
    else if (!state.facingRight)
    {
        sprite.setScale(-1.f, 1.f);
        sprite.setOrigin(106, 0);
    }
}

void RemotePlayer::applyState(PlayerState& s)
{
    state = s;
    animationTime = s.animationTime;
}

void RemotePlayer::draw(sf::RenderWindow& window)
{
    sprite.setPosition(state.position);
    window.draw(sprite);

    sf::RectangleShape healthBarBg(sf::Vector2f(50, 5));
    healthBarBg.setFillColor(sf::Color::Red);
    healthBarBg.setPosition(state.position.x - 25, state.position.y - 40);

    sf::RectangleShape healthBar(sf::Vector2f(50.f * (state.health / 100.f), 5));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(state.position.x - 25, state.position.y - 40);

    window.draw(healthBarBg);
    window.draw(healthBar);
}

PlayerState RemotePlayer::getState() const
{
    return state;
}

sf::FloatRect RemotePlayer::getHitbox() const 
{
    sf::Vector2f pos = sprite.getPosition();
    return sf::FloatRect(pos.x, pos.y, sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
}