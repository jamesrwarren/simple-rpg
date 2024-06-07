// Hero.h
#pragma once

#include "../character.h"

class Hero : public Character {
public:
    Hero(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type);
    void handleEvent(SDL_Event& event);
};