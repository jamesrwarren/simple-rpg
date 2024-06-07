#pragma once

#include "../character.h"
#include <iostream>

class Terrain : public Character {
public:
    Terrain(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type, std::string terrainType, int cellX, int cellY);
    std::string getTerrainType() const;
    SDL_Rect getRect() const;
    int getHeight() const;
    int getWidth() const;
private:
    std::string type, terrainType;
    int x, y, width, height, cellX, cellY;
};
