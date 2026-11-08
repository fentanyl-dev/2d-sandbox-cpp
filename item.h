#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

enum ItemID : int
{
    none = 0,
    woodenSword = 1,
    woodenShovel = 2,
    woodenAxe = 3,
    woodenPickaxe = 4,
    stoneSword = 5,
    stoneShovel = 6,
    stoneAxe = 7,
    stonePickaxe = 8,
    ironSword = 9,
    ironShovel = 10,
    ironAxe = 11,
    ironPickaxe = 12,
    ametystSword = 13,
    ametystShovel = 14,
    ametystAxe = 15,
    ametystPickaxe = 16,
    stick = 17,
    wood = 18,
    cake = 19,
    diamondSword = 20,
    diamondShovel = 21,
    diamondAxe = 22,
    diamondPickaxe = 23,
    obsidianSword = 24,
    obsidianShovel = 25,
    obisdianAxe = 26,
    obsidianPickaxe = 27,
    coal = 28,
    diamond = 29,
    cobble = 30,
    iron_clod = 31,
    iron_ingot = 32,
    ametyst_ignot = 33,
    workbench = 34,
    oak_plank = 35,

    //Bloki 
    dirt = 102,
    tile_7 = 107,
    tile_8 = 108,
    stone = 109,
    tile_10 = 110,
    tile_11 = 111,
    tile_12 = 112,
};

struct ItemDef
{
    std::string name;
    int maxStack = 64;
    const sf::Texture* texture = nullptr;
    bool isTile = false;
    sf::IntRect tileRect;
};

class ItemDatabase
{
    public: 
        static void innit();
        static const ItemDef* get(int id);
    private:
        static void addTextureItem(int id, const std::string& name, const std::string& path, int maxStack);
        static void addTileItem(int id, const std::string& name, sf::IntRect rect, int maxStack = 64);

        static std::unordered_map<int, ItemDef> items;
        static std::unordered_map<int, sf::Texture> textures;
        static sf::Texture terrain;

};