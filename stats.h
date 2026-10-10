#pragma once

#include <algorithm>
#include <iostream>
using namespace std;

class stats
{
    public:
        float health = 100;
        float maxHealth = 100;
        float mana = 100;
        float maxMana = 100;
        float manaRegen = 5;

        void takeDamage(float amount) {health = max(0.f, health - amount); }
        void heal(float amount) {health = min(maxHealth, health + amount); }

        bool useMana(float cost)
        {
            if (mana < cost) return false;
            mana -= cost;
            return true;
        }
        
        void update(float dt) { mana = min(maxMana, mana + manaRegen * dt); }
        bool isDead() const {return health <= 0.f; }
};