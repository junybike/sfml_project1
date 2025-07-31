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
        sf::FloatRect getHitbox() const;

    private:
    
        sf::Texture texture;
        sf::Texture animation;
        sf::Sprite sprite;
        PlayerState state;
        float animationTime = 0.f;

};

#endif