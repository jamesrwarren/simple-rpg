#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../engine/collisions.h"
#include "../characters/terrain/terrain.h"

class Collisions; // Forward declaration

class Background {
public:
    Background(SDL_Renderer* renderer, int mapRows, int mapColumns, int screenHeight, int screenWidth, Collisions* collisions);
    ~Background();
    std::shared_ptr<Terrain> displayBackgroundObjectStack[20][30];
    void render(SDL_Renderer* renderer);
    int getTileHeight();
    int getTileWidth();
    int getMapRows();
    int getMapColumns();

private:
    SDL_Texture* grassTexture;
    SDL_Texture* dirtTexture;
    int tileHeight;
    int tileWidth;
    int bgMapRows;
    int bgMapColumns;
    char tileMap[20][30]; // Assuming your example is 20 rows by 30 columns
};
