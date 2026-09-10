#include "player.h"
#include <iostream>

using namespace std;

player::player()
{
    body.setSize({50.f, 50.f});
    body.setPosition({500.f, 350.f});

    floor = sf::FloatRect({0.f, 460.f}, {1920.f, 100.f});
}

void player::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

void player::move()
{
    sf::Vector2f oldPosition = body.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        body.move({0.f, -50.f});
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        body.move({0.f, 50.f});
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        body.move({50.f, 0.f});
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        body.move({-50.f, 0.f});
    }
    
    auto intersection = body.getGlobalBounds().findIntersection(floor);

    if (intersection)
    {
        if (body.getPosition().y > oldPosition.y)
        {
            body.setPosition({body.getPosition().x, oldPosition.y});
        }
        
    } 
    
}