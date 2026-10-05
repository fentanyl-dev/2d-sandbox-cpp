#include "inventory.h"
#include <iostream>
#include <string>

using namespace std;

Inventory::Inventory() : inventorySprite(inventoryTexture), amountText(font)
{
    slots.resize(26);

    if (!inventoryTexture.loadFromFile("assets/inventory/Inventory.png"))
    {
        cout << "Nie udalo sie wczytac tekstury inventory!" << endl;
    }
    else
    {
        inventorySprite.setTexture(inventoryTexture);
        inventorySprite.setTextureRect(sf::IntRect({440, 0}, {111, 107}));
        inventorySprite.setScale({2.2f, 2.2f}); 
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

    // Wygląd hotbara na dole ekranu (gdy ekwipunek jest zamknięty)
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
    float totalWidth = (5 * slotSize) + (4 * padding) + 12.f;
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
    float s = 2.2f;

    // 1. Hotbar (0 - 4) -> dolny prawy pasek (5 slotów)
    if (index >= 0 && index < 5)
    {
        return { invPos.x + (32.0f + index * 13.5f) * s, invPos.y + 83.5f * s };
    }
    // 2. Siatka główna 4x4 (5 - 20) -> górna prawa siatka
    else if (index >= 5 && index < 21)
    {
        int gridIdx = index - 5;
        int col = gridIdx % 4;
        int row = gridIdx / 4;
        return { invPos.x + (32.0f + col * 17.0f) * s, invPos.y + (5.5f + row * 17.0f) * s };
    }
    // 3. Lewy pionowy pasek 1x4 (21 - 24)
    else if (index >= 21 && index < 25)
    {
        int row = index - 21;
        return { invPos.x + 5.5f * s, invPos.y + (5.5f + row * 17.0f) * s };
    }
    // 4. Pojedynczy slot lewy dolny (25)
    else if (index == 25)
    {
        return { invPos.x + 5.5f * s, invPos.y + 83.5f * s };
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
    if (index >= 0 && index < 5)
    {
        selectedSlot = index;
    }
}

void Inventory::handleMouseClick(sf::Vector2f mousePos, sf::RenderWindow& window)
{
    if (!isOpen) return;

    sf::View currentView = window.getView();
    sf::Vector2f center = currentView.getCenter();
    sf::Vector2f size = currentView.getSize();

    sf::Vector2f invPos(center.x + (size.x / 2.f) - 260.f, center.y - (size.y / 2.f) + 20.f);

    for (size_t i = 0; i < slots.size(); i++)
    {
        sf::Vector2f pos = getSlotPosition(i, invPos);
        float slotBoundSize = (i < 5) ? 24.f : 32.f;
        sf::FloatRect slotBounds(pos, {slotBoundSize, slotBoundSize});

        if (slotBounds.contains(mousePos))
        {
            if (draggedSlotIndex == -1)
            {
                if (slots[i].itemID != 0)
                {
                    draggedSlotIndex = i;
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

    sf::RectangleShape itemPlaceholder({26.f, 26.f});

    // 1. Rysowanie skróconego hotbara na dole ekranu, gdy ekwipunek NIE jest otwarty
    if (!isOpen)
    {
        float slotSize = 42.f; 
        float padding = 5.f;
        float totalSlotsWidth = (5 * slotSize) + (4 * padding);
        float startX = center.x - (totalSlotsWidth / 2.f);
        float startY = center.y + (size.y / 2.f) - slotSize - 18.f;

        backgroundBar.setPosition({startX - 6.f, startY - 6.f});
        window.draw(backgroundBar);

        for (int i = 0; i < 5; i++)
        {
            sf::Vector2f pos(startX + i * (slotSize + padding), startY);

            slotBox.setPosition(pos);
            window.draw(slotBox);

            if (slots[i].itemID != 0)
            {
                itemPlaceholder.setSize({26.f, 26.f});
                itemPlaceholder.setTexture(nullptr);
                itemPlaceholder.setFillColor(sf::Color::White);

                if (slots[i].itemID == 1) itemPlaceholder.setTexture(&cakeTexture);
                else if (slots[i].itemID == 3) itemPlaceholder.setTexture(&swordTexture);
                else itemPlaceholder.setFillColor(sf::Color::Yellow);

                itemPlaceholder.setPosition({pos.x + 8.f, pos.y + 8.f});
                window.draw(itemPlaceholder);

                if (slots[i].amount > 1 && font.getInfo().family != "")
                {
                    amountText.setString(to_string(slots[i].amount));
                    amountText.setPosition({pos.x + slotSize - 16.f, pos.y + slotSize - 18.f});
                    window.draw(amountText);
                }
            }

            if (i == selectedSlot)
            {
                selectorBox.setPosition(pos);
                window.draw(selectorBox);
            }
        }
    }

    // 2. Rysowanie pełnego ekwipunku po naciśnięciu 'I'
    if (isOpen)
    {
        sf::Vector2f invPos(center.x + (size.x / 2.f) - 260.f, center.y - (size.y / 2.f) + 20.f);

        inventorySprite.setPosition(invPos);
        window.draw(inventorySprite);

        for (size_t i = 0; i < slots.size(); i++)
        {
            if (i == static_cast<size_t>(draggedSlotIndex)) continue;

            if (slots[i].itemID != 0)
            {
                sf::Vector2f slotPos = getSlotPosition(i, invPos);

                // Mniejszy rozmiar dla paska hotbar, większy dla pozostałych kratek
                if (i < 5)
                {
                    itemPlaceholder.setSize({20.f, 20.f});
                    itemPlaceholder.setPosition({slotPos.x + 3.f, slotPos.y + 3.f});
                }
                else
                {
                    itemPlaceholder.setSize({28.f, 28.f});
                    itemPlaceholder.setPosition({slotPos.x + 3.f, slotPos.y + 3.f});
                }

                itemPlaceholder.setTexture(nullptr);
                itemPlaceholder.setFillColor(sf::Color::White);

                if (slots[i].itemID == 1) itemPlaceholder.setTexture(&cakeTexture);
                else if (slots[i].itemID == 3) itemPlaceholder.setTexture(&swordTexture);
                else itemPlaceholder.setFillColor(sf::Color::Yellow);

                window.draw(itemPlaceholder);

                if (slots[i].amount > 1 && font.getInfo().family != "")
                {
                    amountText.setString(to_string(slots[i].amount));
                    amountText.setPosition({slotPos.x + 14.f, slotPos.y + 14.f});
                    window.draw(amountText);
                }
            }
        }

        // 3. Rysowanie podniesionego przedmiotu przy kursorze myszy
        if (draggedSlotIndex != -1)
        {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, window.getDefaultView());

            sf::RectangleShape draggedShape({24.f, 24.f});
            draggedShape.setOrigin({12.f, 12.f});
            draggedShape.setPosition(mousePos);

            if (slots[draggedSlotIndex].itemID == 1) draggedShape.setTexture(&cakeTexture);
            else if (slots[draggedSlotIndex].itemID == 3) draggedShape.setTexture(&swordTexture);
            else draggedShape.setFillColor(sf::Color::Yellow);

            window.draw(draggedShape);
        }
    }
}