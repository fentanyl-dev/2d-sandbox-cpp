#include "inventory.h"

Inventory::Inventory() : inventorySprite(inventoryTexture)
{
    slots.resize(5);

    if (!inventoryTexture.loadFromFile("assets/inventory/Inventory.png"))
    {
        cout << "Nie udalo sie wczytac tekstury inventory!" << endl;
    }
    else
    {
        inventorySprite.setTexture(inventoryTexture);
        inventorySprite.setTextureRect(sf::IntRect({440, 0}, {111, 107}));
        inventorySprite.setScale({2.2f, 2.2f}); 
        inventorySprite.setOrigin({111.f, 0.f}); 
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
    float totalWidth = (5 * slotSize) + (4 * padding) + 12.f;
    backgroundBar.setSize({totalWidth, slotSize + 12.f});
    backgroundBar.setFillColor(sf::Color(20, 18, 24, 200));
    backgroundBar.setOutlineColor(sf::Color(60, 55, 70));
    backgroundBar.setOutlineThickness(1.5f);
}

void Inventory::toggle()
{
    isOpen = !isOpen;
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

void Inventory::draw(sf::RenderWindow& window)
{
    sf::View currentView = window.getView();
    sf::Vector2f center = currentView.getCenter();
    sf::Vector2f size = currentView.getSize();

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

        if (i == selectedSlot)
        {
            selectorBox.setPosition(pos);
            window.draw(selectorBox);
        }
    }

    
    if (isOpen)
    {
        float rightX = center.x + (size.x / 2.f);
        float topY = center.y - (size.y / 2.f);

        inventorySprite.setPosition({rightX - 10.f, topY + 10.f});
        window.draw(inventorySprite);
    }
}