#include "multiplayermenu.h"

#include <iostream>

MultiplayerOption MultiplayerMenu::show(sf::RenderWindow& window) 
{
    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "multiplayermenu.cpp: Failed to load font" << std::endl;
    }

    // Define buttons as rectangles + text
    sf::RectangleShape makeRoomBtn(sf::Vector2f(200.f, 50.f));
    makeRoomBtn.setPosition(300.f, 200.f);
    makeRoomBtn.setFillColor(sf::Color(100, 100, 200));

    sf::Text makeRoomText("Make Room", font, 20);
    makeRoomText.setPosition(320.f, 210.f);

    sf::RectangleShape findRoomBtn(sf::Vector2f(200.f, 50.f));
    findRoomBtn.setPosition(300.f, 300.f);
    findRoomBtn.setFillColor(sf::Color(100, 200, 100));

    sf::Text findRoomText("Find Room", font, 20);
    findRoomText.setPosition(320.f, 310.f);

    sf::RectangleShape backBtn(sf::Vector2f(200.f, 50.f));
    backBtn.setPosition(300.f, 400.f);
    backBtn.setFillColor(sf::Color(200, 100, 100));

    sf::Text backText("Back", font, 20);
    backText.setPosition(370.f, 410.f);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return MultiplayerOption::BACK;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (makeRoomBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) return MultiplayerOption::MAKE_ROOM;
                else if (findRoomBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) return MultiplayerOption::FIND_ROOM;
                else if (backBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) return MultiplayerOption::BACK;
            }
        }
        // Draw
        window.clear(sf::Color(50, 50, 50));
        window.draw(makeRoomBtn);
        window.draw(makeRoomText);
        window.draw(findRoomBtn);
        window.draw(findRoomText);
        window.draw(backBtn);
        window.draw(backText);
        window.display();
    }
    return MultiplayerOption::BACK;
}
