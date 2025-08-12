#include "multiplayerclient.h"
#include <SFML/Graphics.hpp>
#include <iostream>

std::pair<std::string, std::string> MultiplayerClient::askForIp(sf::RenderWindow& window) 
{
    sf::Font font; 
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    std::string ipInput, nameInput;
    bool enteringIP = true;
    bool done = false;

    sf::Text promptIP("Enter server IP", font, 24);
    promptIP.setPosition(100.f, 100.f); 
    sf::Text IPText("", font, 24);
    IPText.setPosition(100.f, 150.f);

    sf::Text promptName("Enter Username", font, 24);
    promptName.setPosition(100.f, 200.f); 
    sf::Text nameText("", font, 24);
    nameText.setPosition(100.f, 230.f);

    sf::Text instruction("Press TAB to switch field, ENTER to confirm", font, 16);
    instruction.setPosition(100.f, 300.f);

    while (!done && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                done = true;
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (enteringIP)
                {
                    if (event.text.unicode == '\b' && !ipInput.empty()) ipInput.pop_back();
                    else if (event.text.unicode < 128 && std::isprint(event.text.unicode)) ipInput += static_cast<char>(event.text.unicode);
                }
                else
                {
                    if (event.text.unicode == '\b' && !nameInput.empty()) nameInput.pop_back();
                    else if (event.text.unicode < 128 && std::isprint(event.text.unicode)) nameInput += static_cast<char>(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Tab) enteringIP = !enteringIP;
                else if (event.key.code == sf::Keyboard::Enter) done = true;
            }
        }

        IPText.setString(ipInput);
        nameText.setString(nameInput);

        // Highlight the active field
        IPText.setFillColor(enteringIP ? sf::Color::Yellow : sf::Color::White);
        nameText.setFillColor(!enteringIP ? sf::Color::Yellow : sf::Color::White);

        window.clear(sf::Color(50, 50, 50));
        window.draw(promptIP);
        window.draw(IPText);
        window.draw(promptName);
        window.draw(nameText);
        window.draw(instruction);
        window.display();
    }

    playerName = nameInput;
    return {ipInput, nameInput};
}

bool MultiplayerClient::connectToServer(const std::string& ip) 
{
    std::cout << " Connecting " << std::endl;
    if (socket.connect(ip, port) != sf::Socket::Done) {
        std::cerr << "Failed to connect!" << std::endl;
    }
    
    sf::Packet namePacket;
    namePacket << std::string("NAME") << playerName;

    if (socket.send(namePacket) != sf::Socket::Done)
    {
        std::cerr << "Failed to send name!" << std::endl;
        return false;
    }
    
    socket.setBlocking(false);
    return true;
}

void MultiplayerClient::waitForHostToStart(sf::RenderWindow& window) 
{
    Popup popup;
    float deltaTime = popup.getClock().restart().asSeconds();
    popup.update(deltaTime);

    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF")) 
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    sf::RectangleShape leaveButton(sf::Vector2f(150.f, 40.f));
    leaveButton.setPosition(300.f, 400.f);
    leaveButton.setFillColor(sf::Color(200,100,100));

    sf::Text leaveText("Leave Lobby", font, 20);
    leaveText.setPosition(310.f, 410.f);

    bool inLobby = true;
    while (inLobby && window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                sf::Packet packet;
                packet << std::string("LEAVE");
                socket.send(packet);
                window.close();
                inLobby = false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (leaveButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) 
                {
                    std::cout << "CLICKED" << std::endl;
                    sf::Packet packet;
                    packet << std::string("LEAVE");
                    socket.send(packet);
                    inLobby = false;
                }
            }
        }

        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done) 
        {
            std::string msg;
            packet >> msg;
            if (msg == "START") 
            {
                inLobby = false;
            }
            else if (msg == "PLAYERS")
            {
                sf::Uint32 cnt;
                packet >> cnt;
                playerList.clear();

                for (sf::Uint32 i = 0; i < cnt; i++)
                {
                    std::string name;
                    packet >> name;
                    playerList.push_back(name);
                }
            }
            else if (msg == "NAME_TAKEN")
            {
                std::cout << "Name already taken!" << std::endl;
                // popup.show("Name already taken!", 3.f);
                inLobby = false;
            }
            else if (msg == "CANCEL")
            {
                std::cout << "Host canceled lobby. Returning to main menu" << std::endl;
                inLobby = false;
            }
        }

        window.clear(sf::Color(50, 50, 50));
        window.draw(leaveButton);
        window.draw(leaveText);
        popup.draw(window);

        sf::Text inLobbyText("Waiting for host to start...", font, 24);
        inLobbyText.setPosition(100.f, 100.f);
        window.draw(inLobbyText);

        for (size_t i = 0; i < playerList.size(); i++)
        {
            sf::Text playerText(playerList[i], font, 20);
            playerText.setPosition(120.f, 150.f + i * 30.f);
            window.draw(playerText);
        }

        window.display();
    }
}

void MultiplayerClient::gameLoop(sf::RenderWindow& window)
{
    sf::Clock clock;
    bool running = true;
    std::map<std::string, RemotePlayer> remotePlayers;

    std::vector<Entity*> entities;
    player = new Player(100, 100);
    PlayerState myState = getPlayerState(player, playerName);

    std::vector<Structure*> structures = 
    {
        new Platform(0.f, 550.f, 800.f, 50.f),     // Ground
        new Platform(300.f, 400.f, 200.f, 20.f),   // Second floor
        new Platform(100.f, 300.f, 100.f, 20.f),   // Third floor

        new Wall(300.f, 450.f, 40.f, 100.f),
        new Wall(500.f, 200.f, 40.f, 200.f)
    };

    socket.setBlocking(false);
    while (running && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) running = false;
        }

        myState = getPlayerState(player, playerName);
        float dt = clock.restart().asSeconds();

        sf::Packet packet;
        packet << std::string("STATE") << playerName << myState;
        if (socket.send(packet) != sf::Socket::Done) 
        {
            std::cerr << "gameLoop: Send failed. Disconnecting.\n";
            running = false;
            break;
        }

        sf::Packet recv;
        sf::Socket::Status status = socket.receive(recv);
        if (status == sf::Socket::Done)
        {
            std::string msg;
            recv >> msg;

            if (msg == "ALL_STATE")
            {
                sf::Uint32 cnt;
                recv >> cnt;

                for (sf::Uint32 i = 0; i < cnt; i++)
                {
                    std::string name;
                    PlayerState ps;
                    recv >> name >> ps;
                    
                    remotePlayers[name].applyState(ps);
                    
                }
            }
        }
        else if (status == sf::Socket::Disconnected)
        {
            std::cerr << "Disconnected from host. \n";
            running = false;
            break;
        }

        window.clear(sf::Color::Black);

        if (window.hasFocus()) player->handleInput(entities, window);
        player->update(dt, structures, entities, window);
        player->draw(window);

        for (const auto& s : structures) s->draw(window);

        for (auto& [name, rp] : remotePlayers) 
        {
            if (name == playerName) continue;
            rp.update(dt);
            rp.draw(window);
        }
        // std::cout << "Host HP: " << myState.health << std::endl;
        window.display();
        sf::sleep(sf::milliseconds(5));
    }
}

std::string MultiplayerClient::getName() const
{
    return playerName;
}
void MultiplayerClient::setName(const std::string name)
{
    playerName = name;
}