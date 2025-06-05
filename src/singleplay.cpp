#include "singleplay.h"

SinglePlay::SinglePlay()
{
    uptime = 0;
}

void SinglePlay::play(sf::RenderWindow& window)
{
    sf::Clock clock;
    bool quit = false;
    
    // Ground
    float groundY = 550;
    sf::RectangleShape ground(sf::Vector2f(800, 50));
    ground.setFillColor(sf::Color(0, 0, 0)); 
    ground.setPosition(0, groundY);

    // Player
    Player player(400.f, groundY);

    // Enemy
    Enemy enemy(400.f, groundY - 64.f);

    while (!quit)
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;   

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                quit = true;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                quit = true; // ESC to return to menu
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(ground);

        player.update(deltaTime);
        player.draw(window);

        window.display();
    }
}