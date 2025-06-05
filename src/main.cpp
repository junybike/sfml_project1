#include <SFML/System.hpp>

#include "player.h"
#include "enemy.h"
#include "singleplay.h"

enum class GameState 
{
    LOBBY,
    SINGLE,
    MULTI,
    EXITING
};

int main() 
{
    // Set up game window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Stickman Game");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "Failed to load ARIAL.TTF" << std::endl;
    }

    // Title screen
    sf::Text title("Stickman Game", font, 48);
    title.setPosition(200.f, 100.f);
    title.setFillColor(sf::Color::White);

    sf::Text singlePlayer("Single Player", font, 32);
    singlePlayer.setPosition(220.f, 200.f);
    sf::Text multiplayer("Multiplayer", font, 32);
    multiplayer.setPosition(220.f, 250.f);
    sf::Text quit("Quit", font, 32);
    quit.setPosition(220.f, 300.f);

    GameState state = GameState::LOBBY;

    while (window.isOpen()) 
    {        
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) window.close();
            if (state == GameState::LOBBY && event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (singlePlayer.getGlobalBounds().contains(mousePos))
                    {
                        state = GameState::SINGLE;
                    }
                    else if (multiplayer.getGlobalBounds().contains(mousePos))
                    {
                        state = GameState::MULTI;
                    }
                    else if (quit.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                    }
                }
            }
        }
        window.clear(sf::Color(30, 30, 30));

        if (state == GameState::LOBBY)
        {
            window.draw(title);
            window.draw(singlePlayer);
            window.draw(multiplayer);
            window.draw(quit);

            singlePlayer.setFillColor(sf::Color::White);
            multiplayer.setFillColor(sf::Color::White);
            quit.setFillColor(sf::Color::White);

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (singlePlayer.getGlobalBounds().contains(mousePos)) 
            {
                singlePlayer.setFillColor(sf::Color(255, 165, 0));
            } 
            else if (multiplayer.getGlobalBounds().contains(mousePos)) 
            {
                multiplayer.setFillColor(sf::Color(255, 165, 0));
            } 
            else if (quit.getGlobalBounds().contains(mousePos)) 
            {
                quit.setFillColor(sf::Color(255, 165, 0));
            }
        }
        else if (state == GameState::SINGLE)
        {   
            SinglePlay session = SinglePlay();
            session.play(window);
            state = GameState::LOBBY;
        }
        else if (state == GameState::MULTI)
        {

        }
        window.display();
    }

    return 0;
}

/*

player.handleInput();

window.clear(sf::Color::White);

sf::RectangleShape box1;
sf::RectangleShape box2;

box1.setPosition(player.getHitbox().left, player.getHitbox().top);
box1.setSize(sf::Vector2f(player.getHitbox().width, player.getHitbox().height));
box1.setFillColor(sf::Color::Transparent);
box1.setOutlineColor(sf::Color::Red);
box1.setOutlineThickness(1.f);
window.draw(box1);

enemy.update(deltaTime, player.sprite.getPosition());
enemy.draw(window);

box2.setPosition(enemy.getHitbox().left, enemy.getHitbox().top);
box2.setSize(sf::Vector2f(enemy.getHitbox().width, enemy.getHitbox().height));
box2.setFillColor(sf::Color::Transparent);
box2.setOutlineColor(sf::Color::Red);
box2.setOutlineThickness(1.f);
window.draw(box2);

if (player.getHitbox().intersects(enemy.getHitbox())) {
    std::cout << "Collision!\n";
}
*/