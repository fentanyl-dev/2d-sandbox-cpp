#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <queue>
#include <vector>
#include "map.h"

// Oswietlenie kafelkowe: swiatlo nieba + pochodnie + lekka poswiata gracza.
// Rysowane jako nakladka BlendMultiply w widoku gry (zastepuje DayNight::draw).
class Lighting
{
public:
    static constexpr int   MAX_LIGHT        = 15;
    static constexpr int   TORCH_LIGHT      = 12;   // pochodnia na ziemi
    static constexpr int   HELD_TORCH_LIGHT = 12;   // pochodnia w rece
    static constexpr int   PLAYER_GLOW      = 3;    // poswiata gracza bez pochodni
    static constexpr float TILE             = 16.f;

    Lighting() : lightSprite(lightTexture) {}

    Lighting(const Lighting&) = delete;
    Lighting& operator=(const Lighting&) = delete;

    // Przelicza swiatlo tylko gdy cos sie zmienilo (mapa, kafel gracza, pochodnia w rece)
    void update(const map& m, sf::Vector2f playerPos, bool holdingTorch)
    {
        sf::Vector2i pt(static_cast<int>(std::floor(playerPos.x / TILE)),
                        static_cast<int>(std::floor(playerPos.y / TILE)));

        if (m.getVersion() == lastVersion && pt == lastPlayerTile && holdingTorch == lastHolding)
            return;

        lastVersion    = m.getVersion();
        lastPlayerTile = pt;
        lastHolding    = holdingTorch;

        recompute(m, pt, holdingTorch);
    }

    void draw(sf::RenderWindow& window, const sf::View& view, sf::Color ambient)
    {
        if (w == 0 || h == 0) return;

        sf::Vector2f c = view.getCenter();
        sf::Vector2f s = view.getSize();

        int x0 = std::max(0,     static_cast<int>(std::floor((c.x - s.x / 2.f) / TILE)) - 1);
        int y0 = std::max(0,     static_cast<int>(std::floor((c.y - s.y / 2.f) / TILE)) - 1);
        int x1 = std::min(w - 1, static_cast<int>(std::floor((c.x + s.x / 2.f) / TILE)) + 1);
        int y1 = std::min(h - 1, static_cast<int>(std::floor((c.y + s.y / 2.f) / TILE)) + 1);
        if (x1 < x0 || y1 < y0) return;

        sf::Vector2u sz(static_cast<unsigned>(x1 - x0 + 1), static_cast<unsigned>(y1 - y0 + 1));

        if (lightTexture.getSize() != sz)
        {
            if (!lightTexture.resize(sz)) return;
            lightTexture.setSmooth(true);          // wygladzanie = miekkie przejscia
            lightSprite.setTexture(lightTexture, true);
        }

        sf::Image img(sz, sf::Color::Black);
        for (int y = y0; y <= y1; y++)
        {
            for (int x = x0; x <= x1; x++)
            {
                int i = y * w + x;
                img.setPixel({static_cast<unsigned>(x - x0), static_cast<unsigned>(y - y0)},
                             tileColor(sky[i], blk[i], ambient));
            }
        }
        lightTexture.update(img);

        lightSprite.setPosition({x0 * TILE, y0 * TILE});
        lightSprite.setScale({TILE, TILE});

        sf::RenderStates states;
        states.blendMode = sf::BlendMultiply;
        window.draw(lightSprite, states);
    }

private:
    int w = 0, h = 0;
    std::vector<std::uint8_t> sky;   // swiatlo nieba
    std::vector<std::uint8_t> blk;   // swiatlo pochodni / gracza

    unsigned     lastVersion = 0xFFFFFFFFu;
    sf::Vector2i lastPlayerTile{-9999, -9999};
    bool         lastHolding = false;

    sf::Texture lightTexture;
    sf::Sprite  lightSprite;

    static std::uint8_t mix(std::uint8_t a, std::uint8_t b, float t)
    {
        return static_cast<std::uint8_t>(a + (b - a) * t);
    }

    static float curve(int level)
    {
        return std::pow(static_cast<float>(level) / MAX_LIGHT, 1.5f);
    }

    static sf::Color tileColor(int s, int b, sf::Color ambient)
    {
        const sf::Color dark(6, 6, 12);          // calkowita ciemnosc
        const sf::Color torch(255, 205, 130);    // cieple swiatlo pochodni

        float fs = curve(s);
        float fb = curve(b);

        sf::Color sc(mix(dark.r, ambient.r, fs), mix(dark.g, ambient.g, fs), mix(dark.b, ambient.b, fs));
        sf::Color bc(mix(dark.r, torch.r,   fb), mix(dark.g, torch.g,   fb), mix(dark.b, torch.b,   fb));

        return sf::Color(std::max(sc.r, bc.r), std::max(sc.g, bc.g), std::max(sc.b, bc.b));
    }

    void propagate(const map& m, std::vector<std::uint8_t>& L, std::queue<int>& q)
    {
        static const int dx[4] = {1, -1, 0, 0};
        static const int dy[4] = {0, 0, 1, -1};

        while (!q.empty())
        {
            int i = q.front();
            q.pop();

            int cur = L[i];
            if (cur <= 1) continue;

            int x = i % w;
            int y = i / w;

            for (int k = 0; k < 4; k++)
            {
                int nx = x + dx[k];
                int ny = y + dy[k];
                if (nx < 0 || ny < 0 || nx >= w || ny >= h) continue;

                int ni   = ny * w + nx;
                int cost = map::isSolidTile(m.tileAt(nx, ny)) ? 3 : 1;
                int nl   = cur - cost;

                if (nl > L[ni])
                {
                    L[ni] = static_cast<std::uint8_t>(nl);
                    q.push(ni);
                }
            }
        }
    }

    void recompute(const map& m, sf::Vector2i playerTile, bool holdingTorch)
    {
        w = m.getTilesX();
        h = m.getTilesY();
        if (w == 0 || h == 0) return;

        sky.assign(static_cast<size_t>(w) * h, 0);
        blk.assign(static_cast<size_t>(w) * h, 0);

        // 1) swiatlo nieba: kolumna jest jasna od gory do pierwszego litego kafla
        std::queue<int> q;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                if (map::isSolidTile(m.tileAt(x, y))) break;
                sky[y * w + x] = MAX_LIGHT;
                q.push(y * w + x);
            }
        }
        propagate(m, sky, q);

        // 2) pochodnie na mapie
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                if (m.tileAt(x, y) == map::TORCH_TILE)
                {
                    blk[y * w + x] = TORCH_LIGHT;
                    q.push(y * w + x);
                }
            }
        }

        // 3) gracz: pochodnia w rece albo slaba poswiata
        if (playerTile.x >= 0 && playerTile.y >= 0 && playerTile.x < w && playerTile.y < h)
        {
            int i = playerTile.y * w + playerTile.x;
            int level = holdingTorch ? HELD_TORCH_LIGHT : PLAYER_GLOW;
            if (level > blk[i])
            {
                blk[i] = static_cast<std::uint8_t>(level);
                q.push(i);
            }
        }

        propagate(m, blk, q);
    }
};