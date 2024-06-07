// Character.cpp
#include "character.h"
#include "terrain/terrain.h"
#include "../world/background.h"

Character::Character(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type)
    : x(x), y(y), width(width), height(height), collisions(collisions), staticObject(staticObject), type(type) {
        this->collided = false;
        if (!staticObject) {
            std::shared_ptr<Character> self_shared_ptr = std::shared_ptr<Character>(this);
            Collisions::GameObject nonStaticGameObject = { x, y, width, height, self_shared_ptr };
            collisions->nonStaticGameObjectStack.push_back(nonStaticGameObject);
        }
    }


bool Character::move(int dx, int dy) {
    int new_x = x + dx;
    int new_y = y + dy;

    std::cout << "-- xy modifier: " << dx << "," << dy << std::endl; 

    std::vector<Collisions::GameObject> gameObjectsArray = collisions->getImmediateGameObjectsArray(collisions->getPartitionForPosition(x, y));

    characterCollisions.clear();
    this->collided = false;
    for (size_t i = 0; i < gameObjectsArray.size(); i++) {  
        if (gameObjectsArray[i].ptr.get() == this) {
            continue;
        }
        if (collisions->checkCollision(
                gameObjectsArray[i].x, gameObjectsArray[i].y, gameObjectsArray[i].width, gameObjectsArray[i].height,
                new_x, new_y, width, height)
            )
        {    
            this->collided = true;
            if (gameObjectsArray[i].ptr->type == "npc") {
                characterCollisions.push_back(gameObjectsArray[i].ptr);
            }
        }
    }

    if (!this->collided) {
        x = new_x;
        y = new_y;
    }

    return this->collided;
}

void Character::getCharacterCollisions() {
    for (size_t i = 0; i < characterCollisions.size(); i++) { 
        std::cout << "Object Type: " << type << " collided with object type: " << characterCollisions[i]->type << std::endl;
    }
}

bool Character::moveNPC(int dx, int dy) {
    int new_x = x + dx;
    int new_y = y + dy;

    std::vector<Collisions::GameObject> gameObjectsArray = collisions->getImmediateGameObjectsArray(collisions->getPartitionForPosition(x, y));
    
    characterCollisions.clear();
    this->collided = false;
    std::cout << "\nSIZE In Move NPC: " << gameObjectsArray.size() << std::endl;
    for (size_t i = 0; i < gameObjectsArray.size(); i++) {  
        if (gameObjectsArray[i].ptr.get() == this) {
            continue;
        }
        if (collisions->checkCollision(
                gameObjectsArray[i].x, gameObjectsArray[i].y, gameObjectsArray[i].width, gameObjectsArray[i].height,
                new_x, new_y, width, height)
            )
        {    
            this->collided = true;
            if (gameObjectsArray[i].ptr->type == "player") {
                characterCollisions.push_back(gameObjectsArray[i].ptr);
            }
        }
    }

    bool xCollided = false;
    bool yCollided = false;
    if (this->collided) {
        for (size_t i = 0; i < gameObjectsArray.size(); i++) { 
            if (gameObjectsArray[i].ptr.get() == this) {
                continue;
            } 
            if (collisions->checkCollision(
                    gameObjectsArray[i].x, gameObjectsArray[i].y, gameObjectsArray[i].width, gameObjectsArray[i].height,
                    new_x, y, width, height)
                )
            {    
                xCollided = true;
            }
        }

        for (size_t i = 0; i < gameObjectsArray.size(); i++) {  
            if (gameObjectsArray[i].ptr.get() == this) {
                continue;
            }
            if (collisions->checkCollision(
                    gameObjectsArray[i].x, gameObjectsArray[i].y, gameObjectsArray[i].width, gameObjectsArray[i].height,
                    x, new_y, width, height)
                )
            {    
                yCollided = true;
            }
        }

        if (!xCollided) {
            x = new_x;
        } else if (!yCollided) {
            y = new_y;
        }

    } 
    else 
    {
        x = new_x;
        y = new_y;
    }

    std::cout << "-- xy modifier: " << dx << "," << dy << std::endl; 

    return this->collided;
}

void Character::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect characterRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &characterRect);
}

int Character::getX() {
    return x;
}

int Character::getY() {
    return y;
}

int Character::getWidth() {
    return width;
}

int Character::getHeight() {
    return height;
}

std::vector<Node*> Character::getPathToTarget() {
    return pathToTarget;
}

void Character::setPathToTarget(std::vector<Node*> ptt) {
    this->pathToTarget = ptt;
}

// Calculate Manhattan distance heuristic
double Character::calculateHeuristic(int x, int y, int targetX, int targetY) {
    return std::abs(x - targetX) + std::abs(y - targetY);
}

std::vector<std::vector<Node>> Character::createGrid(std::shared_ptr<Terrain> (&terrainArray)[20][30]) {
    std::vector<std::vector<Node>> grid;

    for (int row = 0; row < 20; ++row) {
        std::vector<Node> rowNodes;
        for (int col = 0; col < 30; ++col) {
            std::string terrainType = terrainArray[row][col]->getTerrainType();
            bool blocker = false;
            if (terrainType == "dirt") {
                blocker = true;
            }
            int gridPixelX = col * terrainArray[row][col]->getWidth();
            int gridPixelY = row * terrainArray[row][col]->getHeight();
            Node node(col, row, gridPixelX, gridPixelY, blocker); // Create a Node object initialized with Terrain values
            rowNodes.push_back(node); // Add the Node to the current row
        }
        grid.push_back(rowNodes); // Add the row to the grid
    }

    return grid;
}

// A* pathfinding function
std::vector<Node*> Character::findPath(std::vector<std::vector<Node>>& grid, int startX, int startY, int targetX, int targetY, Background* background) {
    int numRows = grid.size();
    int numCols = grid[0].size();

    int centreCharX = startX + width / 2;
    int centreCharY = startY + height / 2;
    int centreTargetX = targetX + width / 2;
    int centreTargetY = targetY + height / 2;

    GridLocator startGridLocation = getGridLocation(startX, startY, background);
    GridLocator targetGridLocation = getGridLocation(centreTargetX, centreTargetY, background);

    std::cout << "Grid Rows: " << numRows << "Grid Cols: " << numCols << " ---- ";
    std::cout << "\nStart X: " << startGridLocation.gridX << " Start Y: " << startGridLocation.gridY << " ---- \n";
    std::cout << "\nTarget X: " << targetGridLocation.gridX << " Target Y: " << targetGridLocation.gridY << " ---- \n";

    // Check if the start and target positions are valid
    if (startGridLocation.gridX < 0 || startGridLocation.gridY < 0 || startGridLocation.gridX >= numCols || startGridLocation.gridY >= numRows ||
        targetGridLocation.gridX < 0 || targetGridLocation.gridY < 0 || targetGridLocation.gridX >= numCols || targetGridLocation.gridY >= numRows) {
        std::cout << "Positions not valid";
        return std::vector<Node*>();
    }

    struct NodeComparator {
        bool operator()(Node* a, Node* b) const {
            return a->f() > b->f();
        }
    };

    // Declare the priority queue using the comparator
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openSet;

    // Initialize start node
    Node* startNode = &grid[startGridLocation.gridY][startGridLocation.gridX];
    startNode->cost = 0;
    startNode->heuristic = calculateHeuristic(startGridLocation.gridX, startGridLocation.gridY, targetGridLocation.gridX, targetGridLocation.gridY);
    
    // To avoid it selecting itself as first path
    startNode->parent = startNode;
    openSet.push(startNode);

    while (!openSet.empty()) {
        // Get the node with the lowest f value from the open set
        Node* currentNode = openSet.top();
        openSet.pop();

        // Check if we've reached the target node
        if (currentNode->gridX == targetGridLocation.gridX && currentNode->gridY == targetGridLocation.gridY) {
            // Reconstruct the path from the target node to the start node
            std::vector<Node*> path;
            Node* current = currentNode;
            std::cout << "\nLAST NODE: " << current->gridX << ", " << current->gridY << " LAST NODE PARENT: " << current->parent->gridX << ", " << current->parent->gridY << " START NODE: " << startNode->gridX << ", " << startNode->gridY << "\n";
            while (current != nullptr) {
                if (current->gridX == startNode->gridX && current->gridY == startNode->gridY) {
                    break;
                }
                std::cout << "NTH ARRAY GRID POSITION: " << current->gridX << ", " << current->gridY << " START POSITION: " <<  startNode->gridX << ", " << startNode->gridY << "\n";
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        double tentativeCost = currentNode->cost + 1; // Assuming each step has a cost of 1

        // Generate neighboring nodes. Path finding with diagonals
        int minX = std::max(0, currentNode->gridX - 1);
        int maxX = std::min(numCols, currentNode->gridX + 1);
        int minY = std::max(0, currentNode->gridY - 1);
        int maxY = std::min(numRows, currentNode->gridY + 1);

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                // Get rid of neighboring diagonals
                // if (x != currentNode->gridX && y != currentNode->gridY) {
                //     continue;
                // }

                if (currentNode->blocker) {
                    continue;
                }

                int nextX = x;
                int nextY = y;

                if (nextX >= 0 && nextX < numCols && nextY >= 0 && nextY < numRows) {
                    // carry on
                } else {
                    continue;
                }
                
                Node* neighbor = &grid[nextY][nextX];


                // std::cout << "\nXY: " << nextX << ", " << nextY << " ---- tent G: " << tentativeCost << " neighbour G: " << neighbor->cost << "\n";

                // Check if the neighbor node is not in the closed set or has a lower g value
                if (tentativeCost < neighbor->cost || !neighbor->parent) {    
                    neighbor->cost = tentativeCost;
                    neighbor->heuristic = calculateHeuristic(nextX, nextY, targetGridLocation.gridX, targetGridLocation.gridY);
                    neighbor->parent = currentNode;

                    // Add the neighbor to the open set
                    openSet.push(neighbor);
                }
            }
        }
    }

    // No path found
    return std::vector<Node*>();
}

Character::GridLocator Character::getGridLocation(int x, int y, Background* background) {
    int gridX = x / background->getTileWidth();
    int gridY = y / background->getTileHeight();
    GridLocator gridLocator;

    // Ensure that the partition coordinates are within valid bounds
    if (gridX >= 0 && gridX < background->getMapColumns() && gridY >= 0 && gridY < background->getMapRows()) {
        gridLocator.gridX = gridX;
        gridLocator.gridY = gridY;
        return gridLocator;
    }

    // Return an empty partition if the coordinates are outside the valid bounds
    gridLocator.gridX = -1;
    gridLocator.gridY = -1;

    return gridLocator;
}
