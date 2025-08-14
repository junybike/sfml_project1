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

    // "Cancel" button
    sf::RectangleShape cancelButton(sf::Vector2f(200.f, 50.f));
    cancelButton.setPosition(520.f, 400.f);
    cancelButton.setFillColor(sf::Color(200, 100, 100));

    sf::Text cancelText("Cancel", font, 24);
    cancelText.setPosition(540.f, 410.f);

    bool running = true;
    while (running && window.isOpen()) 
    {
        if (selector.wait(sf::milliseconds(1000))) 
        {
            if (selector.isReady(listener)) // player joining
            {
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) 
                {
                    clients.push_back(client);
                    selector.add(*client);
                }
                else 
                {
                    delete client;
                }
            }
            else    // handle players
            {
                for (size_t i = 0; i < clients.size(); i++)
                {
                    auto& c = clients[i];
                    if (selector.isReady(*c))
                    {
                        sf::Packet packet;
                        if (c->receive(packet) == sf::Socket::Done)
                        {
                            std::string msg;
                            packet >> msg;

                            if (msg == "LEAVE") // a player has left the lobby
                            {
                                selector.remove(*c);
                                delete c;
                                
                                clients.erase(clients.begin() + i);
                                playerList.erase(playerList.begin() + i);
                                i--;

                                sf::Packet listPacket;
                                listPacket << std::string("PLAYERS") << static_cast<sf::Uint32>(playerList.size());

                                for (const auto& name : playerList) listPacket << name;
                                for (const auto& client : clients) client->send(listPacket);
                                continue;
                            }
                            else if (msg == "NAME") // a player joined and receives its name
                            {
                                std:: string name;
                                packet >> name;
                                bool duplicate = false;

                                for (const auto& n : playerList)
                                {
                                    if (n == name)
                                    {
                                        duplicate = true;
                                        break;
                                    }
                                }
                                if (duplicate)
                                {
                                    sf::Packet reject;
                                    reject << std::string("NAME_TAKEN");
                                    c->send(reject);

                                    selector.remove(*c);
                                    delete c;
                                    clients.erase(clients.begin() + i);
                                    i--;
                                }
                                else
                                {
                                    playerList.push_back(name);
                                    std::cout << name << " joined! Total: " << playerList.size() << std::endl;

                                    sf::Packet listPacket;
                                    listPacket << std::string("PLAYERS") << static_cast<sf::Uint32>(playerList.size());
                                    
                                    for (const auto& n : playerList) listPacket << n;
                                    for (const auto& c : clients) c->send(listPacket);
                                }
                            }
                        }
                        else // a player disconnected unexpectedly
                        {
                            selector.remove(*c);
                            delete c;

                            clients.erase(clients.begin() + i);
                            playerList.erase(playerList.begin() + i);
                            --i;

                            sf::Packet listPacket;
                            listPacket << std::string("PLAYERS") << static_cast<sf::Uint32>(playerList.size());

                            for (const auto& name : playerList) listPacket << name;                        
                            for (const auto& other : clients) other->send(listPacket);
                            continue;
                        }
                    }
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
                    sf::Packet packet;
                    packet << std::string("START");
                    for (auto& c : clients) 
                    {
                        if (c->send(packet) != sf::Socket::Done) 
                        {
                            std::cerr << "Failed to send START to a client." << std::endl;
                        }
                    }
                    runGameLoop(window);
                    running = false;
                }
                if (cancelButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    std::cout << "Cancelling lobby" << std::endl;
                    sf::Packet packet;
                    packet << std::string("CANCEL");
                    for (auto& c : clients)
                    {
                        c->send(packet);
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

        for (size_t i = 0; i < playerList.size(); ++i) 
        {
            sf::Text playerText(playerList[i], font, 20);
            playerText.setPosition(120.f, 100.f + i * 30.f);
            window.draw(playerText);
        }

        window.draw(startButton);
        window.draw(startText);
        window.draw(cancelButton);
        window.draw(cancelText);

        window.display();
    }

    // Clean up clients when done
    for (auto c : clients) { delete c; }
    clients.clear();
    listener.close();
}

void MultiplayerHost::runGameLoop(sf::RenderWindow& window)
{
    sf::Clock clock;
    bool running = true;
    std::map<std::string, RemotePlayer> remotePlayers;

    std::vector<Entity*> entities;
    player = new Player(100, 100);
    
    PlayerState hostState;
    hostState = getPlayerState(player, "host");
    

    std::vector<Structure*> structures = 
    {
        new Platform(0.f, 550.f, 800.f, 50.f),     // Ground
        new Platform(300.f, 400.f, 200.f, 20.f),   // Second floor
        new Platform(100.f, 300.f, 100.f, 20.f),   // Third floor

        new Wall(300.f, 450.f, 40.f, 100.f),
        new Wall(500.f, 200.f, 40.f, 200.f)
    };

    sf::SocketSelector selector;
    for (auto& c : clients)
    {
        c->setBlocking(false);
        selector.add(*c);
    }

    while (running && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) running = false;
        }
        
        float dt = clock.restart().asSeconds();

        if (hostState.damageCooldown > 0.f) 
        {
            hostState.damageCooldown -= dt;
            if (hostState.damageCooldown < 0.f) hostState.damageCooldown = 0.f;
        }

        for (auto& c : clients)
        {
            sf::Packet packet;
            if (c->receive(packet) == sf::Socket::Done)
            {
                std::string msg;
                packet >> msg;

                if (msg == "STATE")
                {
                    std::string name;
                    PlayerState ps;
                    packet >> name >> ps;

                    PlayerState existing = remotePlayers[name].getState();

                    existing.position      = ps.position;      
                    existing.curAnimation  = ps.curAnimation;
                    existing.animationTime = ps.animationTime;
                    existing.isAttacking   = ps.isAttacking;
                    existing.facingRight   = ps.facingRight;
                    existing.canAttack     = ps.canAttack;

                    remotePlayers[name].applyState(existing);
                }
                else if (msg == "LEAVE")
                {
                    // remove this client
                }
            }
        }

        // playerStates[hostName] = getHostState();
        window.clear(sf::Color::Black);

        if (window.hasFocus()) player->handleInput(entities, window);
        player->update(dt, structures, entities, window);
        player->draw(window);

        for (const auto& s : structures) s->draw(window);

        hostState = getPlayerState(player, "host");
        if (hostState.canAttack == false)
        {
            sf::FloatRect hostAtkBox = player->getHitbox();
            for (auto& [victimName, victimRP] : remotePlayers)
            {
                if (victimName == "host") continue;
                sf::FloatRect victimBox = victimRP.getHitbox();
                if (hostAtkBox.intersects(victimBox))
                {
                    PlayerState vs = victimRP.getState();
                    // std::cout << vs.damageCooldown << std::endl;

                    if (vs.damageCooldown > 0.f) continue;
                    vs.health -= 10;
                    vs.damageCooldown = 0.3f;
                    // vs.velocity.x += hostState.facingRight ? 300.f : -300.f;
                    victimRP.applyState(vs);
                }
            }
        }

        // Check if remote players attack host
        for (auto& [attackerName, attackerRP] : remotePlayers)
        {
            // if (attackerName == "host") continue;
            if (attackerRP.getState().canAttack == false)
            {
                // std::cout <<"Client Attacking" << std::endl;
                sf::FloatRect atkBox = attackerRP.getHitbox();
                sf::FloatRect hostBox = player->getHitbox();
                if (atkBox.intersects(hostBox) && hostState.canAttack)
                {
                    if (hostState.damageCooldown > 0.f) continue;
                    hostState.health -= 10;
                    hostState.damageCooldown = 0.3f;

                    std::cout << hostState.health << std::endl;
                    // hostState.velocity.x += hostState.facingRight ? 300.f : -300.f;
                }
            }
        }

        // hostState.position += hostState.velocity * dt;
        // hostState.velocity *= 0.9f;
        if (hostState.damageCooldown > 0.f) 
        {
            hostState.damageCooldown -= dt;
            if (hostState.damageCooldown < 0.f) hostState.damageCooldown = 0.f;
        }
        applyPlayerState(player, hostState);
        

        for (auto& [name, rp] : remotePlayers) 
        {
            if (name == "host") continue;
            PlayerState ps = rp.getState();
            // ps.position += ps.velocity * dt;
            // ps.velocity *= 0.9f;

            if (ps.damageCooldown > 0.f) 
            {
                ps.damageCooldown -= dt;
                if (ps.damageCooldown < 0.f) ps.damageCooldown = 0.f;
            }
            
            rp.applyState(ps);
            rp.update(dt);
            rp.draw(window);
        }

        sf::Packet out;
        out << std::string("ALL_STATE") << static_cast<sf::Uint32>(remotePlayers.size() + 1);
        out << "host" << hostState;
        for (const auto& [name, rp] : remotePlayers)
        {
            out << name << rp.getState();
        }
        for (auto& c : clients) 
        {
            c->send(out);
        }

        window.display();
        sf::sleep(sf::milliseconds(5));
    }

    for (auto& c : clients) delete c;
    clients.clear();
    remotePlayers.clear();
}