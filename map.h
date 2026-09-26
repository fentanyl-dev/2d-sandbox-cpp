#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class map
{
private:
    std::vector<std::string> mapData;
     float tileSize = 16.f;
     float mapOffsetY = 150.f;

     sf::Texture sky;
     sf::Sprite skysprite;

    sf::Texture tilesetTexture;
    sf::Sprite tileSprite;

    sf::IntRect getTileRect(char tileType);

public:
    map();

    sf::Vector2f getMapSize();

    void loadMap();
    void draw(sf::RenderWindow& window);
    bool isSolid(float x, float y);
    float getGroundY(float x, float y);

    bool destroyTile(float worldX, float worldY);
};