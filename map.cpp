#include "map.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

#include "map.h"
#include <fstream>
#include <iostream>

using namespace std;

map::map() : skysprite(sky), tileSprite(tilesetTexture)
{

}

void map::loadMap()
{
    ifstream mapFile("map.txt");
    string line;
    
    if (!mapFile)
    {
        cout << "Nie udalo sie wczytac pliku" << endl;
    }
    else 
    {
        cout << "Plik wczytano poprawnie" << endl;

        while (getline(mapFile, line))
        {
            mapData.push_back(line);
        }
    }

    if (!sky.loadFromFile("assets/world/sky.png"))
    {
        cout << "Nie udalo sie wczytac pliku sky" << endl;
    }
    sky.setSmooth(false);
    sky.setRepeated(true); 

    skysprite.setTexture(sky);

    float skyWidth = sky.getSize().x;
    float skyHeight = sky.getSize().y;

    skysprite.setTextureRect(sf::IntRect({0, 0}, {2000, static_cast<int>(skyHeight)}));
    
    skysprite.setPosition({0.f, 0.f});
    

    if (!tilesetTexture.loadFromFile("assets/world/Terrain.png"))
    {
        cout << "Nie udalo sie wczytac tekstur mapy" << endl;
    }
    tilesetTexture.setSmooth(false);

    tileSprite.setTexture(tilesetTexture);
    
}

sf::IntRect map::getTileRect(char tileType)
{
    switch (tileType)
    {
        case '1': return sf::IntRect({16, 0}, {16, 16});  // Trawa góra (środek)
        case '2': return sf::IntRect({16, 16}, {16, 16}); // Ziemia (wypełnienie)
        case '3': return sf::IntRect({0, 0}, {16, 16});   // Trawa LEWA krawędź
        case '4': return sf::IntRect({32, 0}, {16, 16});  // Trawa PRAWA krawędź
        case '5': return sf::IntRect({0, 16}, {16, 16});  // Ziemia LEWA ściana
        case '6': return sf::IntRect({32, 16}, {16, 16}); // Ziemia PRAWA ściana
        default:  return sf::IntRect({0, 0}, {0, 0});
    }
}

void map::draw(sf::RenderWindow& window)
{
    window.draw(skysprite);

    for (size_t y = 0; y < mapData.size(); y++)
    {
        for (size_t x = 0; x < mapData[y].size(); x++)
        {
            char tileType = mapData[y][x];

        // POWIETRZE
            if (tileType == '0')
            {
                continue;
            }

            tileSprite.setPosition({x * tileSize, y * tileSize + mapOffsetY});

            tileSprite.setTextureRect(getTileRect(tileType));

            window.draw(tileSprite);
        }
    }
}
    
bool map::isSolid(float x, float y)
{
    int tileX = x / tileSize; 
    int tileY = (y - mapOffsetY) / tileSize;

    if (tileY < 0 || tileY >= mapData.size())
    {
        return false;
    }

    if (tileX < 0 || tileX >= mapData[tileY].size())
    {
        return false;
    }
    
    if (mapData[tileY][tileX] == '0')
    {
        return false;
    }
    return true;
}

float map::getGroundY(float x, float y)
{
    int tileY = (y - mapOffsetY) / tileSize;

    return tileY * tileSize + mapOffsetY;
}

sf::Vector2f map::getMapSize()
{
    float width = mapData[0].size() * tileSize;
    float height= mapData.size() * tileSize + mapOffsetY;

    return {width, height};
}