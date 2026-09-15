#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"

class player
{
    private:
        sf::RectangleShape hitbox;
        sf::Texture playerTexture;
        sf::Sprite playerSprite;
        sf::Texture walkTexture;
        sf::Clock animationClock;
        sf::Texture attackTexture;
        int currentFrame = 0;

        bool onGround;
        bool isWalking = false;
        bool facingRight = true;
        bool isAttacking = false;
        int attackFrame = 0;

        float velocityY;
        
        map* worldMap;

    public:
        player(map* worldMap);
        sf::Vector2f getPosition();

        void move();
        void draw(sf::RenderWindow& window);
        void jump();
        void gravity();
        void updateSpritePosition();
        void animateWalk();
        void animateIdle();
        void animateAttack();
        void attack();
};