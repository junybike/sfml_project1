#include "playerstate.h"

PlayerState getPlayerState(Player* player, std::string name)
{
    PlayerState state;
    state.name = name;             
    state.position = player->sprite.getPosition(); 
    state.velocity = player->velocity;     
    state.health = player->getHealth();         
    state.facingRight = player->facingRight;
    state.canAttack = player->getCanAttack();
    return state;
}

void applyPlayerState(Player* player, const PlayerState& state)
{
    player->sprite.setPosition(state.position);
    player->velocity = state.velocity;
    player->setHealth(state.health);
    player->facingRight = state.facingRight;
    player->setCanAttack(state.canAttack);
}

void drawPlayer(sf::RenderWindow& window, const PlayerState& state)
{
    sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
    shape.setPosition(state.position);
    shape.setFillColor(state.canAttack ? sf::Color::Red : sf::Color::Green);
    window.draw(shape);
}