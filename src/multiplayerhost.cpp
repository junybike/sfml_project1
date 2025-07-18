#include "multiplayerhost.h"

#include <iostream>

void MultiplayerHost::runLobby(sf::RenderWindow& window) 
{
    if (listener.listen(port) != sf::Socket::Done) 
    {
        std::cerr << "Failed to listen on port " << port << std::endl;
        return;
    }

    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    sf::SocketSelector selector;
    selector.add(listener);

    // "Start Game" button
    sf::RectangleShape startButton(sf::Vector2f(200.f, 50.f));
    startButton.setPosition(300.f, 400.f);
    startButton.setFillColor(sf::Color(100, 200, 100));

    sf::Text startText("Start Game", font, 24);
    startText.setPosition(320.f, 410.f);

    bool running = true;
    while (running && window.isOpen()) 
    {
        if (selector.wait(sf::milliseconds(100))) 
        {
            if (selector.isReady(listener)) 
            {
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) 
                {
                    clients.push_back(client);
                    selector.add(*client);
                    std::cout << "New player joined! Total: " << clients.size() << std::endl;
                }
                else 
                {
                    delete client;
                }
            }
        }

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
                running = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) 
            {
                running = false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) 
                {
                    std::cout << "Starting game!" << std::endl;

                    // Send "START" message to all connected clients
                    sf::Packet packet;
                    packet << std::string("START");
                    for (auto& c : clients) 
                    {
                        if (c->send(packet) != sf::Socket::Done) 
                        {
                            std::cerr << "Failed to send START to a client." << std::endl;
                        }
                    }

                    running = false;
                }
            }
        }

        // Draw lobby UI
        window.clear(sf::Color(50, 50, 50));

        sf::Text title("Lobby - Waiting for players", font, 24);
        title.setPosition(100.f, 50.f);
        window.draw(title);

        for (size_t i = 0; i < clients.size(); ++i) 
        {
            sf::Text playerText("Player " + std::to_string(i+1), font, 20);
            playerText.setPosition(120.f, 100.f + i * 30.f);
            window.draw(playerText);
        }

        window.draw(startButton);
        window.draw(startText);

        window.display();
    }

    // Optional: clean up clients when done
    // for (auto c : clients) { delete c; }
    // clients.clear();
}


void MultiplayerHost::startGame()   // Send "start" message to all clients
{
    for (auto& client : clients) 
    {
        sf::Packet packet;
        packet << "START";
        client->send(packet);
    }
}