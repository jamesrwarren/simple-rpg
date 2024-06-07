// Hero.cpp
#include "hero.h"

Hero::Hero(int x, int y, int width, int height, Collisions* collisions, bool staticObject, std::string type)
    : Character(x, y, width, height, collisions, staticObject, type) {}

void Hero::handleEvent(SDL_Event& event) {
    // Use SDL_GetKeyboardState to get the current keyboard state
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    int dx = 0;
    int dy = 0;

    if (event.type == SDL_KEYDOWN) {
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            dy = -10;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            dy = 10;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            dx = -10;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            dx = 10;
        }

        // Move in both x and y directions
        move(dx, dy);
    }
}
