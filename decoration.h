#ifndef DECORATION_H
#define DECORATION_H
#include <SFML/Graphics.hpp>

enum class DecorationType
{
    treeLarge,
    treeMedium,
    treeSmall,
    barrel,
    crate,
    singpost
};

class Decoration
{
    private:
        sf::Sprite sprite;
    public:
        Decoration(const sf::Texture& texture, DecorationType type, sf::Vector2f position);
        void draw(sf::RenderWindow& window) const;
};

#endif 