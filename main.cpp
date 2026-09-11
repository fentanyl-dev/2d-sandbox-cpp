#include <SFML/Graphics.hpp>
#include "map.h"
#include "player.h"

using namespace std;

int main() 
{
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Terraria");

    window.setFramerateLimit(60);

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

        myMap.draw(window);
        Player.draw(window);

        window.display();
    }
    
}