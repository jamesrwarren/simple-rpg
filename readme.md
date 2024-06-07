## Simple RPG

The point of this project is a first go at a game using c++. 

To build and run

```
g++ -std=c++11 $(pkg-config --cflags sdl2) \
-o ../build/simple-rpg \
main.cpp \
engine/collisions.cpp \
characters/character.cpp \
characters/player/hero.cpp \
characters/npc/npc.cpp \
characters/terrain/terrain.cpp \
world/background.cpp \
$(pkg-config --libs sdl2 sdl2_image)
```

Trying to loosely stick to this plan though actually have no idea where this will take me yet...:

- Game loop - Done
- Map based on text file - Done
- Basic NPC (no graphics) - Done
- Basic Character with controls (no graphics) - Done
- Collision detection - Done
- Pathfinding and have NPC follow main character - In Progress
- Graphics for player character (4 frames per direction rotating)
- Graphics for npc character (4 frames per direction rotating)
- Create hit points and attack for NPC and player
- Create text showing hit points and attack power and armor
- Create refresh from start on player death
- Create items to pick up and work out logic for what happens on contact with them
- Refactor and work on logic a bit before seeing where to go next!

```
../build/simple-rpg
```

Because I'm a noob and don't know better, I'm keeping a devfile around for debugging specific functions and logic..

```
g++ -std=c++11 \
-o ../build/devfile \
devfile.cpp

../build/devfile
```