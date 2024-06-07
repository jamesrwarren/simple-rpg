#include "background.h"
#include <fstream>
#include <iostream>

Background::Background(SDL_Renderer* renderer, int mapRows, int mapColumns, int screenHeight, int screenWidth, Collisions* collisions) {
    grassTexture = IMG_LoadTexture(renderer, "world/grass.png");
    dirtTexture = IMG_LoadTexture(renderer, "world/dirt.jpeg");
    const char* tileMapPath = "world/level1.txt";
    bgMapRows = mapRows;
    bgMapColumns = mapColumns;
    tileHeight = screenHeight / bgMapRows;
    tileWidth = screenWidth / bgMapColumns;

    // Load tile map from file
    std::ifstream file(tileMapPath);
    if (file.is_open()) {
        for (int row = 0; row < bgMapRows; ++row) {
            for (int col = 0; col < bgMapColumns; ++col) {
                file >> tileMap[row][col];
                // Create instance of Terrain and give a shared pointer to it
                int tileX = col * tileWidth;
                int tileY = row * tileHeight;
                if (tileMap[row][col] == 'o') {
                    // Create instance of Terrain and give a shared pointer to it
                    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(tileX, tileY, tileWidth, tileHeight, collisions, true, "terrain", "dirt", row, col);
                    Collisions::GameObject newTileObject = { tileX, tileY, tileWidth, tileHeight, terrain };
                    collisions->addObjectToPartition(collisions->getPartitionForPosition(tileX, tileY), newTileObject);
                    displayBackgroundObjectStack[row][col] = terrain;
                } else {
                    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(tileX, tileY, tileWidth, tileHeight, collisions, true, "terrain", "grass", row, col);
                    displayBackgroundObjectStack[row][col] = terrain;
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open tile map file." << std::endl;
    }
}

Background::~Background() {
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(dirtTexture);
}

void Background::render(SDL_Renderer* renderer) {
    for (int row = 0; row < bgMapRows; ++row) {
        for (int col = 0; col < bgMapColumns; ++col) {
            std::shared_ptr<Terrain> terrainObject = displayBackgroundObjectStack[row][col];
            SDL_Rect destRect = terrainObject->getRect();
            // Cache the terrain type
            std::string terrainType = terrainObject->getTerrainType();

            // Use a switch statement for terrain types
            SDL_Texture* textureToRender = nullptr;
            if (terrainType == "grass") {
                textureToRender = grassTexture;
            } else if (terrainType == "dirt") {
                textureToRender = dirtTexture;
            }
            
            // Render the texture
            if (textureToRender) {
                SDL_RenderCopy(renderer, textureToRender, nullptr, &destRect);
            }
        }
    }
}

int Background::getTileHeight() {
    return tileHeight;
};
int Background::getTileWidth() {
    return tileWidth;
};
int Background::getMapRows() {
    return bgMapRows;
};
int Background::getMapColumns() {
    return bgMapColumns;
};

// DEAD CODE------

// std::cout << "-- TYPE OBJ --[" << terrainObject->getType() << "]--" << std::endl;


// void Background::render(SDL_Renderer* renderer) {
//     for (int row = 0; row < bgMapRows; ++row) {
//         for (int col = 0; col < bgMapColumns; ++col) {
            
//             SDL_Rect destRect = {col * tileWidth, row * tileHeight, tileWidth, tileHeight};
            
//             if (tileMap[row][col] == 'w') {
//                 SDL_RenderCopy(renderer, grassTexture, nullptr, &destRect);
//             } else if (tileMap[row][col] == 'o') {
//                 SDL_RenderCopy(renderer, dirtTexture, nullptr, &destRect);
//             }
//         }
//     }
// }