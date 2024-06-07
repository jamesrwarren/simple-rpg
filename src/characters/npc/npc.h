#pragma once

#include "../character.h"
#include <cmath>
#include <iostream>

class Npc : public Character {
public:
    Npc(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type);
    bool moveToTarget(int targetX, int targetY);
    void makeSmartDecisions(int targetX, int targetY, std::shared_ptr<Terrain> (&terrainArray)[20][30], Background* background, bool recalculate);

private:
    int targetX, targetY, npcNextX, npcNextY, x, y, pathIteration;
    bool realTarget;
    Node* npcNextPath;
    Node* nextTarget;
    std::vector<Node*> newTargetPath;
};