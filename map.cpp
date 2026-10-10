#include "map.h"
#include "inventory.h"
#include "FastNoiseLite.h"

#include <fstream>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;

map::map() : skysprite(sky), tileSprite(tilesetTexture)
{

}

void map::loadMap()
{
    if (!sky.loadFromFile("assets/world/sky.png"))
    {
        cout << "Nie udalo sie wczytac pliku sky!" << endl;
    }
    sky.setSmooth(false);
    sky.setRepeated(true); 
    skysprite.setTexture(sky);
    skysprite.setPosition({0.f, 0.f});

    if (!tilesetTexture.loadFromFile("assets/world/Terrain.png"))
    {
        cout << "Nie udalo sie wczytac tekstur mapy Terrain.png!" << endl;
    }
    tilesetTexture.setSmooth(false);
    tileSprite.setTexture(tilesetTexture);

    ifstream mapFile("map.txt");
    string line;
    if (mapFile)
    {
        mapData.clear();
        while (getline(mapFile, line))
        {
            vector<int> row;
            for (char c : line)
            {
                row.push_back(c - '0');
            }
            mapData.push_back(row);
        }
    }
}

sf::IntRect map::getTileRect(int tileType)
{
    switch (tileType)
    {
        case 1:  return sf::IntRect({16, 0}, {16, 16});
        case 2:  return sf::IntRect({16, 16}, {16, 16});
        case 3:  return sf::IntRect({0, 16}, {16, 16});
        case 4:  return sf::IntRect({32, 16}, {16, 16});
        case 5:  return sf::IntRect({0, 0}, {16, 16});
        case 6:  return sf::IntRect({32, 0}, {16, 16});

        case 7:  return sf::IntRect({64, 0}, {16, 16});

        case 10: return sf::IntRect({0, 32}, {16, 16});
        case 11: return sf::IntRect({16, 32}, {16, 16});
        case 12: return sf::IntRect({32, 32}, {16, 16});

        case 8:  return sf::IntRect({0, 48}, {16, 16});
        case 9:  return sf::IntRect({16, 48}, {16, 16});

        default: return sf::IntRect({0, 0}, {0, 0});
    }
}

void map::draw(sf::RenderWindow& window)
{
    sf::Vector2f mapSize = getMapSize();
    if (mapSize.x > 0 && mapSize.y > 0)
    {
        skysprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(mapSize.x), static_cast<int>(mapSize.y)}));
    }
    window.draw(skysprite);

    for (size_t y = 0; y < mapData.size(); y++)
    {
        for (size_t x = 0; x < mapData[y].size(); x++)
        {
            int tileType = mapData[y][x];

            if (tileType == 0) continue;

            tileSprite.setPosition({x * tileSize, y * tileSize + mapOffsetY});
            tileSprite.setTextureRect(getTileRect(tileType));

            window.draw(tileSprite);
        }
    }

    for (const auto& drop : droppedItems)
    {
        window.draw(drop.item);
    }
}

bool map::isSolid(float x, float y)
{
    int tileX = static_cast<int>(x / tileSize); 
    int tileY = static_cast<int>((y - mapOffsetY) / tileSize);

    if (tileY < 0 || tileY >= static_cast<int>(mapData.size()))
    {
        return false;
    }

    if (tileX < 0 || tileX >= static_cast<int>(mapData[tileY].size()))
    {
        return false;
    }
    
    return mapData[tileY][tileX] != 0;
}

float map::getGroundY(float x, float y)
{
    int tileY = static_cast<int>((y - mapOffsetY) / tileSize);
    return tileY * tileSize + mapOffsetY;
}

float map::getSurfaceY(float worldX)
{
    int tileX = static_cast<int>(worldX / tileSize);

    if (tileX >= 0 && !mapData.empty() && tileX < static_cast<int>(mapData[0].size()))
    {
        for (size_t y = 0; y < mapData.size(); y++)
        {
            if (mapData[y][tileX] != 0) 
            {
                return y * tileSize + mapOffsetY;
            }
        }
    }
    return 0.f;
}

sf::Vector2f map::getMapSize()
{
    if (mapData.empty()) return {0.f, 0.f};
    
    float width = mapData[0].size() * tileSize;
    float height = mapData.size() * tileSize + mapOffsetY;

    return {width, height};
}

bool map::destroyTile(float worldX, float worldY)
{
    int tileX = static_cast<int>(worldX / tileSize);
    int tileY = static_cast<int>((worldY - mapOffsetY) / tileSize);

    if (tileY >= 0 && tileY < static_cast<int>(mapData.size()))
    {
        if (tileX >= 0 && tileX < static_cast<int>(mapData[tileY].size()))
        {
            if (mapData[tileY][tileX] != 0)
            {
                int tileType = mapData[tileY][tileX];

                if (tileType == 1 || tileType == 3 || tileType == 4 || tileType == 5 || tileType == 6)
                {
                    tileType = 2;
                }

                int itemID = 100 + tileType;

                float dropX = tileX * tileSize + 4.f;
                float dropY = tileY * tileSize + mapOffsetY + 4.f;

                droppedItem newItem;
                newItem.position = {dropX, dropY};
                newItem.itemID = itemID;

                newItem.item.setPosition(newItem.position);
                newItem.item.setSize({8.f, 8.f});
                newItem.item.setFillColor(sf::Color::White);
                newItem.item.setTexture(&tilesetTexture);
                newItem.item.setTextureRect(getTileRect(tileType));

                droppedItems.push_back(newItem);

                mapData[tileY][tileX] = 0;
                return true;
            }
        }
    }
    return false;
}

void map::checkItemPickup(sf::Vector2f playerPos, Inventory& inventory)
{
    float pickUpRange = 25.f;

    for (auto it = droppedItems.begin(); it != droppedItems.end(); )
    {
        sf::Vector2f diff = it->position - playerPos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (distance <= pickUpRange)
        {
            if (inventory.addItem(it->itemID, 1))
            {
                it = droppedItems.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void map::generateMap(int width, int height, int seed)
{
    mapData = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));

    FastNoiseLite surfaceNoise;
    surfaceNoise.SetSeed(seed);
    surfaceNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    surfaceNoise.SetFrequency(0.015f);

    FastNoiseLite caveNoise;
    caveNoise.SetSeed(seed + 1);
    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveNoise.SetFrequency(0.03f);

    int baseSurfaceY = 20;
    int maxHillHeight = 12;

    for (int x = 0; x < width; x++)
    {
        float noiseVal = surfaceNoise.GetNoise(static_cast<float>(x), 0.f);
        int surfaceY = baseSurfaceY + static_cast<int>(noiseVal * maxHillHeight);

        for (int y = 0; y < height; y++)
        {
            if (y < surfaceY)
            {
                mapData[y][x] = 0;
            }
            else if (y < surfaceY + 5)
            {
                mapData[y][x] = 2; 
            }
            else
            {
                int randVal = rand() % 100;

                if (randVal < 4)       mapData[y][x] = 11;
                else if (randVal < 8)  mapData[y][x] = 8;
                else if (randVal < 10) mapData[y][x] = 10;
                else if (randVal < 11 && y > surfaceY + 25) mapData[y][x] = 12;
                else                   mapData[y][x] = 9;
            }

            if (y > surfaceY + 5)
            {
                float caveVal = caveNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
                if (caveVal > 0.52f) 
                {
                    mapData[y][x] = 0;
                }
            }
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (mapData[y][x] == 2) 
            {
                bool topIsAir   = (y > 0 && mapData[y - 1][x] == 0);
                bool leftIsAir  = (x > 0 && mapData[y][x - 1] == 0);
                bool rightIsAir = (x < width - 1 && mapData[y][x + 1] == 0);

                if (topIsAir)
                {
                    if (leftIsAir)       mapData[y][x] = 5; 
                    else if (rightIsAir) mapData[y][x] = 6; 
                    else                 mapData[y][x] = 1; 
                }
                else if (leftIsAir)  mapData[y][x] = 3; 
                else if (rightIsAir) mapData[y][x] = 4; 
            }
        }
    }
}