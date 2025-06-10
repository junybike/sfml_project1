#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
    public:

        Player(float x, float y);

        void handleInput();
        void update(float deltaTime, std::vector<Platform>& platforms) override;

    private:

        float speed;
        float jumpStrength;
};

#endif