#pragma once

#include <vector> 
#include "../characters/character.h"

class Character;

class Collisions {
public:
    // Constructor and Destructor
    Collisions(int screenWidth, int screenHeight);
    ~Collisions();

    struct GameObject {
        int x;
        int y;
        int width;
        int height;
        std::shared_ptr<Character> ptr;
    };

    struct Partition {
        int x;
        int y;
        int width;
        int height;
        std::vector<GameObject> gameObjects;
    };

    struct PartitionLocator {
        int partitionX;
        int partitionY;
    };

    PartitionLocator getPartitionForPosition(int x, int y);

    void addObjectToPartition(PartitionLocator partitionLocator, GameObject object);

    std::vector<std::vector<Partition> > partitionObjects;

    std::vector<GameObject> nonStaticGameObjectStack;
    
    void cleanNonStaticGameObjectStack();

    std::vector<GameObject> getAllGameObjectsArray();

    std::vector<GameObject> getImmediateGameObjectsArray(PartitionLocator partitionLocator);

    bool checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
    
private:
    int screenWidth;
    int screenHeight;
    int partitionWidth;
    int partitionHeight;
    // Data members or data structures for optimization
    // For example, you can use spatial data structures like quad trees or grids for faster collision checks.
};