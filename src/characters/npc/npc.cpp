#include "npc.h"
#include "../terrain/terrain.h"
#include "../../world/background.h"

Npc::Npc(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type)
    : Character(x, y, width, height, collisions, staticObject, type) {
        pathIteration = 0;
    }

bool Npc::moveToTarget(int targetX, int targetY) {
    // Calculate the direction vector from current position to the target
    int deltaX = targetX - getX();
    int deltaY = targetY - getY();

    double distanceToTarget = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    // std::cout << "\nDistance to target: " << distanceToTarget << std::endl;
    if (distanceToTarget <= 0.0) {
        return true;
    }

    // Calculate the normalized direction vector
    int modifierX;
    int modifierY;
    if (deltaX > 0) {
        modifierX = 1;  // Positive
    } else if (deltaX < 0) {
        modifierX = -1; // Negative
    } else {
        modifierX = 0;  // Zero
    }

    if (deltaY > 0) {
        modifierY = 1;  // Positive
    } else if (deltaY < 0) {
        modifierY = -1; // Negative
    } else {
        modifierY = 0;  // Zero
    }

    this->collided = moveNPC(modifierX, modifierY);
    return false;
};

void Npc::makeSmartDecisions(int targetX, int targetY, std::shared_ptr<Terrain> (&terrainArray)[20][30], Background* background, bool recalculate) {
    if (recalculate) {
        pathIteration = 0;
        std::vector<std::vector<Node>> grid = createGrid(terrainArray);
        pathToTarget = findPath(grid, getX(), getY(), targetX, targetY, background);

        for (Node* nodePtr : newTargetPath) {
            delete nodePtr;  // Delete each dynamically allocated Node
        }
        newTargetPath.clear();
        // Iterate through the source vector and create deep copies
        for (Node* nodePtr : pathToTarget) {
            if (nodePtr) {
                Node* newNode = new Node(*nodePtr);  // Assuming Node has a copy constructor
                newTargetPath.push_back(newNode);
            }
        }
    }

    // std::cout << "-- current XY: " << getX() << "," << getY() << std::endl;
    // std::cout << "-- next deep copy: " << newTargetPath[pathIteration]->gridX << " " << newTargetPath[pathIteration]->gridY << " -- " << newTargetPath[pathIteration]->x << "," << newTargetPath[pathIteration]->y << std::endl;
    bool reachedTarget;
    if ((newTargetPath.size() - pathIteration) < 2) {
        realTarget = true;
        reachedTarget = moveToTarget(targetX, targetY);        
    } else
    {
        realTarget = false;
        reachedTarget = moveToTarget(newTargetPath[pathIteration]->x, newTargetPath[pathIteration]->y);
    }
    
    if (reachedTarget and !realTarget) {
        pathIteration++;
        // std::cout << "-- PATH ITER: --" << pathIteration << std::endl;
    } 
}
   