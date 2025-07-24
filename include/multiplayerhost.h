#ifndef MULTIPLAYERHOST_H
#define MULTIPLAYERHOST_H

#include "popup.h"
#include "playerstate.h"

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MultiplayerHost 
{
public:
    void runLobby(sf::RenderWindow& window);
    void startGame();
    void runGameLoop(sf::RenderWindow& window);

private:
    
    sf::TcpListener listener;
    std::vector<sf::TcpSocket*> clients;
    unsigned short port = 54000;

    std::vector<std::string> playerList; 
};

#endif
