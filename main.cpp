#include <SFML/Graphics.hpp>
#include "map.h"
#include "player.h"
#include "inventory.h"
#include "decoration.h"
#include "stats.h"
#include "enemy.h"
#include "item.h"
#include "crafting.h"
#include "daynight.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

void drawHUD(sf::RenderWindow& window, const stats& playerStats, 
             const sf::Texture& hFull, const sf::Texture& hHalf, const sf::Texture& hEmpty,
             const sf::Texture& eFull, const sf::Texture& eHalf, const sf::Texture& eEmpty)
{
    sf::Sprite icon(hFull);
    icon.setScale({2.0f, 2.0f});

    float startX = 20.f;
    float startY = 20.f;
    float iconSpacing = 25.f;

    // ZYCIE
    int totalHearts = 10;
    float hpPerHeart = playerStats.maxHealth / totalHearts;

    for (int i = 0; i < totalHearts; i++)
    {
        float heartMin = i * hpPerHeart;
        float heartMax = (i + 1) * hpPerHeart;

        if (playerStats.health >= heartMax)
        {
            icon.setTexture(hFull);
        } 
        else if (playerStats.health <= heartMin)
        {
            icon.setTexture(hEmpty);
        } 
        else 
        {
            icon.setTexture(hHalf);
        }
        
        icon.setPosition({startX + (i * iconSpacing), startY});
        window.draw(icon);
    }

    // MANA
    float energyStartY = startY + 30.f;
    int totalEnergies = 10;
    float energyPerIcon = playerStats.maxMana / totalEnergies;

    for (int i = 0; i < totalEnergies; i++)
    {
        float energyMin = i * energyPerIcon;
        float energyMax = (i + 1) * energyPerIcon;

        if (playerStats.mana >= energyMax)
        {
            icon.setTexture(eFull);
        } 
        else if (playerStats.mana <= energyMin)
        {
            icon.setTexture(eEmpty);
        } 
        else 
        {
            icon.setTexture(eHalf);
        }
        
        icon.setPosition({startX + (i * iconSpacing), energyStartY});
        window.draw(icon);
    }
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "RPG GAME");
    window.setFramerateLimit(60);

    {
    ItemDatabase::innit();

    sf::View gameView(sf::FloatRect({0.f, 0.f}, {480.f, 270.f}));

    map myMap;
    myMap.loadMap();                   
    myMap.generateMap(200, 100, 12345);

    player Player(&myMap);

    float spawnX = 200.f;
    float spawnY = myMap.getSurfaceY(spawnX);
    Player.setPosition({spawnX, spawnY - 32.f});

    std::vector<Enemy> enemies;
    sf::Clock spawnClock;
    float spawnInterval = 4.0f; 
    int maxEnemies = 10;

    
    float enemySpawnX = 260.f;
    float enemySpawnY = myMap.getSurfaceY(enemySpawnX);
    enemies.emplace_back(&myMap, sf::Vector2f(enemySpawnX, enemySpawnY - 16.f));

    Inventory inventory;
    Crafting crafting;

    inventory.addItem(obsidianSword, 1);
    inventory.addItem(oak_plank, 6);
    inventory.addItem(stone, 7);

    stats stats;

    DayNight dayNight;

    sf::Texture heartFullTex, heartHalfTex, heartEmptyTex;
    sf::Texture energyFullTex, energyHalfTex, energyEmptyTex;

    if (!heartFullTex.loadFromFile("assets/hud/heart_full.png") ||
        !heartHalfTex.loadFromFile("assets/hud/heart_half.png") ||
        !heartEmptyTex.loadFromFile("assets/hud/heart_empty.png") ||
        !energyFullTex.loadFromFile("assets/hud/energy_full.png") ||
        !energyHalfTex.loadFromFile("assets/hud/energy_half.png") ||
        !energyEmptyTex.loadFromFile("assets/hud/energy_empty.png"))
    {
        cout << "Nie udalo sie wczytac tekstur HUD!" << endl;
    }

    heartFullTex.setSmooth(false);
    heartHalfTex.setSmooth(false);
    heartEmptyTex.setSmooth(false);
    energyFullTex.setSmooth(false);
    energyHalfTex.setSmooth(false);
    energyEmptyTex.setSmooth(false);

    sf::Texture decorTexture;
    if (!decorTexture.loadFromFile("assets/decoration/WoodTileset.png"))
    {
        cout << "Nie udalo sie wczytac tekstur dekoracji" << endl;
    }
    
    std::vector<Decoration> decorations;

    auto addDecor = [&](DecorationType type, float x)
    {
        decorations.emplace_back(decorTexture, type, sf::Vector2f(x, myMap.getSurfaceY(x)));
    };

    addDecor(DecorationType::treeLarge,  84.f);
    addDecor(DecorationType::singpost,   238.f);
    addDecor(DecorationType::barrel,     400.f);
    addDecor(DecorationType::crate,      430.f);
    addDecor(DecorationType::treeMedium, 496.f);

    sf::Clock frameClock;

    bool running = true;

    while (running && window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();
        stats.update(dt);
        dayNight.update(dt);

        // Spawn zalezny od pory dnia
        spawnInterval = dayNight.isNight() ? 2.0f : 8.0f;
        maxEnemies    = dayNight.isNight() ? 15   : 3;

        if (stats.isDead())
        {
            stats.health = stats.maxHealth;
            stats.mana = stats.maxMana;
            Player.setPosition({spawnX, spawnY - 32.f});
        }

        if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
        {
            if (enemies.size() < (size_t)maxEnemies)
            {
                float randomOffsetX = (rand() % 2 == 0 ? 1.f : -1.f) * (250.f + (rand() % 300));
                float newEnemyX = Player.getPosition().x + randomOffsetX;

                if (newEnemyX > 50.f && newEnemyX < myMap.getMapSize().x - 50.f)
                {
                    float newEnemyY = myMap.getSurfaceY(newEnemyX);
                    enemies.emplace_back(&myMap, sf::Vector2f(newEnemyX, newEnemyY - 16.f));
                }
            }
            spawnClock.restart();
        }

        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                running = false;
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>();

                if (mousePress->button == sf::Mouse::Button::Left)
                {
                    if (inventory.getIsOpen())
                    {
                        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f guiMousePos = window.mapPixelToCoords(mousePixelPos, window.getDefaultView());

                        // najpierw panel craftingu, potem ekwipunek
                        if (!crafting.handleClick(guiMousePos, window, inventory))
                        {
                            inventory.handleMouseClick(guiMousePos, window);
                        }
                    }
                    else
                    {
                        Player.attack();
                        sf::FloatRect attackArea = Player.getAttackBounds();

                        for (auto& enemy : enemies)
                        {
                            if (!enemy.isDead())
                            {
                                if (attackArea.findIntersection(enemy.getBounds()).has_value())
                                {
                                    // Zadajemy obrazenia i przekazujemy pozycje gracza do odrzutu!
                                    enemy.takeDamage(1.0f, Player.getPosition());
                                }
                            }
                        }
                    }
                }

                if (mousePress->button == sf::Mouse::Button::Right)
                {
                    if (inventory.getIsOpen())
                    {
                        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f guiMousePos = window.mapPixelToCoords(mousePixelPos, window.getDefaultView());

                        inventory.handleRightClick(guiMousePos, window);
                    }
                    else
                    {
                        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos, gameView);

                        float maxReach = 64.f;
                        sf::Vector2f playerPos = Player.getPosition();
                        sf::Vector2f diff = worldPos - playerPos;

                        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                        if (distance <= maxReach)
                        {
                            myMap.destroyTile(worldPos.x, worldPos.y);
                        }
                    }
                }
            }
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::I)
                {
                    inventory.toggle();
                }

                if (keyPressed->code == sf::Keyboard::Key::H) stats.takeDamage(10.f);
                if (keyPressed->code == sf::Keyboard::Key::M) stats.useMana(10.f);

                // Test: przewijanie czasu o 0.1 doby
                if (keyPressed->code == sf::Keyboard::Key::T)
                {
                    dayNight.setTime(dayNight.getTime() + 0.1f);
                }

                if (keyPressed->code == sf::Keyboard::Key::Num1) inventory.selectSlot(0);
                if (keyPressed->code == sf::Keyboard::Key::Num2) inventory.selectSlot(1);
                if (keyPressed->code == sf::Keyboard::Key::Num3) inventory.selectSlot(2);
                if (keyPressed->code == sf::Keyboard::Key::Num4) inventory.selectSlot(3);
            }
        }

        if (!running)
            break;

        Player.move();
        Player.jump();
        Player.gravity();
        
        for (size_t i = 0; i < enemies.size(); )
        {
            if (!enemies[i].isDead())
            {
                enemies[i].update(Player.getPosition(), stats);
                i++;
            }
            else
            {
                enemies.erase(enemies.begin() + i);
            }
        }

        myMap.checkItemPickup(Player.getPosition(), inventory);
        
        sf::Vector2f cameraPos = Player.getPosition();
        float halfWidth = gameView.getSize().x / 2.f;
        float halfHeight = gameView.getSize().y / 2.f;
        sf::Vector2f mapSize = myMap.getMapSize();

        if (cameraPos.x < halfWidth) cameraPos.x = halfWidth;
        if (cameraPos.x > mapSize.x - halfWidth) cameraPos.x = mapSize.x - halfWidth;
        if (cameraPos.y < halfHeight) cameraPos.y = halfHeight;
        if (cameraPos.y > mapSize.y - halfHeight) cameraPos.y = mapSize.y - halfHeight;

        gameView.setCenter(cameraPos);

        window.clear();
        window.setView(gameView);

        myMap.draw(window);

        for (const auto& decor : decorations)
        {
            decor.draw(window);
        }

        for (auto& enemy : enemies)
        {
            enemy.draw(window);
        }

        Player.draw(window);

        window.setView(window.getDefaultView());

        dayNight.draw(window);

        drawHUD(window, stats, heartFullTex, heartHalfTex, heartEmptyTex, 
                energyFullTex, energyHalfTex, energyEmptyTex);

        inventory.draw(window);

        if (inventory.getIsOpen())
        {
            crafting.draw(window, inventory);
        }

        window.display();
    }

    enemies.clear();
    decorations.clear();
    Enemy::releaseTexture();
    ItemDatabase::shutDown();
    }

    window.close();

    return 0;
}