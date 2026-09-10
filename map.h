#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class map
{
private:
    std::vector<std::string> mapData;
     float tileSize = 50.f;
     float mapOffsetY = 280.f;
     sf::Texture sky;
     sf::Texture grass;
     sf::Texture dirt;
     sf::Texture stone;
public:
    void loadMap();
    void draw(sf::RenderWindow& window);
};