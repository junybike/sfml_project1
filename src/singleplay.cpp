#include "singleplay.h"

SinglePlay::SinglePlay()
{
    uptime = 0;
}

void SinglePlay::play(sf::RenderWindow& window)
{
    sf::Clock clock;
    bool quit = false;

    std::vector<Platform> platforms = 
    {
        Platform(0.f, 550.f, 800.f, 50.f),     // Ground
        Platform(300.f, 400.f, 200.f, 20.f),   // Second floor
        Platform(100.f, 300.f, 100.f, 20.f),   // Third floor
    };

    // Player
    Player player(100, 100);

    // Enemy
    Enemy* enemy = new Enemy(400.f, 100.f);
    
    // Hitbox
    sf::RectangleShape box1;
    sf::RectangleShape box2;

    std::vector<Entity*> entities;
    entities.push_back(enemy); 

    while (!quit)
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;   

        // exiting game
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
        
        for (const auto& p : platforms)
        {
            p.draw(window);
        }

        player.update(deltaTime, platforms, entities, window);
        player.draw(window);
        enemy->update(deltaTime, player, platforms);
        enemy->draw(window);

        display_hitbox(box1, player, window);
        display_hitbox(box2, *enemy, window);

        window.display();
    }
    
    for (auto& e : entities)
    {
        delete e;
    }
    entities.clear();
}

void display_hitbox(sf::RectangleShape& box, Entity& entity, sf::RenderWindow& window)
{
    box.setPosition(entity.getHitbox().left, entity.getHitbox().top);
    box.setSize(sf::Vector2f(entity.getFrameWidth(), entity.getFrameHeight()));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(1.f);
    window.draw(box);
}