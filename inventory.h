#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

struct inventorySlot
{
    int itemID = 0;
    int amount = 0;
};

class Inventory
{
private:
    vector<inventorySlot> slots;

    sf::Texture inventoryTexture;
    sf::Sprite inventorySprite;

    sf::Font font;
    sf::Text amountText;

    sf::RectangleShape backgroundBar; 
    sf::RectangleShape slotBox;       
    sf::RectangleShape selectorBox;

    sf::IntRect getItemRect(int itemID);

    int selectedSlot = 0;
    bool isOpen = false;

public:
    Inventory();
    void draw(sf::RenderWindow& window);
    void toggle();
    bool getIsOpen() const;
    void selectSlot(int index);

    bool addItem(int itemID, int amount = 1);
    bool useSelectedItem(int amount = 1);
    inventorySlot getSelectedItem() const;
};