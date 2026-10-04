#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

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
    std::vector<std::string> mapData;

    float tileSize = 16.f;
    float mapOffsetY = 150.f;

    sf::Texture sky;
    sf::Sprite skysprite;

    sf::Texture tilesetTexture;
    sf::Sprite tileSprite;

    sf::IntRect getTileRect(char tileType);

    std::vector<droppedItem> droppedItems;

public:
    map();

    void loadMap();

    void draw(sf::RenderWindow& window);

    sf::Vector2f getMapSize();
    bool isSolid(float x, float y);
    float getGroundY(float x, float y);

    bool destroyTile(float worldX, float worldY);

    void checkItemPickup(sf::Vector2f playerPos, Inventory& inventory);
};