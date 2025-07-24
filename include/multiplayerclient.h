#ifndef MULTIPLAYERCLIENT_H
#define MULTIPLAYERCLIENT_H

#include "popup.h"
#include "playerstate.h"
#include "player.h"

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class MultiplayerClient 
{
public:
        
    std::pair<std::string, std::string> askForIp(sf::RenderWindow& window);
    bool connectToServer(const std::string& ip);
    void waitForHostToStart(sf::RenderWindow& window);
    void gameLoop(sf::RenderWindow& window);

    std::string getName() const;
    void setName(const std::string name);

private:
    
    sf::TcpSocket socket;
    unsigned short port = 54000;

    Player* player = nullptr;
    std::string playerName;
    std::vector<std::string> playerList;
};

#endif