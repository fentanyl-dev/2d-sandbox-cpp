#include "decoration.h"

Decoration::Decoration(const sf::Texture& texture, DecorationType type, sf::Vector2f position): sprite(texture)
{
    switch (type)
    {
    case DecorationType::treeLarge:
            sprite.setTextureRect(sf::IntRect({256, 160}, {144, 160}));
            sprite.setOrigin({72.f, 160.f});
        break;
    case DecorationType::treeMedium:
            sprite.setTextureRect(sf::IntRect({144,176}, {64,144}));
            sprite.setOrigin({32.f, 144.f});
        break;
    case DecorationType::treeSmall:
            sprite.setTextureRect(sf::IntRect({0,272}, {32, 48}));
            sprite.setOrigin({16.f, 48.f});
        break;
    case DecorationType::barrel:
            sprite.setTextureRect(sf::IntRect({0, 344}, {32, 40}));
            sprite.setScale({0.8f, 0.8f});
            sprite.setOrigin({16.f, 40.f});
        break;
    case DecorationType::crate:
            sprite.setTextureRect(sf::IntRect({192, 344}, {32, 40}));
            sprite.setScale({0.6f, 0.6f});
            sprite.setOrigin({16.f, 40.f});
        break;
    case DecorationType::singpost:
            sprite.setTextureRect(sf::IntRect({256, 312}, {32, 72}));
            sprite.setOrigin({16.f, 72.f});
        break;
    }
    sprite.setPosition(position);
}

void Decoration::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}