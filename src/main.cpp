#include <SFML/System.hpp>

#include "player.h"
#include "enemy.h"
#include "singleplay.h"
#include "multiplayermenu.h"
#include "multiplayerhost.h"
#include "multiplayerclient.h"

enum class GameState 
{
    LOBBY,
    SINGLE,
    MULTI,
    EXITING
};

int main() 
{
    // Set up game window 800 600
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Stickman Game");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "Failed to load ARIAL.TTF" << std::endl;
    }

    // Title screen
    sf::Text title("Stickman Game", font, 48);
    title.setPosition(625.f, 150.f);
    title.setFillColor(sf::Color::White);

    sf::Text singlePlayer("Singleplayer", font, 32);
    singlePlayer.setPosition(650.f, 300.f);
    sf::Text multiplayer("Multiplayer", font, 32);
    multiplayer.setPosition(650.f, 350.f);
    sf::Text quit("Quit", font, 32);
    quit.setPosition(650.f, 400.f);

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
            window.setView(window.getDefaultView());
        }
        else if (state == GameState::MULTI)
        {
            MultiplayerMenu menu;
            MultiplayerOption choice = menu.show(window);

            if (choice == MultiplayerOption::MAKE_ROOM)
            {
                MultiplayerHost host;
                host.runLobby(window);
            }
            else if (choice == MultiplayerOption::FIND_ROOM)
            {
                MultiplayerClient client;
                auto [ip, name] = client.askForIp(window);
                client.setName(name);

                if (client.connectToServer(ip)) client.waitForHostToStart(window);
            }

            state = GameState::LOBBY;
            window.setView(window.getDefaultView());
        }
        window.display();
    }

    return 0;
}