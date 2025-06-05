#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Entity 
{
    public:

        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2f velocity;
        bool onGround;

        Entity();
        virtual ~Entity() = default;

        virtual void update(float deltaTime) = 0;
        virtual void draw(sf::RenderWindow& window);
        virtual sf::FloatRect getHitbox() const;
};

#endif