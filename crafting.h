#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "inventory.h"
#include "item.h"

struct Ingredient
{
    int itemID;
    int amount;
};

struct Recipe
{
    std::vector<Ingredient> inputs;
    int resultID;
    int resultAmount = 1;
};

class Crafting
{
public:
    Crafting();

    Crafting(const Crafting&) = delete;
    Crafting& operator=(const Crafting&) = delete;

    const std::vector<Recipe>& getRecipes() const { return recipes; }
    bool canCraft(const Recipe& recipe, const Inventory& inv) const;
    bool craft(const Recipe& recipe, Inventory& inv) const;

    void draw(sf::RenderWindow& window, const Inventory& inv);

    bool handleClick(sf::Vector2f guiMousePos, sf::RenderWindow& window, Inventory& inv);

private:
    std::vector<Recipe> recipes;
};