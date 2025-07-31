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
}

PlayerState RemotePlayer::getState() const
{
    return state;
}