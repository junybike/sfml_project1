#include "multiplayerclient.h"
#include <SFML/Graphics.hpp>
#include <iostream>

std::string MultiplayerClient::askForIp(sf::RenderWindow& window) 
{
    sf::Font font; 
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    std::string ipInput;
    sf::Text prompt("Enter server IP", font, 24);
    prompt.setPosition(100.f, 100.f);

    sf::Text inputText("", font, 24);
    inputText.setPosition(100.f, 150.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return "";
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b')
                {
                    if (!ipInput.empty()) ipInput.pop_back();
                }
                else if (event.text.unicode == '\r' || event.text.unicode == '\n')
                {
                    return ipInput;
                }
                else if (event.text.unicode < 128) 
                {
                    ipInput += static_cast<char>(event.text.unicode);
                }
            }
        }

        inputText.setString(ipInput);

        window.clear(sf::Color(50, 50, 50));
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }

    return "";
}

void MultiplayerClient::connectToServer(const std::string& ip) 
{
    if (socket.connect(ip, port) != sf::Socket::Done) {
        // Handle connection error
    }
}

void MultiplayerClient::waitForHostToStart(sf::RenderWindow& window) 
{
    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF")) 
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    bool waiting = true;
    while (waiting) 
    {
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done) 
        {
            std::string msg;
            packet >> msg;
            if (msg == "START") 
            {
                waiting = false;
            }
        }

        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
                waiting = false;
            }
        }

        window.clear(sf::Color(50, 50, 50));

        sf::Text waitingText("Waiting for host to start...", font, 24);
        waitingText.setPosition(100.f, 100.f);
        window.draw(waitingText);

        window.display();
    }
}
