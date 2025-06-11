#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>

#include "platform.h"

class Entity 
{
    public:

        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2f velocity;

        Entity();
        virtual ~Entity();

        virtual void update(float deltaTime, std::vector<Platform>& platforms);
        virtual void draw(sf::RenderWindow& window);
        virtual sf::FloatRect getHitbox() const;        
        virtual void applyGravity(float deltaTime, std::vector<Platform>& platforms);

        void takeDamage(int dmg, sf::Vector2f kbV, float kbD);
        void handleKb(float deltaTime);

        int getFrameWidth() const;
        void setFrameWidth(const int w);
        int getFrameHeight() const;
        void setFrameHeight(const int h); 

        bool isOnGround() const;
        void setOnGround(const bool option);
        bool isMoving() const;
        void setMoving(const bool option);

        bool isInvincible() const;
        void setInvincible(const bool option);
        float getInvincibleTime() const;
        void setInvincibleTime(const float dt);

        bool isAlive() const;
        int getHealth() const;

        bool isKb() const;
        void setKb(const bool option);
        float getKbTime() const;
        void setKbTime(const float t);
        float getKbDuration() const;
        void setkbDuration(const float t);
        sf::Vector2f getKbVelocity() const;
        void setKbVelocity(const sf::Vector2f v);

    protected:

        int maxHealth;
        int health;
        sf::RectangleShape healthBar;
    
    private:

        int frameWidth;
        int frameHeight;

        bool onGround;
        bool moving;

        bool invincible;
        float invincibleTime;

        bool kb;
        float kbTime;
        float kbDuration;
        sf::Vector2f kbVelocity;

};

#endif