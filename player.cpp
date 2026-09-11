#include "player.h"
#include <iostream>

using namespace std;

player::player(map* worldMap)
{
    this->worldMap = worldMap;

    body.setSize({50.f, 50.f});
    body.setPosition({500.f, 350.f});

     onGround = true;
     velocityY = 0.f;
}

void player::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

void player::move()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        body.move({50.f, 0.f});
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        body.move({-50.f, 0.f});
    }
    
}

void player::jump()
{

    if (onGround)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            velocityY = -10.f;
            onGround = false;
        }
    }
    
}

void player::gravity()
{
    velocityY += 0.5f;

    body.move({0.f, velocityY});

    float playerBottom = body.getPosition().y + body.getSize().y;

    float playerCenterX =
        body.getPosition().x + body.getSize().x / 2.f;

    if (velocityY > 0 &&
        worldMap->isSolid(playerCenterX, playerBottom))
    {
        float groundY =
            worldMap->getGroundY(playerCenterX, playerBottom);

        body.setPosition({
            body.getPosition().x,
            groundY - body.getSize().y
        });

        velocityY = 0.f;
        onGround = true;
    }
}