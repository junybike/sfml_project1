#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation 
{
    public:

        Animation();
        Animation(sf::Texture* texture, int frameCnt, float frameDuration, int frameWidth, int frameHeight);

        void update(float deltaTime);
        void applyToSprite(sf::Sprite& sprite);
        void reset();

    private:

        sf::Texture* texture;
        int frameCnt;
        int curFrame;

        float frameDuration;
        float elapsedTime;

        int frameWidth;
        int frameHeight;
};

#endif