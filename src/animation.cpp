#include "animation.h"

Animation::Animation() {}

Animation::Animation(sf::Texture* t, int frames, float duration, int width, int height) 
    : texture(t), frameCnt(frames), frameDuration(duration), curFrame(0), elapsedTime(0.f), frameWidth(width), frameHeight(height) {}

void Animation::update(float dt)
{
    elapsedTime += dt;
    if (elapsedTime >= frameDuration)
    {
        elapsedTime = 0.f;
        curFrame = (curFrame + 1) % frameCnt;
    }
}

void Animation::applyToSprite(sf::Sprite& sprite)
{
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(curFrame * frameWidth, 0, frameWidth, frameHeight));
}

void Animation::reset()
{
    curFrame = 0;
    elapsedTime = 0.f;
}