#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
    enum class AnimationState 
    {
        Idle,
        Run,
        Slide,
        AttackHit,
        AttackKick,
        guardShield
    };

    public:

        Player(float x, float y);

        void handleInput(std::vector<Entity*>& entities, sf::RenderWindow& window);
        void update(float deltaTime, std::vector<Structure*>& structures, std::vector<Entity*>& entities, sf::RenderWindow& window);
        void setAnimationState(AnimationState state);

        void slide();
        void attackHit(std::vector<Entity*>& entities, sf::RenderWindow& window);
        void attackKick(std::vector<Entity*>& entities, sf::RenderWindow& window);
        void guardShield();

    private:

        float speed;
        float jumpStrength;

        sf::Clock cooldownClock;
        bool canAttack;

        AnimationState curState;
        Animation idleAnimation;
        Animation runAnimation;
        Animation slideAnimation;
        Animation attackHitAnimation;
        Animation attackKickAnimation;
        Animation guardShieldAnimation;

        sf::Texture runTexture;
        sf::Texture idleTexture;
        sf::Texture slideTexture;
        sf::Texture hitTexture;
        sf::Texture kickTexture;
        sf::Texture shieldTexture;
};

#endif