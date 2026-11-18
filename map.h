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

    unsigned version = 0;   // zwieksza sie przy kazdej zmianie mapy (dla oswietlenia)
    void drawTorch(sf::RenderWindow& window, sf::Vector2f pos);

public:
    static constexpr int TORCH_TILE = 13;
    static bool isSolidTile(int t) { return t != 0 && t != TORCH_TILE; }

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

    int getTile(float worldX, float worldY);
    bool placeTile(float worldX, float worldY, int tileType);
    sf::FloatRect getTileBounds(float worldX, float worldY);


    int getTilesX() const { return mapData.empty() ? 0 : static_cast<int>(mapData[0].size()); }
    int getTilesY() const { return static_cast<int>(mapData.size()); }
    int tileAt(int tx, int ty) const;
    unsigned getVersion() const { return version; }
};