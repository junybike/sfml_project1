#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unistd.h>

class Entity 
{
    public:

        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2f velocity;

        int frameWidth;
        int frameHeight;

        bool onGround;
        bool isMoving;

        Entity();
        virtual ~Entity() = default;

        virtual void update(float deltaTime) = 0;
        virtual void draw(sf::RenderWindow& window);
        virtual sf::FloatRect getHitbox() const;        

        bool isInvincible() const;
        void setInvincible(const bool option);
        float getInvincibleTime() const;
        void setInvincibleTime(const float dt);
    
    private:

        bool invincible;
        float invincibleTime;
};

#endif