#include <SFML/Graphics.hpp>
#include "map.h"
#include "player.h"
#include "inventory.h"
#include "decoration.h"

#include <vector>
#include <iostream>
#include <cmath>

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

    Inventory inventory;

     sf::Texture decorTexture;
    if (!decorTexture.loadFromFile("assets/decoration/WoodTileset.png"))
    {
        cout << "Nie udalo sie wczytac tekstur dekoracji" << endl;
    }
    
    float groundY = 198.f;

    std::vector<Decoration> decorations;
    decorations.emplace_back(decorTexture, DecorationType::treeLarge, sf::Vector2f(100.f, groundY));
    decorations.emplace_back(decorTexture, DecorationType::singpost, sf::Vector2f(270.f, groundY));
    decorations.emplace_back(decorTexture, DecorationType::barrel, sf::Vector2f(400.f, groundY));
    decorations.emplace_back(decorTexture, DecorationType::crate, sf::Vector2f(430.f, groundY));
    decorations.emplace_back(decorTexture, DecorationType::treeMedium, sf::Vector2f(480.f, groundY));

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
                const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>();

                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    Player.attack();
                }

                if (mousePress->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos, gameView);

                    float maxReach = 64.f;
                    sf::Vector2f playerPos = Player.getPosition();

                    sf::Vector2f diff = worldPos - playerPos;

                    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                    if (distance <= maxReach)
                    {
                        myMap.destroyTile(worldPos.x, worldPos.y);
                    }
                    
                }
            }
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::I)
                {
                    inventory.toggle();
                }

                if (keyPressed->code == sf::Keyboard::Key::Num1) inventory.selectSlot(0);
                if (keyPressed->code == sf::Keyboard::Key::Num2) inventory.selectSlot(1);
                if (keyPressed->code == sf::Keyboard::Key::Num3) inventory.selectSlot(2);
                if (keyPressed->code == sf::Keyboard::Key::Num4) inventory.selectSlot(3);
                if (keyPressed->code == sf::Keyboard::Key::Num5) inventory.selectSlot(4);
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

        for (const auto& decor : decorations)
        {
            decor.draw(window);
        }
        
        Player.draw(window);

        window.setView(window.getDefaultView());
        inventory.draw(window);

        window.display();
    }
    
}