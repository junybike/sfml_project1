#include "player.h"

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Stickman Game");
    window.setFramerateLimit(60);

    // Ground
    float groundY = 550;
    sf::RectangleShape ground(sf::Vector2f(800, 50));
    ground.setFillColor(sf::Color(0, 0, 0)); 
    ground.setPosition(0, groundY);

    // Player
    Player player(400, groundY);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        player.handleInput();
        player.applyGravity(groundY);

        window.clear(sf::Color::White);
        window.draw(ground);
        player.draw(window);
        window.display();
    }

    return 0;
}
