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
    float playerCenterY = body.getPosition().y + body.getSize().y / 2.f;

    float playerRight = body.getPosition().x + body.getSize().x;

    float playerLeft = body.getPosition().x - 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        if (!worldMap->isSolid(playerRight,playerCenterY))
        {
            body.move({50.f, 0.f});
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (!worldMap->isSolid(playerLeft, playerCenterY))
        {
            body.move({-50.f, 0.f});
        }
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

    float playerCenterX = body.getPosition().x + body.getSize().x / 2.f;

    float playerBottom = body.getPosition().y + body.getSize().y;

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