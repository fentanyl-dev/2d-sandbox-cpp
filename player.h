#pragma once

#include <SFML/Graphics.hpp>

class player
{
    private:
        sf::RectangleShape body;
        sf::FloatRect floor;

    public:
        player();
        void move();
        void draw(sf::RenderWindow& window);
};