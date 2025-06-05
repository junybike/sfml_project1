#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"


class Enemy : public Entity
{
    public:

        float speed;
        bool attacking;

        Enemy(float x, float y);

        void update(float deltaTime, const sf::Vector2f& playerPos);
        void update(float deltaTime) override;
        bool isAttacking() const;

        sf::FloatRect getHitbox() const override; 
};

#endif