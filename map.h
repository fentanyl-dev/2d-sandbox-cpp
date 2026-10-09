#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "FastNoiseLite.h"

//Klasa inventory

class Inventory;

//Struktura od wyrzucania itemow

struct droppedItem
{
    sf::Vector2f position;
    sf::RectangleShape item;
    int itemID = 0;
};

//

class map
{
private:
    std::vector<std::vector<int>> mapData;

    float tileSize = 16.f;
    float mapOffsetY = 0.f;

    sf::Texture sky;
    sf::Sprite skysprite;

    sf::Texture tilesetTexture;
    sf::Sprite tileSprite;

    sf::IntRect getTileRect(int tileType);

    std::vector<droppedItem> droppedItems;

public:
    map();

    void loadMap();

    void generateMap(int width, int height, int seed = 1337);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getMapSize();
    bool isSolid(float x, float y);
    float getGroundY(float x, float y);

    float getSurfaceY(float worldX);

    bool destroyTile(float worldX, float worldY);

    void checkItemPickup(sf::Vector2f playerPos, Inventory& inventory);
};