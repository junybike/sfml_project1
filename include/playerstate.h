#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "player.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

struct PlayerState 
{
    std::string name;
    sf::Vector2f position;
    sf::Vector2f velocity;

    int health;
    bool canAttack;
    bool isAttacking = false;
    bool facingRight;
    std::string curAnimation;
    float animationTime;

    friend sf::Packet& operator<<(sf::Packet& packet, const PlayerState& ps) 
    {
        return packet << ps.name 
                      << ps.position.x << ps.position.y
                      << ps.velocity.x << ps.velocity.y
                      << ps.health << ps.facingRight 
                      << ps.canAttack<< ps.curAnimation << ps.animationTime;
    }

    friend sf::Packet& operator>>(sf::Packet& packet, PlayerState& ps) 
    {
        return packet >> ps.name 
                      >> ps.position.x >> ps.position.y
                      >> ps.velocity.x >> ps.velocity.y
                      >> ps.health >> ps.facingRight 
                      >> ps.canAttack >> ps.curAnimation >> ps.animationTime;
    }
};

PlayerState getPlayerState(Player* player, std::string name);

void applyPlayerState(Player* player, const PlayerState& state);

void drawPlayer(sf::RenderWindow& window, const PlayerState& state);




#endif