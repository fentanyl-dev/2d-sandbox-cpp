#include "inventory.h"

Inventory::Inventory():inventorySprite(inventoryTexture)
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

        inventorySprite.setScale({0.65f, 0.65f});

        inventorySprite.setOrigin({110.f, 0.f});
    }
    
}

void Inventory::toggle()
{
    isOpen =! isOpen;
}

bool Inventory::getIsOpen() const
{
    return isOpen;
}

void Inventory::draw(sf::RenderWindow& window)
{
    if (isOpen)
    {
        sf::View currentView = window.getView();
        sf::Vector2f center = window.getView().getCenter();
        sf::Vector2f size = currentView.getSize();

        float rightX = center.x + (size.x / 2.f);
        float topY = center.y - (size.y / 2.f);

        inventorySprite.setPosition({rightX - 10.f, topY + 10.f});

        window.draw(inventorySprite);
    }
}