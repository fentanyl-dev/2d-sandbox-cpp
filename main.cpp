#include <SFML/Graphics.hpp>
#include "map.h"
#include "player.h"

using namespace std;

int main() 
{
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "RPG GAME");

    window.setFramerateLimit(60);

    sf::View gameView(sf::FloatRect({0.f, 0.f}, {480.f, 270.f}));

    map myMap;
    myMap.loadMap();

    player Player(&myMap);

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            
        }
        Player.move();
        Player.jump();
        Player.gravity();

        window.clear();

        window.setView(gameView);

        myMap.draw(window);
        Player.draw(window);

        window.display();
    }
    
}