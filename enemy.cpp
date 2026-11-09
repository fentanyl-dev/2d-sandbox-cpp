#include "enemy.h"
#include <cmath>
#include <iostream>

using namespace std;

sf::Texture* Enemy::enemyTexture = nullptr;

sf::Texture& Enemy::getTexture()
{
    if (!enemyTexture)
    {
        enemyTexture = new sf::Texture();
        if (!enemyTexture->loadFromFile("assets/enemy/blob.png"))
        {
            cout << "Nie udalo sie wczytac tekstury bloba! Uzywam koloru zastepczego." << endl;
        }
        else
        {
            enemyTexture->setSmooth(false);
        }
    }

    return *enemyTexture;
}

Enemy::Enemy(map* worldMap, sf::Vector2f startPos) : enemySprite(getTexture())
{
    this->worldMap = worldMap;

    textureLoaded = enemyTexture->getSize().x != 0;
    enemySprite.setTexture(*enemyTexture, true);
    enemySprite.setOrigin({8.f, 8.f});

    hitbox.setSize({16.f, 16.f});
    hitbox.setPosition(startPos);
    hitbox.setFillColor(sf::Color::Red);

    healthBarBackground.setFillColor(sf::Color(50,50,50,200));
    healthBarBackground.setSize({20.f, 3.0f});
    healthBarBackground.setOrigin({10.f, 0.f});

    healthBarFill.setSize({20.f, 3.f});
    healthBarFill.setFillColor({sf::Color::Green});
    healthBarFill.setOrigin({10.f, 0.f});

    updateSpritePosition();
}

void Enemy::draw(sf::RenderWindow& window)
{
    if (textureLoaded)
    {
        window.draw(enemySprite);
    }
    else
    {
        window.draw(hitbox);
    }

    if (hp > 0)
    {
        window.draw(healthBarBackground);
        window.draw(healthBarFill);
    }
    
}

void Enemy::update(sf::Vector2f playerPos, stats& playerStats)
{
    if (hitFlashTimer > 0.f)
    {
        hitFlashTimer -= 0.016f;
        if (hitFlashTimer < 0.f) hitFlashTimer = 0.f;
    }

    if (hitFlashTimer > 0.f)
    {
        enemySprite.setColor(sf::Color::Red);
        hitbox.setFillColor(sf::Color(255, 100, 100));
    }
    else
    {
        enemySprite.setColor(sf::Color::White);
        hitbox.setFillColor(sf::Color::Red);
    }

    sf::Vector2f pos = hitbox.getPosition();

    if (knockbackVelocityX != 0.f)
    {
        float nextX = pos.x + knockbackVelocityX;
        float headY = pos.y + 4.f;

        if (!worldMap->isSolid(nextX + (knockbackVelocityX > 0 ? hitbox.getSize().x : 0.f), headY))
        {
            hitbox.move({knockbackVelocityX, 0.f});
        }
        else
        {
            knockbackVelocityX = 0.f; 
        }

        knockbackVelocityX *= 0.92f;
        if (abs(knockbackVelocityX) < 0.1f)
        {
            knockbackVelocityX = 0.f;
        }  
    }
    else 
    {
        if (playerPos.x > pos.x)
        {
            movingRight = true;
            if (!worldMap->isSolid(pos.x + hitbox.getSize().x + 1.f, pos.y + hitbox.getSize().y / 2.f))
            {
                hitbox.move({speed, 0.f});
            }
        }
        else 
        {
            movingRight = false;
            if (!worldMap->isSolid(pos.x - 1.f, pos.y + hitbox.getSize().y / 2.f))
            {
                hitbox.move({-speed, 0.f});
            }
        }
    }

    // Grawitacja
    velocityY += 0.5f;
    hitbox.move({0.f, velocityY});

    float centerX = hitbox.getPosition().x + hitbox.getSize().x / 2.f;
    float bottomY = hitbox.getPosition().y + hitbox.getSize().y + 1.f;

    if (velocityY > 0 && worldMap->isSolid(centerX, bottomY))
    {
        float groundY = worldMap->getGroundY(centerX, bottomY);
        hitbox.setPosition({hitbox.getPosition().x, groundY - hitbox.getSize().y});
        velocityY = 0.f;
        onGround = true;
    }

    updateSpritePosition();

    if (movingRight)
    {
        enemySprite.setScale({1.f, 1.f});
    }
    else
    {
        enemySprite.setScale({-1.f, 1.f});
    }

    if (hitbox.getGlobalBounds().contains(playerPos))
    {
        playerStats.takeDamage(0.5f);
    }
}

void Enemy::updateSpritePosition()
{
    sf::Vector2f spriteCenter = {
        hitbox.getPosition().x + hitbox.getSize().x / 2.f,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f
    };

    enemySprite.setPosition(spriteCenter);

    sf::Vector2f barPosition = {spriteCenter.x, hitbox.getPosition().y - 4.f};

    healthBarBackground.setPosition(barPosition);
    healthBarFill.setPosition(barPosition);

    float healthPrecent = hp / maxHp;
    if (healthPrecent < 0.f) healthPrecent = 0.f;

    healthBarFill.setSize({20.f * healthPrecent, 3.f});
    
}

sf::FloatRect Enemy::getBounds() const
{
    return hitbox.getGlobalBounds();
}

void Enemy::releaseTexture()
{
    delete enemyTexture;
    enemyTexture = nullptr;
}