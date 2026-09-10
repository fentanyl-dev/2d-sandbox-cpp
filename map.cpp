#include "map.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

void map::loadMap()
{
    ifstream mapFile("map.txt");
    string line;

    if (!sky.loadFromFile("sky.png"))
    {
        cout << "Nie udalo sie wczytac pliku sky" << endl;
    }
    
    if (!grass.loadFromFile("grass.png"))
    {
        cout << "Nie udalo sie wczytac pliku grass";
    }
    
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
    
}

void map::draw(sf::RenderWindow& window)
{

    sf::Sprite skySprite(sky);

    skySprite.setScale({3.3333f, 3.3333f});

    window.draw(skySprite);

    sf::Sprite grassSprite(grass);    

    grassSprite.setScale({3.125f, 3.125f});

    for (int y = 0; y < mapData.size(); y++)
    {
        for (int x = 0; x < mapData[y].size(); x++)
        {
            sf::RectangleShape tile;
            tile.setSize({tileSize, tileSize});
            tile.setPosition({x * tileSize, y * tileSize});

            // Powietrze

            if (mapData[y][x] == '0')
            {
                continue;
            }

            //Trawa

            else if (mapData[y][x] == '1')
            {
                grassSprite.setPosition({x * tileSize, y * tileSize});
                window.draw(grassSprite);
                continue;
            }

            // Ziemia

            else if (mapData[y][x] == '2')
            {
                tile.setFillColor(sf::Color(150, 75, 0));
            }

            //Kamienie

            else if (mapData[y][x] == '3')
            {
                tile.setFillColor(sf::Color(77, 56, 51));
            }

            window.draw(tile);
        }
        
    }
    
}