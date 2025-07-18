#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include <SFML/Graphics.hpp>

enum class MultiplayerOption { MAKE_ROOM, FIND_ROOM, BACK };

class MultiplayerMenu 
{
public:
    
    MultiplayerOption show(sf::RenderWindow& window);
};

#endif