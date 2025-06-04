#ifndef PLAYER_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{
    public:

        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2f velocity;
        bool onGround;

        Player(float x, float y);

        void handleInput();
        void applyGravity(float groundY);
        void draw(sf::RenderWindow& window); 
};


#endif