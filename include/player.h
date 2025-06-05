#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"


class Player : public Entity
{
    public:

        float speed;
        float jumpStrength;

        Player(float x, float y);

        void handleInput();
        void update(float deltaTime) override;
        sf::FloatRect getHitbox() const override; 
};


#endif