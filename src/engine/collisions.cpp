#include "collisions.h"
#include <iostream>

// Constructor
Collisions::Collisions(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // Calculate the width and height of each partition
    partitionWidth = screenWidth / 10;
    partitionHeight = screenHeight / 10;

    // Here we create a 2d array of partitions
    for (int i = 0; i < 10; i++) {
        std::vector<Partition> row;
        for (int j = 0; j < 10; j++) {   
            Partition partition;
            partition.x = i * partitionWidth;
            partition.y = j * partitionHeight;
            partition.width = partitionWidth;
            partition.height = partitionHeight;
            row.push_back(partition);
        }
        partitionObjects.push_back(row);
    }
}

// Destructor
Collisions::~Collisions() {
    // Destructor code, if needed
}

void Collisions::addObjectToPartition(PartitionLocator partitionLocator, GameObject object) {
    // Ensure that the partition coordinates are within valid bounds
    if (partitionLocator.partitionX >= 0 && partitionLocator.partitionX < 10 
        && partitionLocator.partitionY >= 0 && partitionLocator.partitionY < 10) {
        Partition& partitionObject = partitionObjects[partitionLocator.partitionX][partitionLocator.partitionY];
        partitionObject.gameObjects.push_back(object);
    }
}

Collisions::PartitionLocator Collisions::getPartitionForPosition(int x, int y) {
    int partitionX = x / partitionWidth;
    int partitionY = y / partitionHeight;
    PartitionLocator partitionLocator;

    // Ensure that the partition coordinates are within valid bounds
    if (partitionX >= 0 && partitionX < 10 && partitionY >= 0 && partitionY < 10) {
        partitionLocator.partitionX = partitionX;
        partitionLocator.partitionY = partitionY;
        return partitionLocator;
    }

    // Return an empty partition if the coordinates are outside the valid bounds
    partitionLocator.partitionX = -1;
    partitionLocator.partitionY = -1;
    return partitionLocator;
}

std::vector<Collisions::GameObject> Collisions::getAllGameObjectsArray() {
    // Create a new vector to hold all gameObjects
    std::vector<GameObject> allGameObjects;

    for (const std::vector<Partition>& row : partitionObjects) {
        for (const Partition partition : row) {
            // Append gameObjects from the current Partition to the new vector
            allGameObjects.insert(allGameObjects.end(), partition.gameObjects.begin(), partition.gameObjects.end());
        }
    }
    return allGameObjects;
}

std::vector<Collisions::GameObject> Collisions::getImmediateGameObjectsArray(Collisions::PartitionLocator partitionLocator) {
    std::vector<GameObject> allGameObjects;

    // Calculate boundaries
    int minX = std::max(0, partitionLocator.partitionX - 1);
    int maxX = std::min(9, partitionLocator.partitionX + 1);
    int minY = std::max(0, partitionLocator.partitionY - 1);
    int maxY = std::min(9, partitionLocator.partitionY + 1);

    // append static game objects in adjacent partitions to allGameObjects
    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            Partition& partition = partitionObjects[x][y];
            allGameObjects.insert(allGameObjects.end(), partition.gameObjects.begin(), partition.gameObjects.end());
        }
    }

    // append all the non static game objects to allGameObjects
    for (size_t i = 0; i < nonStaticGameObjectStack.size(); ++i) {
        const GameObject& obj = nonStaticGameObjectStack[i];
        allGameObjects.push_back(obj);     
    }

    std::cout << "\nSIZE: " << allGameObjects.size() << std::endl;

    return allGameObjects;
}

void Collisions::cleanNonStaticGameObjectStack() {
    for (auto it = nonStaticGameObjectStack.begin(); it != nonStaticGameObjectStack.end(); ++it) {
        GameObject& gameObject = *it;

        // Check the condition you want (e.g., if the object ceases to exist)
        if (!gameObject.ptr) {
            nonStaticGameObjectStack.erase(it);
        }
    }
}


// Function to check for AABB collision
bool Collisions::checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
    // Check if two rectangles (AABBs) overlap
    if (x1 < x2 + width2 &&
        x1 + width1 > x2 &&
        y1 < y2 + height2 &&
        y1 + height1 > y2) {
        // Rectangles overlap (collision detected)
        return true;
    }
    // No collision detected
    return false;
}

// Function to check for AABB collision and return collision type
// int Collisions::checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
//     bool xCollision = (x1 < x2 + width2) && (x1 + width1 > x2);
//     bool yCollision = (y1 < y2 + height2) && (y1 + height1 > y2);

//     if (xCollision && yCollision) {
//         // Both X and Y collisions
//         return 3;
//     }
//     else if (xCollision) {
//         // X collision only
//         return 1;
//     }
//     else if (yCollision) {
//         // Y collision only
//         return 2;
//     }
//     else {
//         // No collision
//         return 0;
//     }
// }