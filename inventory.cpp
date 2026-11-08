#include "inventory.h"
#include <iostream>
#include <string>

#include "item.h"
#include "ui_common.h"
#include <algorithm>

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

    constexpr int HOTBAR_COUNT = 4;

    constexpr bool DEBUG_SLOTS = false;

    sf::Vector2f computeInventoryPosition(const sf::View& view, const sf::Sprite& sprite)
    {
        sf::Vector2f center = view.getCenter();
        sf::Vector2f size = view.getSize();
        float spriteWidth = sprite.getGlobalBounds().size.x;

        return { center.x + (size.x / 2.f) - spriteWidth - 16.f,
                 center.y - (size.y / 2.f) + 20.f };
    }

    int maxStackOf(int itemID)
    {
        const ItemDef* def = ItemDatabase::get(itemID);
        return def ? def->maxStack : 99;
    }
}

Inventory::Inventory() : inventorySprite(inventoryTexture)
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

    if (!font.openFromFile("assets/font/ARIAL.TTF"))
    {
        cout << "Brak pliku czcionki assets/font/ARIAL.TTF" << endl;
    }
    else
    {
        fontLoaded = true;
    }

    selectorBox.setSize({ui::SLOT_SIZE, ui::SLOT_SIZE});
    selectorBox.setFillColor(sf::Color::Transparent);
    selectorBox.setOutlineColor(sf::Color::White);
    selectorBox.setOutlineThickness(2.5f);

    float totalWidth = (4 * ui::SLOT_SIZE) + (3 * ui::SLOT_PAD) + 12.f;
    backgroundBar.setSize({totalWidth, ui::SLOT_SIZE + 12.f});
    backgroundBar.setFillColor(sf::Color(20, 18, 24, 200));
    backgroundBar.setOutlineColor(sf::Color(60, 55, 70));
    backgroundBar.setOutlineThickness(1.5f);
}

const sf::Font* Inventory::getFont() const
{
    return fontLoaded ? &font : nullptr;
}

sf::Vector2f Inventory::getSlotPosition(int index, sf::Vector2f invPos) const
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

int Inventory::slotIndexAt(sf::Vector2f mousePos, sf::Vector2f invPos) const
{
    const float slotSize = SLOT_TEX_SIZE * UI_SCALE;

    for (int i = 0; i < static_cast<int>(slots.size()); i++)
    {
        sf::FloatRect bounds(getSlotPosition(i, invPos), {slotSize, slotSize});
        if (bounds.contains(mousePos)) return i;
    }
    return -1;
}

int Inventory::fillRange(int itemID, int amount, int begin, int end)
{
    int maxStack = maxStackOf(itemID);

    for (int i = begin; i < end && amount > 0; i++)
    {
        inventorySlot& s = slots[i];
        if (s.itemID == itemID && s.amount < maxStack)
        {
            int add = std::min(maxStack - s.amount, amount);
            s.amount += add;
            amount -= add;
        }
    }

    for (int i = begin; i < end && amount > 0; i++)
    {
        inventorySlot& s = slots[i];
        if (s.itemID == 0)
        {
            int add = std::min(maxStack, amount);
            s.itemID = itemID;
            s.amount = add;
            amount -= add;
        }
    }

    return amount;
}


void Inventory::quickMove(int index)
{
    inventorySlot& from = slots[index];
    if (from.itemID == 0) return;

    int total = static_cast<int>(slots.size());
    int begin = (index < HOTBAR_COUNT) ? HOTBAR_COUNT : 0;
    int end   = (index < HOTBAR_COUNT) ? total : HOTBAR_COUNT;

    int left = fillRange(from.itemID, from.amount, begin, end);
    from.amount = left;
    if (left == 0) from = inventorySlot{};
}

void Inventory::returnCursorItem()
{
    if (cursorItem.itemID == 0) return;

    cursorItem.amount = fillRange(cursorItem.itemID, cursorItem.amount, 0, static_cast<int>(slots.size()));
    if (cursorItem.amount == 0) cursorItem = inventorySlot{};
}

bool Inventory::addItem(int itemID, int amount)
{
    return fillRange(itemID, amount, 0, static_cast<int>(slots.size())) == 0;
}

int Inventory::countItem(int itemID) const
{
    int total = 0;
    for (const auto& slot : slots)
    {
        if (slot.itemID == itemID) total += slot.amount;
    }
    return total;
}

void Inventory::removeItem(int itemID, int amount)
{
    for (auto& slot : slots)
    {
        if (slot.itemID != itemID) continue;

        int take = std::min(slot.amount, amount);
        slot.amount -= take;
        amount -= take;

        if (slot.amount == 0) slot.itemID = 0;
        if (amount == 0) return;
    }
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
    if (isOpen) returnCursorItem();
    isOpen = !isOpen;
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
    int idx = slotIndexAt(mousePos, invPos);
    if (idx == -1) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        if (cursorItem.itemID == 0) quickMove(idx);
        return;
    }

    inventorySlot& slot = slots[idx];

    if (cursorItem.itemID == 0)
    {
        // podnies caly stack
        if (slot.itemID != 0)
        {
            cursorItem = slot;
            slot = inventorySlot{};
        }
    }
    else if (slot.itemID == 0)
    {
        slot = cursorItem;
        cursorItem = inventorySlot{};
    }
    else if (slot.itemID == cursorItem.itemID)
    {
        int room = maxStackOf(slot.itemID) - slot.amount;
        int add = std::min(room, cursorItem.amount);
        if (add > 0)
        {
            slot.amount += add;
            cursorItem.amount -= add;
            if (cursorItem.amount == 0) cursorItem = inventorySlot{};
        }
    }
    else
    {
        std::swap(slot, cursorItem);
    }
}

void Inventory::handleRightClick(sf::Vector2f mousePos, sf::RenderWindow& window)
{
    if (!isOpen) return;

    sf::Vector2f invPos = computeInventoryPosition(window.getView(), inventorySprite);
    int idx = slotIndexAt(mousePos, invPos);
    if (idx == -1) return;

    inventorySlot& slot = slots[idx];

    if (cursorItem.itemID == 0)
    {
        if (slot.itemID == 0) return;

        int take = (slot.amount + 1) / 2;
        cursorItem.itemID = slot.itemID;
        cursorItem.amount = take;

        slot.amount -= take;
        if (slot.amount == 0) slot = inventorySlot{};
        return;
    }

    if (slot.itemID == 0)
    {
        slot.itemID = cursorItem.itemID;
        slot.amount = 1;
    }
    else if (slot.itemID == cursorItem.itemID && slot.amount < maxStackOf(slot.itemID))
    {
        slot.amount++;
    }
    else
    {
        return;
    }

    cursorItem.amount--;
    if (cursorItem.amount == 0) cursorItem = inventorySlot{};
}

void Inventory::draw(sf::RenderWindow& window)
{
    sf::View currentView = window.getView();
    sf::Vector2f center = currentView.getCenter();
    sf::Vector2f size = currentView.getSize();
    const sf::Font* f = getFont();

    if (!isOpen)
    {
        float slotSize = ui::SLOT_SIZE;
        float padding = ui::SLOT_PAD;
        float totalSlotsWidth = (4 * slotSize) + (3 * padding);
        float startX = center.x - (totalSlotsWidth / 2.f);
        float startY = center.y + (size.y / 2.f) - slotSize - 18.f;

        backgroundBar.setPosition({startX - 6.f, startY - 6.f});
        window.draw(backgroundBar);

        for (int i = 0; i < HOTBAR_COUNT; i++)
        {
            sf::Vector2f pos(startX + i * (slotSize + padding), startY);

            ui::drawSlot(window, pos, slotSize);

            if (slots[i].itemID != 0)
            {
                ui::drawItemIcon(window, slots[i].itemID, pos, slotSize);
                ui::drawAmount(window, f, slots[i].amount, pos, slotSize);
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

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), currentView);
        int hovered = slotIndexAt(mousePos, invPos);

        for (size_t i = 0; i < slots.size(); i++)
        {
            if (slots[i].itemID != 0)
            {
                sf::Vector2f slotPos = getSlotPosition(static_cast<int>(i), invPos);
                ui::drawItemIcon(window, slots[i].itemID, slotPos, slotWidth);
                ui::drawAmount(window, f, slots[i].amount, slotPos, slotWidth);
            }
        }

        if (hovered != -1)
        {
            ui::drawHover(window, getSlotPosition(hovered, invPos), slotWidth);
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

        if (hovered != -1 && cursorItem.itemID == 0 && slots[hovered].itemID != 0)
        {
            const ItemDef* def = ItemDatabase::get(slots[hovered].itemID);
            if (def)
            {
                ui::drawTooltip(window, f, def->name, mousePos);
            }
        }

        if (cursorItem.itemID != 0)
        {
            sf::Vector2f topLeft = mousePos - sf::Vector2f(slotWidth / 2.f, slotWidth / 2.f);
            ui::drawItemIcon(window, cursorItem.itemID, topLeft, slotWidth);
            ui::drawAmount(window, f, cursorItem.amount, topLeft, slotWidth);
        }
    }
}