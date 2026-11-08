#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "stats.h"

class Enemy
{
private:
    sf::RectangleShape hitbox;
    static sf::Texture enemyTexture; 
    sf::Sprite enemySprite;

    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarFill;
    float maxHp = 7.0f;

    map* worldMap;

    float hitFlashTimer = 0.f;

    float knockbackVelocityX = 0.f;

    float velocityY = 0.f;
    float speed = 1.5f;
    float hp = 7.0f;
    bool movingRight = true;
    bool onGround = false;

    bool textureLoaded = false;

    void updateSpritePosition();

public:
    Enemy(map* worldMap, sf::Vector2f startPos);

    void update(sf::Vector2f playerPos, stats& playerStats);
    void draw(sf::RenderWindow& window);

    void takeDamage(float amount, sf::Vector2f playerPos)
    {
        hp -= amount;
        hitFlashTimer = 0.15f;

        if (playerPos.x < hitbox.getPosition().x)
        {
            knockbackVelocityX = 14.0f;
        }
        else
        {
            knockbackVelocityX = -14.0f;
        }

        //velocityY = -7.0f;
    }

    bool isDead() const
    {
        return hp <= 0;
    }
    
    sf::FloatRect getBounds() const;
};