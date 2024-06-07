#include "terrain.h"

Terrain::Terrain(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type, std::string terrainType, int cellX, int cellY)
    : Character(x, y, width, height, collisions, staticObject, type) {
        this->type = type;
        this->terrainType = terrainType;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->cellX = cellX;
        this->cellY = cellY;
    }

std::string Terrain::getTerrainType() const {
    return this->terrainType;
}

int Terrain::getWidth() const {
    return this->width;
}

int Terrain::getHeight() const {
    return this->height;
}

SDL_Rect Terrain::getRect() const {
    SDL_Rect destRect = {this->x, this->y, this->width, this->height};
    return destRect;
};