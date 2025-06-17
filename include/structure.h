#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Structure 
{
    public:

        Structure(float x, float y, float width, float height, sf::Color color);
        virtual ~Structure() = default;

        virtual void draw(sf::RenderWindow& window);

        sf::FloatRect getBounds() const;
        sf::FloatRect getHitbox() const; 

        virtual bool isPlatform() const = 0;
        virtual bool isWall() const = 0;
    
    protected:

        float s_width;
        float s_height;
        sf::RectangleShape shape;
};

#endif