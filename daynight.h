#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <cstdint>

class DayNight
{
public:
    float dayLength = 240.f;

    void update(float dt)
    {
        time += dt / dayLength;
        if (time >= 1.f) time -= 1.f;
    }

    bool isNight() const { return time < 0.2f || time > 0.8f; }

    void setTime(float t)
    {
        time = std::fmod(t, 1.f);
        if (time < 0.f) time += 1.f;
    }

    float getTime() const { return time; }

    sf::Color ambient() const
    {
        struct Key { float t; sf::Color c; };
        static const std::array<Key, 7> keys = {{
            {0.00f, sf::Color(50, 60, 120)},   
            {0.20f, sf::Color(60, 70, 130)},    
            {0.27f, sf::Color(255, 180, 140)},  
            {0.35f, sf::Color(255, 255, 255)},  
            {0.65f, sf::Color(255, 255, 255)},
            {0.75f, sf::Color(255, 150, 110)},  
            {0.85f, sf::Color(50, 60, 120)},
        }};

        for (size_t i = 0; i < keys.size(); i++)
        {
            const Key& a = keys[i];
            const Key& b = keys[(i + 1) % keys.size()];
            float bt = (i + 1 < keys.size()) ? b.t : 1.f;
            if (time >= a.t && time <= bt)
            {
                float k = (time - a.t) / (bt - a.t);
                return lerp(a.c, b.c, k);
            }
        }
        return sf::Color::White;
    }

    void draw(sf::RenderWindow& window) const
    {
        sf::RectangleShape overlay(window.getDefaultView().getSize());
        overlay.setFillColor(ambient());

        sf::RenderStates states;
        states.blendMode = sf::BlendMultiply;
        window.draw(overlay, states);
    }

private:
    float time = 0.3f;   

    static sf::Color lerp(sf::Color a, sf::Color b, float k)
    {
        return sf::Color(
            static_cast<std::uint8_t>(a.r + (b.r - a.r) * k),
            static_cast<std::uint8_t>(a.g + (b.g - a.g) * k),
            static_cast<std::uint8_t>(a.b + (b.b - a.b) * k)
        );
    }
};