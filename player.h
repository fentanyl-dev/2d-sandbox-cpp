#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"

class player
{
    private:
        sf::RectangleShape body;

        bool onGround;
        float velocityY;
        
        map* worldMap;

    public:
        player(map* worldMap);
        void move();
        void draw(sf::RenderWindow& window);
        void jump();
        void gravity();
};