#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace ui
{
    constexpr float SLOT_SIZE = 42.f; 
    constexpr float SLOT_PAD  = 5.f;   


    void drawSlot(sf::RenderTarget& target, sf::Vector2f pos, float size, bool highlighted = false);

    void drawHover(sf::RenderTarget& target, sf::Vector2f pos, float size);

    void drawItemIcon(sf::RenderTarget& target, int itemID, sf::Vector2f slotPos, float slotSize,
                      sf::Color tint = sf::Color::White);

    void drawAmount(sf::RenderTarget& target, const sf::Font* font, int amount,
                    sf::Vector2f slotPos, float slotSize);

    void drawTooltip(sf::RenderTarget& target, const sf::Font* font, const std::string& text,
                     sf::Vector2f mousePos);
}