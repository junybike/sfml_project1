#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
    enum class AnimationState 
    {
        Idle,
        Run,
        AttackHit
    };

    public:

        Player(float x, float y);

        void handleInput(std::vector<Entity*>& entities, sf::RenderWindow& window);
        void update(float deltaTime, std::vector<Platform>& platforms, std::vector<Entity*>& entities, sf::RenderWindow& window);
        void setAnimationState(AnimationState state);

        void attackHit(std::vector<Entity*>& entities, sf::RenderWindow& window);

    private:

        float speed;
        float jumpStrength;

        sf::Clock cooldownClock;
        bool canAttack;

        AnimationState curState;
        Animation idleAnimation;
        Animation runAnimation;
        Animation attackHitAnimation;

        sf::Texture runTexture;
        sf::Texture idleTexture;
        sf::Texture hitTexture;
};

#endif