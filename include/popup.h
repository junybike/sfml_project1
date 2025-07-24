#ifndef POPUP_H
#define POPUP_H

#include <SFML/Graphics.hpp>
#include <string>

class Popup
{
    public:

        Popup();

        void show(const std::string& message, float duration = 3.f);
        void update(float deltaTime);
        void draw(sf::RenderWindow& window);

        sf::Clock getClock();

    private:

        sf::Text text;
        sf::Font font;
        sf::Clock clock;
        float timer;
        float displayDuration;
        bool visible;
};

#endif