# RPG GAME

A small 2D side-scrolling RPG prototype written in C++17 with SFML 3. It features a procedurally generated tile world, a controllable player, enemies, item drops, an inventory, and a basic crafting interface.

## Features

- Perlin/OpenSimplex-based terrain generation using the bundled `FastNoiseLite.h`.
- Player movement, jumping, gravity, idle/walk/attack animations, and collision with terrain.
- Enemies that spawn around the player, chase and damage the player, and can be attacked.
- Breakable terrain blocks that drop items, which the player can pick up.
- Health and mana HUD, inventory, item stacking, and crafting recipes.
- Sprite and UI assets stored in `assets/`.

## Controls

| Input | Action |
| --- | --- |
| A / Left Arrow | Move left |
| D / Right Arrow | Move right |
| Space | Jump |
| Left Mouse Button | Attack; when the inventory is open, interact with the UI |
| Right Mouse Button | Break a nearby terrain block; when the inventory is open, split an item stack |
| I | Open or close the inventory |
| 1–4 | Select a hotbar slot |

## Requirements

- macOS with Clang and the Xcode Command Line Tools.
- SFML 3 (the checked-in VS Code build configuration currently points to Homebrew SFML 3.0.2 under `/opt/homebrew/Cellar/sfml/3.0.2`).

## Build and run

Open the repository folder in VS Code and run the **Build SFML Project** task (`Cmd+Shift+B`). The task compiles the C++ source files into `main`.

Run the game from the repository root so its relative `assets/...` paths resolve:

```sh
./main
```

If SFML is installed at a different location, update the include and library paths in `.vscode/tasks.json` and `.vscode/c_cpp_properties.json`.

## Source layout

- `main.cpp` — game window, main loop, input, HUD, camera, and entity orchestration.
- `map.*` — terrain loading/generation, rendering, collision, block breaking, and item pickup.
- `player.*`, `enemy.*`, `stats.*` — player, enemy, and player-stat logic.
- `inventory.*`, `item.*`, `crafting.*`, `ui_common.*` — item definitions and inventory/crafting UI.
- `decoration.*` — world decoration rendering.
- `map.txt` — text map data; the current startup sequence generates the runtime map after loading it.
- `assets/` — textures, sprites, and font used by the game.
