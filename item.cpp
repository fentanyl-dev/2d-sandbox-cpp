#include "item.h"
#include <iostream>

std::unordered_map<int, ItemDef> ItemDatabase::items;
std::unordered_map<int, sf::Texture*> ItemDatabase::textures;
sf::Texture* ItemDatabase::terrain = nullptr;

void ItemDatabase::addTextureItem(int id, const std::string& name, const std::string& path, int maxStack)
{
    ItemDef def;
    def.name = name;
    def.maxStack = maxStack;

    sf::Texture* tex = new sf::Texture();
    if (tex->loadFromFile(path))
    {
        tex->setSmooth(false);
        def.texture = tex;
        textures[id] = tex;
    }
    else 
    {
        delete tex;
        std::cout << "Nie udalo sie wczytac tekstury itemu" << path << std::endl;
    }
    items[id] = def;
}

void ItemDatabase::addTileItem(int id, const std::string& name, sf::IntRect rect, int maxStack)
{
    ItemDef def;
    def.name = name;
    def.maxStack = maxStack;
    def.texture = terrain;
    def.isTile = true;
    def.tileRect = rect;
    items[id] = def;
    def.type = ItemType::Block;
}

void ItemDatabase::innit()
{
    terrain = new sf::Texture();
    if (!terrain->loadFromFile("assets/world/Terrain.png"))
    {
        std::cout << "Nie udalo sie wczytac pliku z teksturami Terenu" << std::endl;
    }
    terrain->setSmooth(false);

    addTextureItem(stoneShovel, "Kamienna Lopata", "assets/items/stone_shovel.png", 1);
    addTextureItem(stonePickaxe, "Kamienny Kilof", "assets/items/stone_pickaxe.png", 1);
    addTextureItem(stoneSword, "Kamienny Miecz", "assets/items/stone_sword.png", 1);
    addTextureItem(stoneAxe, "Kamienna Siekiera", "assets/items/stone_axe.png", 1);

    addTextureItem(diamondSword, "Diamentowy Miecz", "assets/items/sword_diamond.png", 1);
    addTextureItem(diamond, "Diament", "assets/items/diamond.png", 64);

    addTextureItem(ametystSword, "Ametystowy Miecz", "assets/items/sword_amethyst.png", 1);
    addTextureItem(ametyst_ignot, "Sztabka Ametystu", "assets/items/amethyst_ingot.png", 64);

    addTextureItem(ironSword, "Zelazny miecz", "assets/items/sword_iron.png", 1);
    addTextureItem(iron_clod, "Ruda zelaza", "assets/items/iron_clod.png", 64);
    addTextureItem(iron_ingot, "Szatbka zelaza", "assets/items/iron_ingot.png", 64);

    addTextureItem(obsidianSword, "Obsydianowy Miecz", "assets/items/sword_obsidian.png", 1);

    addTextureItem(woodenAxe, "Drewniana Siekiera", "assets/items/wooden_axe.png", 1);
    addTextureItem(woodenPickaxe, "Drewniany Kilof", "assets/items/wooden_pickaxe.png", 1);
    addTextureItem(woodenShovel, "Drewniana Lopata", "assets/items/wooden_shovel.png", 1);
    
    addTextureItem(cobble, "Kamien", "assets/items/cobble.png", 64);
    addTextureItem(cake, "Ciastko", "assets/items/cake.png", 16);
    addTextureItem(stick, "Patyk", "assets/items/stick.png", 64);
    addTextureItem(workbench, "Crafting", "assets/items/crafting_table.png", 64);
    addTextureItem(oak_plank, "Drewniana deska", "assets/items/oak_plank.png", 64);
    //Bloki

    addTileItem(dirt,    "Ziemia",  sf::IntRect({16, 16}, {16, 16}));
    addTileItem(tile_7,  "Blok 7",  sf::IntRect({64, 0},  {16, 16}));
    addTileItem(tile_8,  "Blok 8",  sf::IntRect({0, 48},  {16, 16}));
    addTileItem(stone,   "Kamien",  sf::IntRect({16, 48}, {16, 16}));
    addTileItem(tile_10, "Blok 10", sf::IntRect({0, 32},  {16, 16}));
    addTileItem(tile_11, "Blok 11", sf::IntRect({16, 32}, {16, 16}));
    addTileItem(tile_12, "Blok 12", sf::IntRect({32, 32}, {16, 16}));

    //Staty
    setStats(stoneSword, ItemType::Weapon, 2.f);
    setStats(ironSword, ItemType::Weapon, 3.f);
    setStats(ametystSword, ItemType::Weapon, 4.f);
    setStats(diamondSword, ItemType::Weapon, 5.f);
    setStats(obsidianSword, ItemType::Weapon, 6.f);

    setStats(woodenPickaxe, ItemType::Pickaxe, 1.f);
    setStats(stonePickaxe, ItemType::Pickaxe, 2.f);

    setStats(cake, ItemType::Food, 20.f);
}

const ItemDef* ItemDatabase::get(int id)
{
    auto it = items.find(id);
    return it != items.end() ? &it->second : nullptr;
}

void ItemDatabase::shutDown()
{
    items.clear();

    for (auto& entry : textures)
        delete entry.second;
    textures.clear();

    delete terrain;
    terrain = nullptr;
}

void ItemDatabase::setStats(int id, ItemType type, float power)
{
    auto it = items.find(id);
    if (it == items.end()) return;
    it->second.type = type;
    it->second.power = power;
}