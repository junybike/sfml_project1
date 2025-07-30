#include "remoteplayer.h"

RemotePlayer::RemotePlayer() : animationTime(0.f) {};

void RemotePlayer::update(float dt)
{
    animationTime += dt;
}

void RemotePlayer::applyState(PlayerState& s)
{
    state = s;
    animationTime = s.animationTime;
}

void RemotePlayer::draw(sf::RenderWindow& window)
{
    sprite.setPosition(state.position);
    sf::RectangleShape shape(sf::Vector2f(50.f, 50.f));
    shape.setPosition(state.position);

    if (state.curAnimation == "walk")
        shape.setFillColor(sf::Color::Blue);
    else if (state.curAnimation == "attack")
        shape.setFillColor(sf::Color::Red);
    else
        shape.setFillColor(sf::Color::Green);

    window.draw(shape);
}

PlayerState RemotePlayer::getState() const
{
    return state;
}