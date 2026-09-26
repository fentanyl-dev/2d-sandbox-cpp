#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

struct inventorySlot
{
    int itemID;
    int amount;
};

class Inventory
{
    private:
        vector<inventorySlot> slots;
        sf::Texture inventoryTexture;
        sf::Sprite inventorySprite;
        bool isOpen = false;

    public:
        Inventory();
        void draw(sf::RenderWindow& window);
        void toggle();
        bool getIsOpen() const;
};