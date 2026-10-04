#include "inventory.h"

//Konstruktor ktory inicjalizuje wektor slotow

Inventory::Inventory() : inventorySprite(inventoryTexture), amountText(font)
{
    slots.resize(25);

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

    if (!font.openFromFile("assets/arial.ttf"))
    {
        cout << "Brak pliku czcionki assets/arial.ttf (ilosci przedmiotow nie beda wyswietlane)" << endl;
    }
    else
    {
        amountText.setFont(font);
        amountText.setCharacterSize(12);
        amountText.setFillColor(sf::Color::White);
        amountText.setOutlineColor(sf::Color::Black);
        amountText.setOutlineThickness(1.f);
    }

    //Konfigruacja wygladu pojedynczego slotu hotbara

    float slotSize = 42.f;
    slotBox.setSize({slotSize, slotSize});
    slotBox.setFillColor(sf::Color(35, 33, 42, 230));
    slotBox.setOutlineColor(sf::Color(85, 80, 95));
    slotBox.setOutlineThickness(2.f);

    //Konfiguracja ramki zaznaczania slotu

    selectorBox.setSize({slotSize, slotSize});
    selectorBox.setFillColor(sf::Color::Transparent);
    selectorBox.setOutlineColor(sf::Color::White);
    selectorBox.setOutlineThickness(2.5f);

    //Konfiguracja tla pod pasek hotbara

    float padding = 5.f;
    float totalWidth = (5 * slotSize) + (4 * padding) + 12.f;
    backgroundBar.setSize({totalWidth, slotSize + 12.f});
    backgroundBar.setFillColor(sf::Color(20, 18, 24, 200));
    backgroundBar.setOutlineColor(sf::Color(60, 55, 70));
    backgroundBar.setOutlineThickness(1.5f);

    //Item

    if (!swordTexture.loadFromFile("assets/items/sword.png"))
    {
        cout << "Nie mozna wczytac tekstury item - sword" << endl;
    }
    swordTexture.setSmooth(false);

    if (!cakeTexture.loadFromFile("assets/items/cake.png"))
    {
        cout << "Nie mozna wczytac tekstury item - cake";
    }
    cakeTexture.setSmooth(false);
}

//Dodawanie itemu do inventory

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

//Zwraca dane przedmiotu lezacego 

inventorySlot Inventory::getSelectedItem() const
{
    return slots[selectedSlot];
}

//Odejmuje podana ilosc przedmiotu z akywtnego slotu

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

//Przelaczanie widoku inventory

void Inventory::toggle()
{
    isOpen = !isOpen;
}

//Sprawdzanie czy inventory jest otwarte

bool Inventory::getIsOpen() const
{
    return isOpen;
}

//Ustawia aktywny slot hotbara 

void Inventory::selectSlot(int index)
{
    if (index >= 0 && index < 5)
    {
        selectedSlot = index;
    }
}

//Rysuje interfejs ekwipunku

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

    sf::RectangleShape itemPlaceholder({26.f, 26.f});


    //1. Rysowanie hotbara

    for (int i = 0; i < 5; i++)
    {
        sf::Vector2f pos(startX + i * (slotSize + padding), startY);

        slotBox.setPosition(pos);
        window.draw(slotBox);

        if (slots[i].itemID != 0)
        {
            itemPlaceholder.setTexture(nullptr);
            itemPlaceholder.setFillColor(sf::Color::White);

            if (slots[i].itemID == 1) itemPlaceholder.setTexture(&cakeTexture);     // np. ID 1 = Czerwony
            else if (slots[i].itemID == 2) itemPlaceholder.setFillColor(sf::Color::Green);
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

    //Rysowanie pełnego ekwipunku 
    
    if (isOpen)
    {
        float rightX = center.x + (size.x / 2.f);
        float topY = center.y - (size.y / 2.f);

        inventorySprite.setPosition({rightX - 10.f, topY + 10.f});
        window.draw(inventorySprite);

        float gridStartX = rightX - 215.f; 
        float gridStartY = topY + 45.f;
        float slotGridSize = 38.f; 

        for (int i = 5; i < 25; i++)
        {
            int indexInGrid = i - 5;
            int col = indexInGrid % 5;
            int row = indexInGrid / 5;

            sf::Vector2f slotPos(gridStartX + col * slotGridSize, gridStartY + row * slotGridSize);

            if (slots[i].itemID != 0)
            {
                itemPlaceholder.setTexture(nullptr);
                itemPlaceholder.setFillColor(sf::Color::White);

                if (slots[i].itemID == 1) itemPlaceholder.setTexture(&cakeTexture);
                else if (slots[i].itemID == 2) itemPlaceholder.setFillColor(sf::Color::Green);
                else if (slots[i].itemID == 3) itemPlaceholder.setTexture(&swordTexture);
                else itemPlaceholder.setFillColor(sf::Color::Yellow);

                itemPlaceholder.setPosition({slotPos.x + 6.f, slotPos.y + 6.f});
                window.draw(itemPlaceholder);
            }
        }
    }
}