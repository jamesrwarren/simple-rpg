#include <SDL2/SDL.h>
#include <vector> 
#include <iostream>
#include <thread>
#include <chrono>
#include "characters/character.h"
#include "characters/player/hero.h"
#include "characters/npc/npc.h"
#include "world/background.h"
#include "engine/collisions.h"
#include <mach/mach.h> // Include the macOS-specific header for task_info

struct task_basic_info info;
mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;

void memoryCounter() {
        // Query memory statistics for the current process
        if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
            std::cerr << "Failed to query memory statistics." << std::endl;
        }

        // Calculate memory usage in megabytes
        double memoryUsageMB = static_cast<double>(info.resident_size) / (1024.0 * 1024.0);

        // Print memory usage
        std::cout << "Memory usage: " << memoryUsageMB << " MB" << std::endl;
}

// CONSTANT VARIABLE SETUP
const int screenWidth = 1200;
const int screenHeight = 800;
const int characterSize = 30;
const int mapRows = 20;
const int mapColumns = 30;
const int targetFPS = 60;
const Uint32 frameDuration = 1000 / targetFPS;

// ===== MAIN GAME =====
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // ===== SETUP ALL THE GAME ELEMENTS =====
    SDL_Window* window = SDL_CreateWindow("Basic SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Collisions collisionHandler(screenWidth, screenHeight);
    Background background(renderer, mapRows, mapColumns, screenHeight, screenWidth, &collisionHandler);
    Hero hero(screenWidth / 2 - characterSize / 2, screenHeight / 2 - characterSize / 2, characterSize, characterSize, &collisionHandler, false, "player");
    Npc npc(screenWidth / 4 - characterSize / 2, screenHeight / 4 - characterSize / 2, characterSize, characterSize, &collisionHandler, false, "npc");
    
    npc.makeSmartDecisions(hero.getX(), hero.getY(), background.displayBackgroundObjectStack, &background, true);

    // SETUP FRAME RATES
    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();
    Uint32 nextFrameTime = startTime + frameDuration;
    
    // ===== MAIN GAME LOOP =====
    bool quit = false;
    while (!quit) {
        
        // ===== TIME FUNCTIONS AND FRAME RATE HANDLING =====
        frameCount++;
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;
        bool recalculate = false;
        if (elapsedTime >= 1000) {
            float fps = static_cast<float>(frameCount) / (elapsedTime / 1000.0f);
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            startTime = currentTime;
            recalculate = true;
        }
        if (currentTime < nextFrameTime) {
            Uint32 delayTime = nextFrameTime - currentTime;
            SDL_Delay(delayTime);
        }
        nextFrameTime += frameDuration;

        // ===== GAME CALCULATIONS GO IN THIS BLOCK =====
        collisionHandler.cleanNonStaticGameObjectStack();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            hero.handleEvent(event);
        }

        npc.makeSmartDecisions(hero.getX(), hero.getY(), background.displayBackgroundObjectStack, &background, recalculate);
        npc.getCharacterCollisions();
        hero.getCharacterCollisions();

        

        // ===== RENDERING GOES IN THIS BLOCK =====

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        background.render(renderer); // Render the background image
        hero.render(renderer);
        npc.render(renderer);

        SDL_RenderPresent(renderer);    
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}