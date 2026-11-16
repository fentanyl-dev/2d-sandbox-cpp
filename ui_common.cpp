#include "ui_common.h"
#include "item.h"
#include <string>
#include <cstdint>

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

    void drawDurability(sf::RenderTarget& target, int itemID, int durability,
                        sf::Vector2f slotPos, float slotSize)
    {
        const ItemDef* def = ItemDatabase::get(itemID);
        if (!def || def->maxDurability <= 0) return;

        int currentDur = (durability <= 0) ? def->maxDurability : durability;

        float frac = static_cast<float>(currentDur) / def->maxDurability;
        float w = slotSize * 0.8f;
        sf::Vector2f pos(slotPos.x + (slotSize - w) / 2.f, slotPos.y + slotSize - 7.f);

        sf::RectangleShape bg({w, 3.f});
        bg.setPosition(pos);
        bg.setFillColor(sf::Color(20, 20, 20, 220));
        target.draw(bg);

        sf::RectangleShape fill({w * frac, 3.f});
        fill.setPosition(pos);
        fill.setFillColor(sf::Color(static_cast<std::uint8_t>(255 * (1.f - frac)),
                                    static_cast<std::uint8_t>(255 * frac), 0));
        target.draw(fill);
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