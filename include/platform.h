#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include <vector>

class Platform 
{
    public:

        Platform(float x, float y, float width, float height);

        void draw(sf::RenderWindow& window) const;
        sf::FloatRect getBounds() const;

    private:

        sf::RectangleShape shape;
};

#endif
