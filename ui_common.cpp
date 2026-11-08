#include "ui_common.h"
#include "item.h"
#include <string>

namespace ui
{
    void drawSlot(sf::RenderTarget& target, sf::Vector2f pos, float size, bool highlighted)
    {
        sf::RectangleShape box({size, size});
        box.setPosition(pos);

        if (highlighted)
        {
            box.setFillColor(sf::Color(35, 80, 40, 230));
            box.setOutlineColor(sf::Color(120, 230, 120));
        }
        else
        {
            box.setFillColor(sf::Color(35, 33, 42, 230));
            box.setOutlineColor(sf::Color(85, 80, 95));
        }
        box.setOutlineThickness(2.f);

        target.draw(box);
    }

    void drawHover(sf::RenderTarget& target, sf::Vector2f pos, float size)
    {
        sf::RectangleShape hl({size, size});
        hl.setPosition(pos);
        hl.setFillColor(sf::Color(255, 255, 255, 50));
        target.draw(hl);
    }

    void drawItemIcon(sf::RenderTarget& target, int itemID, sf::Vector2f slotPos, float slotSize,
                      sf::Color tint)
    {
        float itemSize = slotSize * 0.75f;
        float offset = (slotSize - itemSize) / 2.f;

        sf::RectangleShape shape({itemSize, itemSize});
        shape.setPosition({slotPos.x + offset, slotPos.y + offset});

        const ItemDef* def = ItemDatabase::get(itemID);

        if (def && def->texture)
        {
            shape.setTexture(def->texture, true);
            if (def->isTile)
            {
                shape.setTextureRect(def->tileRect);
            }
            shape.setFillColor(tint);
        }
        else
        {
            shape.setFillColor(sf::Color::Yellow);   
        }

        target.draw(shape);
    }

    void drawAmount(sf::RenderTarget& target, const sf::Font* font, int amount,
                    sf::Vector2f slotPos, float slotSize)
    {
        if (!font || amount <= 1) return;

        sf::Text text(*font, std::to_string(amount), 12);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1.f);

        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin({b.position.x + b.size.x, b.position.y + b.size.y});
        text.setPosition({slotPos.x + slotSize - 3.f, slotPos.y + slotSize - 3.f});

        target.draw(text);
    }

    void drawTooltip(sf::RenderTarget& target, const sf::Font* font, const std::string& str,
                     sf::Vector2f mousePos)
    {
        if (!font || str.empty()) return;

        sf::Text text(*font, str, 12);
        text.setFillColor(sf::Color::White);
        text.setPosition({mousePos.x + 12.f, mousePos.y - 20.f});

        sf::FloatRect tb = text.getGlobalBounds();
        sf::RectangleShape bg({tb.size.x + 8.f, tb.size.y + 8.f});
        bg.setPosition({tb.position.x - 4.f, tb.position.y - 4.f});
        bg.setFillColor(sf::Color(20, 18, 24, 230));
        bg.setOutlineColor(sf::Color(85, 80, 95));
        bg.setOutlineThickness(1.f);

        target.draw(bg);
        target.draw(text);
    }
}