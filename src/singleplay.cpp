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
    Player player(100, 100);

    // Enemy
    Enemy enemy(400.f, groundY - 64.f);
    
    // Hitbox
    sf::RectangleShape box1;

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

        display_hitbox(box1, player, window);

        window.display();
    }
}

void display_hitbox(sf::RectangleShape& box, Entity& entity, sf::RenderWindow& window)
{
    box.setPosition(entity.getHitbox().left, entity.getHitbox().top);
    box.setSize(sf::Vector2f(entity.frameWidth, entity.frameHeight));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(1.f);
    window.draw(box);
}