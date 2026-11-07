# Search Algorithms in SDL

A small C program that shows breadth-first search (BFS) pathfinding in real time, rendered with SDL2.

Click anywhere on the screen. BFS searches outward pixel by pixel (8 directions) from the player's position until it reaches the clicked point. The search progress and the path it finds are logged to the console.

## How it works

- **Queue**: a linked-list FIFO queue. Each node stores its parent, so the final path can be traced back from the goal.
- **Visited set**: a hash table keyed on (x, y) coordinates, so no point is explored twice.
- **BFS** (`src/ai.c`): expands neighbours level by level until it reaches the target, then rebuilds the path by following parent pointers.

```
src/main.c     window and renderer setup
src/game.c     game loop and input
src/player.c   player sprite and movement
src/ai.c       queue, hash table and BFS
```

## Building (Windows, MinGW)

SDL2 headers and libraries are bundled in `include/` and `lib/`.

```bat
build.bat
cd bin
main.exe
```

Run it from `bin/`, because assets are loaded from `../assets`.

Controls: **left-click** sets a target, and **Esc** quits.
