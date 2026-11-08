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
    bool fontLoaded = false;

    sf::RectangleShape backgroundBar;
    sf::RectangleShape selectorBox;

    int selectedSlot = 0;
    bool isOpen = false;

    inventorySlot cursorItem;  

    int fillRange(int itemID, int amount, int begin, int end);
    void quickMove(int index);
    void returnCursorItem();
    int slotIndexAt(sf::Vector2f mousePos, sf::Vector2f invPos) const;

public:
    Inventory();

    void draw(sf::RenderWindow& window);
    void toggle();
    bool getIsOpen() const;
    void selectSlot(int index);

    const sf::Font* getFont() const;

    bool addItem(int itemID, int amount = 1);
    bool useSelectedItem(int amount = 1);
    inventorySlot getSelectedItem() const;

    sf::Vector2f getSlotPosition(int index, sf::Vector2f invPos) const;
    void handleMouseClick(sf::Vector2f mousePos, sf::RenderWindow& window);
    void handleRightClick(sf::Vector2f mousePos, sf::RenderWindow& window);

    int countItem(int itemID) const;
    void removeItem(int itemID, int amount);
};