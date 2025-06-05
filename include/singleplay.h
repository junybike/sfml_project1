#ifndef SINGLEPLAY_H
#define SINGLEPLAY_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "player.h"
#include "enemy.h"

class SinglePlay
{
    public: 

        int uptime;

        SinglePlay();
        void play(sf::RenderWindow& window);
};

#endif