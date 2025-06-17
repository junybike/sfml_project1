#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Enemy : public Entity
{
    public:

        Enemy(float x, float y);

        void update(float deltaTime, Entity& player, std::vector<Structure*>& structures);
        bool isAttacking() const;

    private:

        float speed;
        bool attacking;
};

#endif