#ifndef MULTIPLAYERCLIENT_H
#define MULTIPLAYERCLIENT_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class MultiplayerClient 
{
public:
        
    std::string askForIp(sf::RenderWindow& window);
    void connectToServer(const std::string& ip);
    void waitForHostToStart(sf::RenderWindow& window);

private:
    
    sf::TcpSocket socket;
    unsigned short port = 54000;
};

#endif