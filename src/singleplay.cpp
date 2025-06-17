#include "singleplay.h"

SinglePlay::SinglePlay()
{
    view.setSize(1600, 900);
    uptime = 0;
}

void SinglePlay::play(sf::RenderWindow& window)
{
    sf::Clock clock;
    bool quit = false;

    std::vector<Structure*> structures = 
    {
        new Platform(0.f, 550.f, 800.f, 50.f),     // Ground
        new Platform(300.f, 400.f, 200.f, 20.f),   // Second floor
        new Platform(100.f, 300.f, 100.f, 20.f),   // Third floor

        new Wall(300.f, 450.f, 40.f, 100.f),
        new Wall(500.f, 300.f, 40.f, 200.f)
    };

    // Player
    Player* player = new Player(100, 100);

    // Enemy
    Enemy* enemy = new Enemy(400.f, 100.f);
    
    // Hitbox
    sf::RectangleShape box1;
    sf::RectangleShape box2;

    // Entity vector
    std::vector<Entity*> entities;
    entities.push_back(enemy);    

    while (!quit)
    {
        sf::Event event;  
        float deltaTime = clock.restart().asSeconds();
         
        // update player's screen view
        update_playerview(window, player);

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
        
        for (const auto& s : structures)
        {
            s->draw(window);
        }

        player->update(deltaTime, structures, entities, window);
        player->draw(window);
        enemy->update(deltaTime, *player, structures);
        enemy->draw(window);

        display_hitbox(box1, *player, window);
        display_hitbox(box2, *enemy, window);

        window.display();
    }
    
    for (auto& e : entities)
    {
        delete e;
    }
    for (auto& s : structures)
    {
        delete s;
    }

    entities.clear();
    delete player;
}

void SinglePlay::update_playerview(sf::RenderWindow& window, Player* player)
{
    sf::Vector2f center = player->sprite.getPosition();
    float maxY = view.getSize().y / 2.f;
    float minY = view.getSize().y / 2.f;

    center.y = std::clamp(center.y, minY - 200, maxY + 200);
    
    view.setCenter(center);
    window.setView(view);
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

