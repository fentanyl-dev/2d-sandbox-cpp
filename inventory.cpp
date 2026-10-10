#include "inventory.h"
#include <iostream>
#include <string>

using namespace std;

namespace
{
    constexpr float UI_SCALE = 2.2f;

    constexpr float SLOT_TEX_SIZE = 16.f;

    constexpr float SLOT_PITCH = 17.f;

    constexpr float GRID_X   = 40.f;
    constexpr float GRID_Y   = 4.f;
    constexpr float HOTBAR_X = 40.f;
    constexpr float HOTBAR_Y = 83.f;
    constexpr float SIDE_X   = 12.f;
    constexpr float SIDE_Y   = 4.f;
    constexpr float EXTRA_Y  = 83.f;

    constexpr bool DEBUG_SLOTS = false;

    sf::Vector2f computeInventoryPosition(const sf::View& view, const sf::Sprite& sprite)
    {
        sf::Vector2f center = view.getCenter();
        sf::Vector2f size = view.getSize();
        float spriteWidth = sprite.getGlobalBounds().size.x;

        return { center.x + (size.x / 2.f) - spriteWidth - 16.f,
                 center.y - (size.y / 2.f) + 20.f };
    }

    sf::Texture& terrainTexture()
    {
        static sf::Texture texture;
        static bool loaded = false;

        if (!loaded)
        {
            loaded = true;
            if (!texture.loadFromFile("assets/world/Terrain.png"))
            {
                cout << "Nie udalo sie wczytac Terrain.png dla inventory!" << endl;
            }
            texture.setSmooth(false);
        }
        return texture;
    }

    sf::IntRect getTileItemRect(int itemID)
    {
        switch (itemID - 100)
        {
            case 2:  return sf::IntRect({16, 16}, {16, 16});
            case 7:  return sf::IntRect({64, 0}, {16, 16});
            case 8:  return sf::IntRect({0, 48}, {16, 16});
            case 9:  return sf::IntRect({16, 48}, {16, 16});
            case 10: return sf::IntRect({0, 32}, {16, 16});
            case 11: return sf::IntRect({16, 32}, {16, 16});
            case 12: return sf::IntRect({32, 32}, {16, 16});
            default: return sf::IntRect({0, 0}, {0, 0});
        }
    }
}

Inventory::Inventory() : inventorySprite(inventoryTexture), amountText(font)
{
    slots.resize(25);

    if (!inventoryTexture.loadFromFile("assets/inventory/Inventory.png"))
    {
        cout << "Nie udalo sie wczytac tekstury inventory!" << endl;
    }
    else
    {
        inventoryTexture.setSmooth(false);
        inventorySprite.setTexture(inventoryTexture);
        inventorySprite.setTextureRect(sf::IntRect({440, 0}, {111, 107}));
        inventorySprite.setScale({UI_SCALE, UI_SCALE});
        inventorySprite.setOrigin({0.f, 0.f});
    }

    if (!font.openFromFile("assets/arial.ttf"))
    {
        cout << "Brak pliku czcionki assets/arial.ttf" << endl;
    }
    else
    {
        amountText.setFont(font);
        amountText.setCharacterSize(12);
        amountText.setFillColor(sf::Color::White);
        amountText.setOutlineColor(sf::Color::Black);
        amountText.setOutlineThickness(1.f);
    }

    float slotSize = 42.f;
    slotBox.setSize({slotSize, slotSize});
    slotBox.setFillColor(sf::Color(35, 33, 42, 230));
    slotBox.setOutlineColor(sf::Color(85, 80, 95));
    slotBox.setOutlineThickness(2.f);

    selectorBox.setSize({slotSize, slotSize});
    selectorBox.setFillColor(sf::Color::Transparent);
    selectorBox.setOutlineColor(sf::Color::White);
    selectorBox.setOutlineThickness(2.5f);

    float padding = 5.f;
    float totalWidth = (4 * slotSize) + (3 * padding) + 12.f;
    backgroundBar.setSize({totalWidth, slotSize + 12.f});
    backgroundBar.setFillColor(sf::Color(20, 18, 24, 200));
    backgroundBar.setOutlineColor(sf::Color(60, 55, 70));
    backgroundBar.setOutlineThickness(1.5f);

    if (!swordTexture.loadFromFile("assets/items/sword.png"))
    {
        cout << "Nie mozna wczytac tekstury item - sword" << endl;
    }
    swordTexture.setSmooth(false);

    if (!cakeTexture.loadFromFile("assets/items/cake.png"))
    {
        cout << "Nie mozna wczytac tekstury item - cake" << endl;
    }
    cakeTexture.setSmooth(false);
}

sf::Vector2f Inventory::getSlotPosition(int index, sf::Vector2f invPos)
{
    const float s = UI_SCALE;

    if (index >= 0 && index < 4)
    {
        return { invPos.x + (HOTBAR_X + index * SLOT_PITCH) * s,
                 invPos.y + HOTBAR_Y * s };
    }
    else if (index >= 4 && index < 20)
    {
        int gridIdx = index - 4;
        int col = gridIdx % 4;
        int row = gridIdx / 4;
        return { invPos.x + (GRID_X + col * SLOT_PITCH) * s,
                 invPos.y + (GRID_Y + row * SLOT_PITCH) * s };
    }
    else if (index >= 20 && index < 24)
    {
        int row = index - 20;
        return { invPos.x + SIDE_X * s,
                 invPos.y + (SIDE_Y + row * SLOT_PITCH) * s };
    }
    else if (index == 24)
    {
        return { invPos.x + SIDE_X * s, invPos.y + EXTRA_Y * s };
    }

    return { 0.f, 0.f };
}

bool Inventory::addItem(int itemID, int amount)
{
    for (auto& slot : slots)
    {
        if (slot.itemID == itemID)
        {
            slot.amount += amount;
            return true;
        }
    }
    for (auto& slot : slots)
    {
        if (slot.itemID == 0)
        {
            slot.itemID = itemID;
            slot.amount = amount;
            return true;
        }
    }
    return false;
}

inventorySlot Inventory::getSelectedItem() const
{
    return slots[selectedSlot];
}

bool Inventory::useSelectedItem(int amount)
{
    if (slots[selectedSlot].itemID != 0)
    {
        slots[selectedSlot].amount -= amount;
        if (slots[selectedSlot].amount <= 0)
        {
            slots[selectedSlot].itemID = 0;
            slots[selectedSlot].amount = 0;
        }
        return true;
    }
    return false;
}

void Inventory::toggle()
{
    isOpen = !isOpen;
    draggedSlotIndex = -1;
}

bool Inventory::getIsOpen() const
{
    return isOpen;
}

void Inventory::selectSlot(int index)
{
    if (index >= 0 && index < 4)
    {
        selectedSlot = index;
    }
}

void Inventory::handleMouseClick(sf::Vector2f mousePos, sf::RenderWindow& window)
{
    if (!isOpen) return;

    sf::Vector2f invPos = computeInventoryPosition(window.getView(), inventorySprite);
    const float slotSize = SLOT_TEX_SIZE * UI_SCALE;

    for (size_t i = 0; i < slots.size(); i++)
    {
        sf::Vector2f pos = getSlotPosition(static_cast<int>(i), invPos);
        sf::FloatRect slotBounds(pos, {slotSize, slotSize});

        if (slotBounds.contains(mousePos))
        {
            if (draggedSlotIndex == -1)
            {
                if (slots[i].itemID != 0)
                {
                    draggedSlotIndex = static_cast<int>(i);
                }
            }
            else
            {
                std::swap(slots[draggedSlotIndex], slots[i]);
                draggedSlotIndex = -1;
            }
            break;
        }
    }
}

void Inventory::draw(sf::RenderWindow& window)
{
    sf::View currentView = window.getView();
    sf::Vector2f center = currentView.getCenter();
    sf::Vector2f size = currentView.getSize();

    sf::RectangleShape itemShape;

    auto applyItemTexture = [&](sf::RectangleShape& shape, int itemID)
    {
        if (itemID == 1)
        {
            shape.setTexture(&cakeTexture, true);
        }
        else if (itemID == 3)
        {
            shape.setTexture(&swordTexture, true);
        }
        else if (itemID >= 100)
        {
            shape.setTexture(&terrainTexture());
            shape.setTextureRect(getTileItemRect(itemID));
        }
        else
        {
            shape.setFillColor(sf::Color::Yellow);
        }
    };

    auto drawItem = [&](const inventorySlot& slot, sf::Vector2f slotPos, float slotWidth)
    {
        float itemSize = slotWidth * 0.75f;
        float offset = (slotWidth - itemSize) / 2.f;

        itemShape.setSize({itemSize, itemSize});
        itemShape.setPosition({slotPos.x + offset, slotPos.y + offset});
        itemShape.setTexture(nullptr);
        itemShape.setFillColor(sf::Color::White);

        applyItemTexture(itemShape, slot.itemID);

        window.draw(itemShape);

        if (slot.amount > 1 && font.getInfo().family != "")
        {
            amountText.setString(to_string(slot.amount));
            amountText.setPosition({slotPos.x + slotWidth - 16.f, slotPos.y + slotWidth - 18.f});
            window.draw(amountText);
        }
    };

    if (!isOpen)
    {
        float slotSize = 42.f;
        float padding = 5.f;
        float totalSlotsWidth = (4 * slotSize) + (3 * padding);
        float startX = center.x - (totalSlotsWidth / 2.f);
        float startY = center.y + (size.y / 2.f) - slotSize - 18.f;

        backgroundBar.setPosition({startX - 6.f, startY - 6.f});
        window.draw(backgroundBar);

        for (int i = 0; i < 4; i++)
        {
            sf::Vector2f pos(startX + i * (slotSize + padding), startY);

            slotBox.setPosition(pos);
            window.draw(slotBox);

            if (slots[i].itemID != 0 && i != draggedSlotIndex)
            {
                drawItem(slots[i], pos, slotSize);
            }

            if (i == selectedSlot)
            {
                selectorBox.setPosition(pos);
                window.draw(selectorBox);
            }
        }
    }

    if (isOpen)
    {
        sf::Vector2f invPos = computeInventoryPosition(currentView, inventorySprite);

        inventorySprite.setPosition(invPos);
        window.draw(inventorySprite);

        const float slotWidth = SLOT_TEX_SIZE * UI_SCALE;

        for (size_t i = 0; i < slots.size(); i++)
        {
            if (static_cast<int>(i) == draggedSlotIndex) continue;

            if (slots[i].itemID != 0)
            {
                sf::Vector2f slotPos = getSlotPosition(static_cast<int>(i), invPos);
                drawItem(slots[i], slotPos, slotWidth);
            }
        }

        if constexpr (DEBUG_SLOTS)
        {
            for (size_t i = 0; i < slots.size(); i++)
            {
                sf::RectangleShape dbg({slotWidth, slotWidth});
                dbg.setPosition(getSlotPosition(static_cast<int>(i), invPos));
                dbg.setFillColor(sf::Color::Transparent);
                dbg.setOutlineColor(sf::Color::Red);
                dbg.setOutlineThickness(1.f);
                window.draw(dbg);
            }
        }

        if (draggedSlotIndex != -1)
        {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, currentView);

            float draggedSize = slotWidth * 0.75f;
            sf::RectangleShape draggedShape({draggedSize, draggedSize});
            draggedShape.setOrigin({draggedSize / 2.f, draggedSize / 2.f});
            draggedShape.setPosition(mousePos);

            applyItemTexture(draggedShape, slots[draggedSlotIndex].itemID);

            window.draw(draggedShape);
        }
    }
}