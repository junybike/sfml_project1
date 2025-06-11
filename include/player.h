#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
    public:

        Player(float x, float y);

        void handleInput(std::vector<Entity*>& entities, sf::RenderWindow& window);
        void update(float deltaTime, std::vector<Platform>& platforms, std::vector<Entity*>& entities, sf::RenderWindow& window);
        
        void attackHit(std::vector<Entity*>& entities, sf::RenderWindow& window);

    private:

        bool facingRight;

        float speed;
        float jumpStrength;

        sf::Clock cooldownClock;
        bool canAttack;
};

#endif