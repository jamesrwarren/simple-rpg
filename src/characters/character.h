// Character.h
#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "../engine/collisions.h"

class Collisions; // Forward declaration
class Terrain;
class Background;

struct Node {
    int gridX, gridY, x, y; // Node position
    bool blocker;
    double cost; // Cost from the start node to this node
    double heuristic; // Heuristic (estimated cost to the goal)
    Node* parent; // Parent node in the path

    Node(int gridX, int gridY, int x, int y, bool blocker) : gridX(gridX), gridY(gridY), x(x), y(y), blocker(blocker), cost(0), heuristic(0), parent(nullptr) {}

    // Total cost (cost + h)
    double f() const {
        return cost + heuristic;
    }
};

class Character {
public:
    Character(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type);
    struct GridLocator {
        int gridX;
        int gridY;
    };
    GridLocator getGridLocation(int x, int y, Background* background);
    bool move(int dx, int dy);
    bool moveNPC(int dx, int dy);
    void render(SDL_Renderer* renderer);
    bool collided;
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    std::vector<std::vector<Node>> createGrid(std::shared_ptr<Terrain> (&terrainArray)[20][30]);
    std::vector<Node*> findPath(std::vector<std::vector<Node>>& grid, int startX, int startY, int targetX, int targetY, Background* background);
    std::vector<Node*> pathToTarget;
    std::vector<Node*> getPathToTarget();
    void setPathToTarget(std::vector<Node*> ptt);
    std::vector<std::shared_ptr<Character>> characterCollisions;
    void getCharacterCollisions();
private:
    int x, y, width, height;
    bool staticObject;
    std::string type;
    Collisions* collisions; // Reference 
    double calculateHeuristic(int x, int y, int targetX, int targetY); // Calculate Manhattan distance heuristic
};
