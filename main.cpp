#include <SFML/Graphics.hpp>
#include "map.h"
#include "player.h"
#include <iostream>

using namespace std;

int main() 
{
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "RPG GAME");

    window.setFramerateLimit(60);

    sf::View gameView(sf::FloatRect({0.f, 0.f}, {480.f, 270.f}));

    map myMap;
    myMap.loadMap();

    cout << "Map width: " << myMap.getMapSize().x << endl;
    cout << "Map height: " << myMap.getMapSize().y << endl;

    player Player(&myMap);

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    Player.attack();
                }
                
            }
            
            
        }
        Player.move();
        Player.jump();
        Player.gravity();

        sf::Vector2f cameraPos = Player.getPosition();

        float halfWidth = gameView.getSize().x / 2.f;
        float halfHeight = gameView.getSize().y / 2.f;

        sf::Vector2f mapSize = myMap.getMapSize();

        if (cameraPos.x < halfWidth)
        {
            cameraPos.x = halfWidth;
        }

        if (cameraPos.x > mapSize.x - halfWidth)
        {
            cameraPos.x = mapSize.x - halfWidth;
        }

        if (cameraPos.y < halfHeight)
        {
            cameraPos.y = halfHeight;
        }

        if (cameraPos.y > mapSize.y - halfHeight)
        {
            cameraPos.y = mapSize.y - halfHeight;
        }

        gameView.setCenter(cameraPos);

        window.clear();

        window.setView(gameView);

        myMap.draw(window);
        Player.draw(window);

        window.display();
    }
    
}