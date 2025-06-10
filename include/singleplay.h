#ifndef SINGLEPLAY_H
#define SINGLEPLAY_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "player.h"
#include "enemy.h"

class SinglePlay
{
    public: 

        SinglePlay();
        void play(sf::RenderWindow& window);
    
    private:

        int uptime;
};

void display_hitbox(sf::RectangleShape& box1, Entity& entity, sf::RenderWindow& window);

#endif