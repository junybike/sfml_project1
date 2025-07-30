#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "playerstate.h"

#include <SFML/Graphics.hpp>

class RemotePlayer
{
    public:

        RemotePlayer();

        void update(float dt);
        void applyState(PlayerState& state);
        void draw(sf::RenderWindow& window);

        PlayerState getState() const;

    private:
 
        PlayerState state;
        float animationTime;

};

#endif