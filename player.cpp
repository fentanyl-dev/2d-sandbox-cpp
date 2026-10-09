#include "player.h"
#include <iostream>

using namespace std;

player::player(map* worldMap):playerSprite(playerTexture)
{
    this->worldMap = worldMap;

    if (!playerTexture.loadFromFile("assets/player/idle/Soldier_Idle.png"))
    {
        cout << "Nie udalo sie wczytac tekstury postaci!";
    }

    if (!walkTexture.loadFromFile("assets/player/walking/Soldier_Walk.png"))
    {
        cout << "Nie udalo sie wczytac tekstury chodzenia postaci!";
    }

    if (!attackTexture.loadFromFile("assets/player/attack/Soldier_Attack01.png"))
    {
        cout << "Nie udalo sie wczytac tekstru ataku postaci";
    }
    

    hitbox.setSize({16.f, 32.f});
    hitbox.setPosition({100.f, 100.f});

    playerSprite.setTexture(walkTexture);
    playerSprite.setTextureRect(sf::IntRect({0, 0}, {100,100}));

    playerSprite.setPosition({475.f, 300.f});

    playerSprite.setOrigin({50.f, 57.f}); 

    playerSprite.setScale({1.f, 1.f});

     onGround = true;
     velocityY = 0.f;

     playerTexture.setSmooth(false);
     walkTexture.setSmooth(false);
     attackTexture.setSmooth(false);

     updateSpritePosition();

}

void player::draw(sf::RenderWindow& window)
{
    window.draw(playerSprite);

}

void player::move()
{
    isWalking = false;

    float playerCenterY = hitbox.getPosition().y + hitbox.getSize().y / 2.f;

    float playerRight = hitbox.getPosition().x + hitbox.getSize().x;

    float playerLeft = hitbox.getPosition().x - 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        if (!worldMap->isSolid(playerRight,playerCenterY))
        {
            hitbox.move({5.f, 0.f});
            isWalking = true;
            facingRight = true;
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (!worldMap->isSolid(playerLeft, playerCenterY))
        {
            hitbox.move({-5.f, 0.f});
            isWalking = true;
            facingRight = false;
        }
    }
    updateSpritePosition();

    if (isAttacking)
    {
        animateAttack();
    }
    else
    {
        animateWalk();
    }
    
}

void player::jump()
{

    if (onGround)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            velocityY = -7.f;
            onGround = false;
        }
    }
    
}

void player::gravity()
{
    velocityY += 0.5f;

    hitbox.move({0.f, velocityY});

    float playerCenterX = hitbox.getPosition().x + hitbox.getSize().x / 2.f;

    float playerBottom = hitbox.getPosition().y + hitbox.getSize().y + 1;

    if (velocityY > 0 &&
        worldMap->isSolid(playerCenterX, playerBottom))
    {
        float groundY =
            worldMap->getGroundY(playerCenterX, playerBottom);

        hitbox.setPosition({
            hitbox.getPosition().x,
            groundY - hitbox.getSize().y
        });

        velocityY = 0.f;
        onGround = true;
    }

    updateSpritePosition();
    
}

void player::updateSpritePosition()
{
    playerSprite.setPosition({hitbox.getPosition().x + hitbox.getSize().x / 2.f, hitbox.getPosition().y + hitbox.getSize().y});
}

void player::animateWalk()
{
    if (!isWalking)
    {
        animateIdle();
        return;
    }

    if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
    {
        currentFrame++;

        if (currentFrame >= 8)
        {
            currentFrame = 0;
        }

        playerSprite.setTexture(walkTexture);

        playerSprite.setTextureRect(
            sf::IntRect({currentFrame * 100, 0}, {100, 100})
        );

        animationClock.restart();

        if (facingRight)
        {
            playerSprite.setScale({1.f, 1.f});
        }
        else 
        {
            playerSprite.setScale({-1.f, 1.f});
        }
    }
}

void player::animateIdle()
{
    if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
    {
        currentFrame++;
        if (currentFrame >= 6)
        {
            currentFrame = 0;
        }

        playerSprite.setTexture(playerTexture);

        playerSprite.setTextureRect(sf::IntRect({currentFrame * 100, 0}, {100, 100}));

        animationClock.restart();
    }
    
}

void player::animateAttack()
{
    if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
    {
        attackFrame++;

        if (attackFrame >= 6)
        {
            attackFrame = 0;
            isAttacking = false;
            return;
        }

        playerSprite.setTexture(attackTexture);

        playerSprite.setTextureRect(sf::IntRect({attackFrame * 100, 0}, {100, 100}));

        animationClock.restart();
    }
    
}

void player::attack()
{
    if (!isAttacking)
    {
        isAttacking = true;
        attackFrame = 0;
        animationClock.restart();

        playerSprite.setTexture(attackTexture);
        playerSprite.setTextureRect(sf::IntRect({100, 0}, {100, 100}));
    }
    
}

sf::Vector2f player::getPosition()
{
    return 
    {
        hitbox.getPosition().x + hitbox.getSize().x / 2.f,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f
    };
}

void player::setPosition(sf::Vector2f newPos)
{
    hitbox.setPosition(newPos);
    updateSpritePosition();
}