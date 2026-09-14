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
        int currentFrame = 0;

        bool onGround;
        bool isWalking = false;
        bool facingRight = true;
        float velocityY;
        
        map* worldMap;

    public:
        player(map* worldMap);
        void move();
        void draw(sf::RenderWindow& window);
        void jump();
        void gravity();
        void updateSpritePosition();
        void animateWalk();
        void animateIdle();
};