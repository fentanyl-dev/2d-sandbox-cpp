#include "crafting.h"
#include "ui_common.h"
#include <string>

using namespace std;

namespace
{
    constexpr float UI_SCALE   = 2.2f;
    constexpr float INV_WIDTH  = 111.f * UI_SCALE;  
    constexpr float INV_MARGIN = 16.f;
    constexpr float INV_TOP    = 20.f;
    constexpr float PANEL_GAP  = 10.f;              

    constexpr float SLOT   = ui::SLOT_SIZE;
    constexpr float PAD    = ui::SLOT_PAD;
    constexpr float BG_PAD = 6.f;

    sf::Vector2f panelOrigin(const sf::View& view)
    {
        sf::Vector2f center = view.getCenter();
        sf::Vector2f size = view.getSize();

        float invX = center.x + size.x / 2.f - INV_WIDTH - INV_MARGIN;
        float invY = center.y - size.y / 2.f + INV_TOP;

        float panelWidth = SLOT + 2.f * BG_PAD;
        return { invX - PANEL_GAP - panelWidth, invY };
    }

    sf::Vector2f rowPosition(sf::Vector2f origin, int row)
    {
        return { origin.x + BG_PAD, origin.y + BG_PAD + row * (SLOT + PAD) };
    }

    int rowAt(sf::Vector2f mouse, const sf::View& view, int count)
    {
        sf::Vector2f origin = panelOrigin(view);
        for (int i = 0; i < count; i++)
        {
            sf::FloatRect bounds(rowPosition(origin, i), {SLOT, SLOT});
            if (bounds.contains(mouse)) return i;
        }
        return -1;
    }
}

Crafting::Crafting()
{
    // 2 patyki -> 4 patyki
    recipes.push_back(Recipe{ {{oak_plank, 2}}, stick, 4 });

    // 2 patyki + 3 kamienie -> kilof
    recipes.push_back(Recipe{ {{stick, 2}, {stone, 3}}, stonePickaxe, 1 });

    // 1 patyk + 2 kamienie -> miecz
    recipes.push_back(Recipe{ {{stick, 1}, {stone, 2}}, obsidianSword, 1 });
}

bool Crafting::canCraft(const Recipe& recipe, const Inventory& inv) const
{
    for (const auto& in : recipe.inputs)
    {
        if (inv.countItem(in.itemID) < in.amount) return false;
    }
    return true;
}

bool Crafting::craft(const Recipe& recipe, Inventory& inv) const
{
    if (!canCraft(recipe, inv)) return false;

    for (const auto& in : recipe.inputs)
    {
        inv.removeItem(in.itemID, in.amount);
    }

    if (!inv.addItem(recipe.resultID, recipe.resultAmount))
    {
        for (const auto& in : recipe.inputs)
        {
            inv.addItem(in.itemID, in.amount);
        }
        return false;
    }
    return true;
}

bool Crafting::handleClick(sf::Vector2f guiMousePos, sf::RenderWindow& window, Inventory& inv)
{
    int idx = rowAt(guiMousePos, window.getDefaultView(), static_cast<int>(recipes.size()));
    if (idx == -1) return false;

    craft(recipes[idx], inv);
    return true;
}

void Crafting::draw(sf::RenderWindow& window, const Inventory& inv)
{
    if (recipes.empty()) return;

    const sf::View view = window.getDefaultView();
    const sf::Font* font = inv.getFont();
    sf::Vector2f origin = panelOrigin(view);

    int count = static_cast<int>(recipes.size());
    float width  = SLOT + 2.f * BG_PAD;
    float height = count * SLOT + (count - 1) * PAD + 2.f * BG_PAD;

    sf::RectangleShape bg({width, height});
    bg.setPosition(origin);
    bg.setFillColor(sf::Color(20, 18, 24, 200));
    bg.setOutlineColor(sf::Color(60, 55, 70));
    bg.setOutlineThickness(1.5f);
    window.draw(bg);

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
    int hovered = rowAt(mouse, view, count);

    for (int i = 0; i < count; i++)
    {
        const Recipe& r = recipes[i];
        sf::Vector2f pos = rowPosition(origin, i);
        bool craftable = canCraft(r, inv);

        ui::drawSlot(window, pos, SLOT, craftable);
        ui::drawItemIcon(window, r.resultID, pos, SLOT,
                         craftable ? sf::Color::White : sf::Color(110, 110, 110));
        ui::drawAmount(window, font, r.resultAmount, pos, SLOT);

        if (i == hovered)
        {
            ui::drawHover(window, pos, SLOT);
        }
    }

    if (hovered != -1)
    {
        const Recipe& r = recipes[hovered];
        const ItemDef* def = ItemDatabase::get(r.resultID);

        string info = def ? def->name : string("Przedmiot");
        if (r.resultAmount > 1) info += " x" + to_string(r.resultAmount);

        for (const auto& in : r.inputs)
        {
            const ItemDef* idef = ItemDatabase::get(in.itemID);
            info += "\n" + (idef ? idef->name : string("?")) + ": "
                  + to_string(inv.countItem(in.itemID)) + "/" + to_string(in.amount);
        }

        ui::drawTooltip(window, font, info, mouse);
    }
}